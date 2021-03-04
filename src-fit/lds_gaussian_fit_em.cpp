//===-- lds_gaussian_fit_em.cpp - GLDS Fit (EM) ---------------------------===//
//
// Copyright 2021 [name of copyright owner]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements a type by which Gaussian-output LDS models are fit by
/// the expectation-maximization (EM) algorithm (`lds::gaussian::emFit_t`).
///
/// References:
/// [1] Shumway RH, Stoffer DS. (1982) An Approach to Time Series Smoothing and
/// Forecasting Using the EM Algorithm.Journal of Time Series Analysis 3(2).
///
/// [2] Ghahramani Z, Hinton GE. (1996) Parameter Estimation for Linear
/// Dynamical Systems. Technical Report CRG-TR-96-2.
///
/// \brief GLDS E-M fit type
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>

using namespace std;
using namespace glds;

// TODO(mfbolus): this is a fudge, but for some reason, cov mats often going
// numerically asymm.
armaMat forceSymPD(armaMat X) {
  X = (X + X.t()) / 2;

  armaVec d;
  armaMat U;
  eig_sym(d, U, X, "std");
  armaMat D = diagmat(abs(d));  // force to be positive...
  X = U * D * U.t();

  return X;
};

glds::emFit_t::emFit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m,
                       armaMat& Q, armaVec& x0, armaMat& P0, armaMat& C,
                       armaVec& d, armaMat& R, data_t dt,
                       vector<armaMat>& uTrain, vector<armaMat>& zTrain)
    : glds::fit_t(A, B, g, m, Q, x0, P0, C, d, R, dt, uTrain, zTrain) {
  reset();
}

// Initialize to nX=nY random walk
glds::emFit_t::emFit_t(data_t dt, vector<armaMat>& uTrain,
                       vector<armaMat>& zTrain) {
  size_t nU = uTrain[0].n_rows;
  size_t nY = zTrain[0].n_rows;
  size_t nX = nY;
  size_t nTrials = zTrain.size();
  size_t nSamps = zTrain[0].n_cols;

  this->dt = dt;
  this->zTrain = zTrain;
  this->uTrain = uTrain;

  A = armaMat(nX, nX, fill::eye);
  B = armaMat(nX, nU, fill::zeros);
  g = armaVec(nU, fill::ones);
  m = armaVec(nX, fill::zeros);
  Q = lds::DEFAULT_Q0 * armaMat(nX, nX, fill::eye);
  x0 = armaVec(nX, fill::zeros);
  P0 = lds::DEFAULT_P0 * armaMat(nX, nX, fill::eye);

  C = armaMat(nY, nY, fill::eye);
  d = calcD_silence(zTrain, uTrain, dt);
  R = lds::DEFAULT_R0 * armaMat(nY, nY, fill::eye);

  xHat = std::vector<armaMat>(nTrials, armaMat(nX, nSamps, fill::zeros));
  yHat = std::vector<armaMat>(nTrials, armaMat(nX, nSamps, fill::zeros));
  P = std::vector<armaCube>(nTrials, armaCube(nX, nX, nSamps, fill::zeros));
  P_t_tm1 =
      std::vector<armaCube>(nTrials, armaCube(nX, nX, nSamps, fill::zeros));

  // in case not homogeneous trial length
  for (size_t trial = 0; trial < nTrials; trial++) {
    nSamps = zTrain[trial].n_cols;
    xHat[trial] = armaMat(nX, nSamps, fill::zeros);
    yHat[trial] = armaMat(nY, nSamps, fill::zeros);
    P[trial] = armaCube(nX, nX, nSamps, fill::zeros);
    P_t_tm1[trial] = armaCube(nX, nX, nSamps, fill::zeros);
  }

  T = 0;  // time samples
  sum_E_xu_t_xu_t =
      armaMat(nX + nU, nX + nU, fill::zeros);  // state-input covariance
  sum_E_xu_tm1_xu_tm1 =
      armaMat(nX + nU, nX + nU, fill::zeros);  // state-input covariance (past)
  sum_E_xu_t_xu_tm1 =
      armaMat(nX + nU, nX + nU, fill::zeros);  // lag-one state-input covariance
}

glds::emFit_t::emFit_t(glds::ssidFit_t& fit0) {
  A = fit0.A;
  B = fit0.B;
  g = fit0.g;
  m = fit0.m;
  Q = fit0.Q;
  x0 = fit0.x0;
  P0 = fit0.P0;
  C = fit0.C;
  d = fit0.d;
  R = fit0.R;
  dt = fit0.dt;

  uTrain = move(fit0.uTrain);
  zTrain = move(fit0.zTrain);

  reset();
}

void glds::emFit_t::reset(void) {
  size_t nX = x0.n_elem;
  size_t nU = uTrain[0].n_rows;
  size_t nY = C.n_rows;
  size_t nTrials = zTrain.size();
  size_t nSamps = zTrain[0].n_cols;

  xHat = std::vector<armaMat>(nTrials, armaMat(nX, nSamps, fill::zeros));
  yHat = std::vector<armaMat>(nTrials, armaMat(nX, nSamps, fill::zeros));
  P = std::vector<armaCube>(nTrials, armaCube(nX, nX, nSamps, fill::zeros));
  P_t_tm1 =
      std::vector<armaCube>(nTrials, armaCube(nX, nX, nSamps, fill::zeros));

  // in case not homogeneous trial length
  for (size_t trial = 0; trial < nTrials; trial++) {
    nSamps = zTrain[trial].n_cols;
    xHat[trial] = armaMat(nX, nSamps, fill::zeros);
    yHat[trial] = armaMat(nY, nSamps, fill::zeros);
    P[trial] = armaCube(nX, nX, nSamps, fill::zeros);
    P_t_tm1[trial] = armaCube(nX, nX, nSamps, fill::zeros);
  }

  T = 0;
  // covariances (see above)
  sum_E_xu_t_xu_t = armaMat(nX + nU, nX + nU, fill::zeros);
  sum_E_xu_tm1_xu_tm1 = armaMat(nX + nU, nX + nU, fill::zeros);
  sum_E_xu_t_xu_tm1 = armaMat(nX + nU, nX + nU, fill::zeros);
}

void glds::emFit_t::runEM(bool calcAB, bool calcQ, bool calcInitial, bool calcC,
                          bool calcd, bool calcR) {
  if (!Q.is_sympd()) {
    cerr << "WARNING: Q not symmetric pd. Forcing...\n";
    Q = forceSymPD(Q);
  }

  if (!P0.is_sympd()) {
    cerr << "WARNING: P0 not symmetric pd. Forcing...\n";
    P0 = forceSymPD(P0);
  }

  if (!R.is_sympd()) {
    cerr << "WARNING: R not symmetric pd. Forcing...\n";
    R = forceSymPD(R);
  }

  size_t nX = xHat[0].n_rows;
  size_t nU = uTrain[0].n_rows;
  size_t nY = yHat[0].n_rows;

  size_t nParams = 3 * nX * nX + nX * nU + nX + nY * nX + nY * nY;
  armaVec theta(nParams);
  armaVec theta_new(nParams);
  data_t max_dTheta = 1;

  // to make things simpler, multiply u->g
  armaVec g0 = g;
  for (size_t trial = 0; trial < uTrain.size(); trial++) {
    uTrain[trial].each_col() %= g;
  }
  g.ones();

  // initialize to x0, P0
  for (size_t trial = 0; trial < uTrain.size(); trial++) {
    xHat[trial].col(0) = x0;
    P[trial].slice(0) = P0;
    yHat[trial].col(0) = C * xHat[trial].col(0) + d;
  }

  // if solving for initial conditions,
  // allow them to be varied.
  // Otherwise freeze at provided values.
  bool forceCommonInitial = true;
  if (calcInitial) {
    forceCommonInitial = false;
  };

  // go until parameter convergence
  for (size_t l = 0; l < maxIter; l++) {
    theta = getTheta();

    cout << "Iteration " << l + 1 << "/" << maxIter << " ...\n";

    Estep(forceCommonInitial);
    Mstep(calcAB, calcQ, calcInitial, calcC, calcd, calcR);

    // check convergence
    theta_new = getTheta();

    // some parameters could be zero...
    armaVec dTheta = abs(theta_new - theta) / abs(theta);
    arma::uvec ubiFinite = find_finite(dTheta);

    max_dTheta = max(dTheta.elem(ubiFinite));
    cout << "max_dTheta: " << max_dTheta << endl;
    if (max_dTheta < tol) {
      cout << "Converged." << endl;
      break;
    }

    cout << endl;
  }

  // Put u/g back
  for (size_t trial = 0; trial < uTrain.size(); trial++) {
    uTrain[trial].each_col() /= g0;
  }
  g = g0;
}

armaVec glds::emFit_t::getTheta() {
  // TODO(mfbolus): This should include nY more params for d.
  size_t nX = xHat[0].n_rows;
  size_t nU = uTrain[0].n_rows;
  size_t nY = yHat[0].n_rows;
  size_t nParams = 3 * nX * nX + nX * nU + nX + nY * nX + nY * nY;
  armaVec theta(nParams);

  size_t idxStart = 0;
  theta.subvec(idxStart, idxStart + nX * nX - 1) = vectorise(A);
  idxStart += nX * nX;
  theta.subvec(idxStart, idxStart + nX * nU - 1) = vectorise(B);
  idxStart += nX * nU;
  theta.subvec(idxStart, idxStart + nX * nX - 1) = vectorise(Q);
  idxStart += nX * nX;
  theta.subvec(idxStart, idxStart + nX - 1) = vectorise(x0);
  idxStart += nX;
  theta.subvec(idxStart, idxStart + nX * nX - 1) = vectorise(P0);
  idxStart += nX * nX;
  theta.subvec(idxStart, idxStart + nY * nX - 1) = vectorise(C);
  idxStart += nY * nX;
  theta.subvec(idxStart, idxStart + nY * nY - 1) = vectorise(R);

  return theta;
}

// Estimate x_t|t
void glds::emFit_t::kalmanFilt() {
  size_t nX = xHat[0].n_rows;
  size_t nY = yHat[0].n_rows;
  armaMat K(nX, nY);

  for (size_t trial = 0; trial < zTrain.size(); trial++) {
    xHat[trial].col(0) = x0;
    P[trial].slice(0) = P0;
    yHat[trial].col(0) = C * xHat[trial].col(0) + d;

    for (size_t t = 1; t < zTrain[trial].n_cols; t++) {
      // predict
      xHat[trial].col(t) =
          A * xHat[trial].col(t - 1) + B * uTrain[trial].col(t - 1) + m;
      P[trial].slice(t) = A * P[trial].slice(t - 1) * A.t() + Q;
      yHat[trial].col(t) = C * xHat[trial].col(t) + d;

      // update
      K = P[trial].slice(t) * C.t() *
          inv_sympd(C * P[trial].slice(t) * C.t() + R);
      xHat[trial].col(t) =
          xHat[trial].col(t) + K * (zTrain[trial].col(t) - yHat[trial].col(t));
      P[trial].slice(t) = P[trial].slice(t) - K * C * P[trial].slice(t);
      yHat[trial].col(t) = C * xHat[trial].col(t) + d;
    }
  }
}

// Estimate x_t|T
void glds::emFit_t::kalmanSmooth(bool forceCommonInitial) {
  size_t nX = xHat[0].n_rows;
  size_t nY = yHat[0].n_rows;
  armaMat K(nX, nY);  // kalman gain
  arma::Cube<data_t> J;

  for (size_t trial = 0; trial < zTrain.size(); trial++) {
    size_t nSamps = zTrain[trial].n_cols;

    armaMat x_pre(nX, nSamps, fill::zeros);
    armaCube P_pre(nX, nX, nSamps, fill::zeros);
    armaMat x_post(nX, nSamps, fill::zeros);
    armaCube P_post(nX, nX, nSamps, fill::zeros);

    if (forceCommonInitial)  // forces all trials to have same initial
                             // conditions.
    {
      xHat[trial].col(0) = x0;
      P[trial].slice(0) = P0;
    }
    yHat[trial].col(0) = C * xHat[trial].col(0) + d;

    // This *should not* be necessary but make sure P is symmetric.
    if (!P[trial].slice(0).is_sympd()) {
      P[trial].slice(0) = forceSymPD(P[trial].slice(0));
    }

    x_pre.col(0) = xHat[trial].col(0);
    x_post.col(0) = xHat[trial].col(0);
    P_pre.slice(0) = P[trial].slice(0);
    P_post.slice(0) = P[trial].slice(0);

    // kalman filter
    for (size_t t = 1; t < nSamps; t++) {
      // predict
      x_pre.col(t) = A * x_post.col(t - 1) + B * uTrain[trial].col(t - 1) + m;
      P_pre.slice(t) = A * P_post.slice(t - 1) * A.t() + Q;
      yHat[trial].col(t) = C * x_pre.col(t) + d;

      // update --> posterior estimation
      // K = P_pre.slice(t) * C.t() * inv( C*P_pre.slice(t)*C.t() + R );
      K = P_pre.slice(t) * C.t() * inv_sympd(C * P_pre.slice(t) * C.t() + R);
      x_post.col(t) =
          x_pre.col(t) + K * (zTrain[trial].col(t) - yHat[trial].col(t));
      P_post.slice(t) = P_pre.slice(t) - K * C * P_pre.slice(t);
      yHat[trial].col(t) = C * x_post.col(t) + d;
    }

    // backfilter -> Smoothed estimate
    // Reference:
    // Shumway et Stoffer (1982)

    // This *should not* be necessary but make sure P is symmetric.
    if (!P[trial].slice(nSamps - 1).is_sympd()) {
      P[trial].slice(nSamps - 1) = forceSymPD(P[trial].slice(nSamps - 1));
    }

    armaCube J(nX, nX, nSamps, fill::zeros);
    xHat[trial].col(nSamps - 1) = x_post.col(nSamps - 1);
    P[trial].slice(nSamps - 1) = P_post.slice(nSamps - 1);
    for (size_t t = (nSamps - 1); t > 0; t--) {
      J.slice(t - 1) = P_post.slice(t - 1) * A.t() * inv_sympd(P_pre.slice(t));
      xHat[trial].col(t - 1) =
          x_post.col(t - 1) +
          J.slice(t - 1) * (xHat[trial].col(t) - x_pre.col(t));
      P[trial].slice(t - 1) =
          P_post.slice(t - 1) + J.slice(t - 1) *
                                    (P[trial].slice(t) - P_pre.slice(t)) *
                                    J.slice(t - 1).t();
    }

    // do the same for P_t_tm1
    armaMat I(nX, nX, fill::eye);
    P_t_tm1[trial].slice(nSamps - 1) =
        (I - K * C) * A * P_post.slice(nSamps - 2);
    for (size_t t = (nSamps - 1); t > 1; t--) {
      P_t_tm1[trial].slice(t - 1) =
          P_post.slice(t - 1) * J.slice(t - 2).t() +
          J.slice(t - 1) * (P_t_tm1[trial].slice(t) - A * P_post.slice(t - 1)) *
              J.slice(t - 2).t();
    }

    // finally, get smoothed estimate of output
    yHat[trial].col(0) = C * xHat[trial].col(0) + d;
    for (size_t t = 1; t < nSamps; t++) {
      yHat[trial].col(t) = C * xHat[trial].col(t) + d;
    }  // samps loop
  }    // trial loop
}

// Calculate needed expectations
void glds::emFit_t::Estep(bool forceCommonInitial) {
  // calculate the mean/cov of state needed for maximizing E[pr(z|theta)]
  kalmanSmooth(forceCommonInitial);

  size_t nX = xHat[0].n_rows;
  size_t nU = uTrain[0].n_rows;
  size_t nY = yHat[0].n_rows;

  // now get the various forms of sum(E[xx']) needed
  // n.b. Going to start at t=1 rather than 0 bc most max terms need that.
  // so really "T" is T-1
  T = 0;
  sum_E_xu_t_xu_t = armaMat(nX + nU, nX + nU, fill::zeros);
  sum_E_xu_tm1_xu_tm1 = armaMat(nX + nU, nX + nU, fill::zeros);
  sum_E_xu_t_xu_tm1 = armaMat(nX + nU, nX + nU, fill::zeros);

  armaVec xu_tm1(nX + nU, fill::zeros);
  armaVec xu_t(nX + nU, fill::zeros);

  for (size_t trial = 0; trial < zTrain.size(); trial++) {
    size_t nSamps = zTrain[trial].n_cols;
    for (size_t t = 1; t < nSamps; t++) {
      // ------------ sum_E_xu_t_xu_t ------------
      xu_t = join_vert(xHat[trial].col(t), uTrain[trial].col(t));
      sum_E_xu_t_xu_t += xu_t * xu_t.t();
      sum_E_xu_t_xu_t.submat(0, 0, nX - 1, nX - 1) += P[trial].slice(t);

      // ------------ sum_E_xu_tm1_xu_tm1 ------------
      xu_tm1 = join_vert(xHat[trial].col(t - 1), uTrain[trial].col(t - 1));
      sum_E_xu_tm1_xu_tm1 += xu_tm1 * xu_tm1.t();
      sum_E_xu_tm1_xu_tm1.submat(0, 0, nX - 1, nX - 1) += P[trial].slice(t - 1);

      // ------------ sum_E_xu_t_xu_tm1 ------------
      sum_E_xu_t_xu_tm1 += xu_t * xu_tm1.t();
      sum_E_xu_t_xu_tm1.submat(0, 0, nX - 1, nX - 1) += P_t_tm1[trial].slice(t);

      T += 1;
    }  // time
  }    // trial
}  // Estep

void glds::emFit_t::Mstep(bool calcAB, bool calcQ, bool calcInitial, bool calcC,
                          bool calcd, bool calcR) {
  size_t nX = xHat[0].n_rows;
  size_t nU = uTrain[0].n_rows;
  size_t nY = yHat[0].n_rows;

  if (calcC) {
    if (calcd) {
      // solve for C+d:
      armaMat sumZX(nY, nX + 1, fill::zeros);
      armaVec x1(nX + 1, fill::zeros);
      x1[nX] = 1.0;//augment with one to solve for bias
      armaMat sum_E_x1_x1(nX + 1, nX + 1, fill::zeros);
      size_t nSamps;
      for (size_t trial = 0; trial < zTrain.size(); trial++) {
        nSamps = zTrain[trial].n_cols;
        for (size_t t = 1; t < nSamps; t++) {
          x1.subvec(0, nX - 1) = xHat[trial].col(t);
          sumZX += zTrain[trial].col(t) * x1.t();
          sum_E_x1_x1 += x1 * x1.t();
          sum_E_x1_x1.submat(0, 0, nX - 1, nX - 1) += P[trial].slice(t);
        }
      }
      armaMat Cd = sumZX * inv_sympd(sum_E_x1_x1);
      C = Cd.submat(0, 0, nY - 1, nX - 1);
      d = vectorise(Cd.submat(0, nX, nY - 1, nX));
      cout << "C_new[0]: " << C[0] << endl;
      cout << "d_new[0]: " << d[0] << endl;
    } else {
      // do NOT solve for d:
      armaMat sumZX(nY, nX, fill::zeros);
      for (size_t trial = 0; trial < zTrain.size(); trial++) {
        size_t nSamps = zTrain[trial].n_cols;
        for (size_t t = 1; t < nSamps; t++) {
          // (z-d) = Cx+noise
          sumZX += (zTrain[trial].col(t) - d) * xHat[trial].col(t).t();
        }
      }
      // only need top elements bc no feedthrough (D)
      C = sumZX * inv_sympd(sum_E_xu_t_xu_t.submat(0, 0, nX - 1, nX - 1));
      cout << "C_new[0]: " << C[0] << endl;
    }
  } else if (calcd) {
    armaMat sumZX(nY, 1, fill::zeros);
    size_t nSamps;
    for (size_t trial = 0; trial < zTrain.size(); trial++) {
      nSamps = zTrain[trial].n_cols;
      for (size_t t = 1; t < nSamps; t++) {
        sumZX += zTrain[trial].col(t) - C * xHat[trial].col(t);
      }
    }
    d = vectorise(sumZX / T);
    cout << "d_new[0]: " << d[0] << endl;
  }

  if (calcR) {
    size_t T = 0;
    // Ghahgramani, Hinton 1996:
    armaMat sumZZ(nY, nY, fill::zeros);
    armaMat sumYZ(nY, nY, fill::zeros);
    for (size_t trial = 0; trial < zTrain.size(); trial++) {
      size_t nSamps = zTrain[trial].n_cols;
      for (size_t t = 1; t < nSamps; t++) {
        sumZZ += zTrain[trial].col(t) * zTrain[trial].col(t).t();
        // Use Cnew:
        sumYZ += (C * xHat[trial].col(t) + d) * zTrain[trial].col(t).t();
        T += 1;
      }
    }
    R = (sumZZ - sumYZ) / T;

    if (!R.is_sympd()) {
      cerr << "WARNING: R not symmetric pd. Forcing...\n";
      R = forceSymPD(R);
    }
    cout << "R_new[0]: " << R[0] << endl;
  }

  armaMat AB = join_horiz(A, B);
  armaMat phi;
  if (calcAB) {
    phi = sum_E_xu_t_xu_tm1 * inv_sympd(sum_E_xu_tm1_xu_tm1);
    // TODO(mfbolus): above could be optimized to solve only for A, B
    // As is, [A B] makes up part of what we just solved for (`phi`).
    AB = AB.submat(0, 0, nX - 1, nX + nU - 1);

    A = AB.submat(0, 0, nX - 1, nX - 1);
    B = AB.submat(0, nX, nX - 1, nX + nU - 1);
    cout << "A_new[0]: " << A[0] << endl;
    cout << "B_new[0]: " << B[0] << endl;
  }

  if (calcQ) {
    if (calcAB) {
      // n.b., Ghahgramani, Hinton 1996 use new AB
      Q = (sum_E_xu_t_xu_t - phi * sum_E_xu_t_xu_tm1.t()) / T;
    } else {
      // Shumway, Stoffer 1982
      // Same as the above, but in case did not re-solve for AB
      Q = (sum_E_xu_t_xu_t - sum_E_xu_t_xu_tm1 *
                                 inv_sympd(sum_E_xu_tm1_xu_tm1) *
                                 sum_E_xu_t_xu_tm1.t()) /
          T;
    }
    // TODO(mfbolus): above could be optimized similar to AB; only part of what
    // was calculated is the state covariance
    Q = Q.submat(0, 0, nX - 1, nX - 1);

    if (!Q.is_sympd()) {
      cerr << "WARNING: Q not symmetric pd. Forcing...\n";
      Q = forceSymPD(Q);
    }
    cout << "Q_new[0]: " << Q[0] << endl;
  }

  if (calcInitial) {
    x0.zeros();
    for (size_t trial = 0; trial < zTrain.size(); trial++) {
      x0 += xHat[trial].col(0);
    }
    x0 /= zTrain.size();
    cout << "x0_new[0]: " << x0[0] << endl;

    // always recalc P0 even if the initial state is fixed (at zero, for
    // example)
    armaMat eVar(nX, nX, fill::zeros);
    for (size_t trial = 0; trial < zTrain.size(); trial++) {
      eVar += (xHat[trial].col(0) - x0) * (xHat[trial].col(0) - x0).t();
    }
    eVar /= zTrain.size();

    // go ahead and subtract x0*x0' so don't have to below.
    eVar -= x0 * x0.t();

    // To get P0, going to get initial P per trial and average.
    // (which might be wrong, but need a single number)
    P0.zeros();
    for (size_t trial = 0; trial < zTrain.size(); trial++) {
      P0 += (xHat[trial].col(0) * xHat[trial].col(0).t()) + P[trial].slice(0) +
            eVar;
    }
    P0 /= zTrain.size();

    // This *should not* be necessary but make sure P0 is symmetric.
    if (!P0.is_sympd()) {
      cerr << "WARNING: P0 not symmetric pd. Forcing...\n";
      P0 = forceSymPD(P0);
    }
    cout << "P0_new[0]: " << P0[0] << endl;
  }  // Initial state/cov.
}  // Mstep

// special case where y = x + noise
emFit_t glds::emFit_x_equals_y(vector<armaMat>& uTrain, vector<armaMat>& zTrain,
                               data_t dt, size_t maxIter, data_t tol, data_t q0,
                               bool calcAB, bool calcQ, bool calcR) {
  emFit_t fit(dt, uTrain, zTrain);//initializes to random walk
  fit.maxIter = maxIter;
  fit.tol = tol;

  size_t nX = fit.x0.n_elem;
  fit.Q = q0 * armaMat(nX, nX, fill::eye);

  // leave C and initial conditions as they are.
  bool calcInitial = false;
  bool calcC = false;
  bool calcd = calcAB;//if recalculating AB, estimate bias
  fit.runEM(calcAB, calcQ, calcInitial, calcC, calcd, calcR);

  if (calcAB) {
    // make the output bias (d) the non-zero mean of process noise/disturbance
    // (m) so that `y = x + noise` still.
    fit.m += fit.d / (fit.C * inv(armaMat(nX, nX, fill::eye) - fit.A));
    fit.d.zeros();

    fit.x0 += inv(armaMat(nX, nX, fill::eye) - fit.A) * fit.m;
  }

  return fit;
}
