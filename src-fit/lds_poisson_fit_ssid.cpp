//===-- lds_poisson_fit_ssid.cpp - PLDS Fit (SSID) ------------------------===//
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
/// This file implements a type by which Poisson-output LDS models are fit by a
/// subspace identification (SSID) algorithm (`lds::gaussian::ssidFit_t`).
/// Currently, these models are fit assuming linear/Gaussian output,
/// and then an exponential output is fit by maximum likelihood under the
/// Poisson assumption. Future work should implement SSID after
/// Poisson<->Gaussian moment conversions as outlined in [2].
///
/// References:
/// [1] van Overschee P, de Moore B. (1996) Subspace Identification for Linear
/// Systems. Boston: Springer.
/// [2] Buesing L, Macke JH, Sahani M. (2012) Spectral learning of linear
/// dynamics from generalised-linear observations with application to neural
/// population data. NIPS 25.
///
/// \brief PLDS SSID fit type
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>

using namespace std;
using namespace plds;

lds::poisson::ssidFit_t::ssidFit_t(armaMat& A, armaMat& B, armaVec& g,
                                   armaVec& m, armaMat& Q, armaVec& x0,
                                   armaMat& P0, armaMat& C, armaVec& d,
                                   data_t dt, data_t t_startSSID,
                                   data_t t_stopSSID, armaVec& singVals,
                                   vector<data_t>& t0, vector<armaMat>& uTrain,
                                   vector<armaMat>& zTrain)
    : lds::poisson::fit_t(A, B, g, m, Q, x0, P0, C, d, dt, uTrain, zTrain) {
  this->t0 = t0;
  this->t_startSSID = t_startSSID;
  this->t_stopSSID = t_stopSSID;
  this->singVals = singVals;
}

void limit(armaVec& x, data_t lb, data_t ub);
void limit(armaVec& x, data_t lb, data_t ub) {
  for (size_t k = 0; k < x.n_elem; k++) {
    x[k] = x[k] < lb ? lb : x[k];
    x[k] = x[k] > ub ? ub : x[k];
  }
}

tuple<ssidFit_t, lds::gaussian::ssidFit_t> lds::poisson::ssidFit(
    vector<armaMat>& u, vector<armaMat>& n, data_t dt, size_t nX, size_t nH,
    armaVec d0, ssidWt wt, data_t wtG0, vector<data_t>& t0, data_t t_startSSID,
    data_t t_stopSSID, bool assumeGaussian) {
  // TODO(mfbolus): Need to implement Buesing, ..., Sahani 2012 Poisson <->
  // Gaussian moment conversions
  if (!assumeGaussian)
    cerr << "Poisson -> Gaussian moment conversion not implemented. Assuming "
            "linear/gaussian. Then refitting output function parameters "
            "assuming Poisson.\n";

  auto start_total = chrono::high_resolution_clock::now();

  // dims
  size_t nY = n[0].n_rows;
  size_t nU = u[0].n_rows;
  size_t nTrials = n.size();
  if (t0.size() != nTrials) {
    t0 = vector<data_t>(nTrials, 0.0);
  }

  // fit assuming linear/gaussian observations...
  // B/c of how fitOutput_mle() is written, it does not preserve the y=x
  // structure, so no point in allowing this right now.
  // TODO(mfbolus): rewrite fitOutput_mle() to only re-scale C rather than refit
  // each element. When this happens, will make the force_unitNormC option
  // visible
  bool force_unitNormC = false;
  lds::gaussian::ssidFit_t linFit =
      lds::gaussian::ssidFit(u, n, dt, nX, nH, d0, force_unitNormC, wt, wtG0,
                             t0, t_startSSID, t_stopSSID);

  armaMat C = linFit.C;
  armaVec d = linFit.d;

  cout << "Refitting output function parameters by Poisson MLE ...\n";
  auto start = chrono::high_resolution_clock::now();
  C *= 10;
  d = log(d);
  limit(d, -7, 7);
  // cout << "Initial guess for C: \n" << C << endl;
  // cout << "Initial guess for d: \n" << d << endl;

  // Simulate dynamics to get state estimates from linear fit
  vector<armaMat> x(nTrials, armaMat(nX, n[0].n_cols, fill::zeros));
  for (size_t k = 0; k < nTrials; k++) {
    x[k] = armaMat(nX, n[k].n_cols);
    for (size_t kk = 1; kk < n[k].n_cols; kk++) {
      x[k].col(kk) = linFit.A * x[k].col(kk - 1) + linFit.B * u[k].col(kk - 1);
    }
  }

  // iteratively refit C and d by Poisson MLE given x
  // (as if there is zero process noise)
  // n.b., this process is analogous to the nonlineary fitting step in Smith et
  // Brown 2003, except MLE rather than MAP
  fitOutput_mle(C, d, x, n, dt);

  vector<armaMat> y(nTrials, armaMat(nY, n[0].n_cols));
  for (size_t k = 0; k < nTrials; k++) {
    y[k] = armaMat(nY, n[k].n_cols);
    for (size_t kk = 1; kk < n[k].n_cols; kk++) {
      y[k].col(kk) = exp(C * x[k].col(kk - 1) + d);
    }
  }

  auto finish = chrono::high_resolution_clock::now();
  chrono::duration<data_t, milli> dur_ms = finish - start;
  cout << "Finished Poisson MLE. Elapsed: " << dur_ms.count() << " ms.\n";

  // create fit
  ssidFit_t fit(linFit.A, linFit.B, linFit.g, linFit.m, linFit.Q, linFit.x0,
                linFit.P0, C, d, dt, t_startSSID, t_stopSSID, linFit.singVals,
                t0, u, n);

  auto finish_total = chrono::high_resolution_clock::now();
  chrono::duration<data_t, milli> dur_total_ms = finish_total - start_total;
  cout << "Total Elapsed Time: " << dur_total_ms.count() << " ms.\n";
  cout << "************** Fin **************\n";

  return forward_as_tuple(fit, linFit);
}

void lds::poisson::fitOutput_mle(armaMat& C, armaVec& d, vector<armaMat>& x,
                                 vector<armaMat>& n, double& dt) {
  data_t tol = 1e-3;          // frac abs change
  size_t itersAllowed = 100;  // how many iterations allowed for convergence

  data_t nll(0); // negative log likelihood
  data_t nll_prev(-1); // "" previous iteration

  cout << "Initial guess for C[0]: " << C[0] << endl;

  // Get initial estimate. (d is good already so start with C)
  cout << "Getting initial estimate...\n";

  nll = newtonSolve_rescaleC_mle(C, d, x, n, dt);

  cout << "New estimate for C[0]: " << C[0] << endl;
  cout << "-logL = " << (nll) << endl;

  armaVec d_new = d;
  armaMat C_new = C;

  armaVec theta = join_vert(d, vectorise(C));
  armaVec theta_new = theta;

  data_t crit(0);
  data_t nResponses(0);
  for (size_t k = 0; k < n.size(); k++) {          // trial loop
    for (size_t kk = 0; kk < n[k].n_cols; kk++) {  // time step loop
      nResponses += sum(n[k].col(kk));
    }
  }

  // loop through multiple intereations (l)...
  for (size_t l = 0; l <= itersAllowed; l++) {
    // cout << "fitOutput_mle iteration: " << l+1;

    // joint iterations.
    nll = calcD_newtonSolve_rescaleC_mle(C_new, d_new, x, n, dt);
    cout << "New estimate for C[0]: " << C_new[0] << endl;
    cout << "New estimate for d[0]: " << d_new[0] << endl;
    cout << "-logL = " << (nll / nResponses) << endl;

    if ((l > 0) & ((nll - nll_prev) > 0)) {
      break;
    } else {
      nll_prev = nll;
    }

    theta = join_vert(d, vectorise(C));
    theta_new = join_vert(d_new, vectorise(C_new));

    crit = max(abs(theta - theta_new) / abs(theta));

    // assign to old
    C = C_new;
    d = d_new;

    if ((crit < tol)) {
      cout << "fitOutput_mle converged.\n";
      break;
    }
    if (l == (itersAllowed - 1)) cerr << "fitOutput_mle failed to converge.\n";
  }  // iterations loop
}  // fitOutput_mle

data_t lds::poisson::newtonSolve_rescaleC_mle(armaMat& C, armaVec& d,
                                              vector<armaMat>& x,
                                              vector<armaMat>& n, double& dt) {
  // rather than independently solving for elements of C, solve for a scaling
  // factor on C.
  data_t tol = 1e-3;  // frac abs change at convergence
  size_t itersAllowed =
      100;  // how many iterations of Newton's method allowed for convergence

  armaVec cx(1, fill::zeros);//single-output c*x
  armaVec r(1, fill::zeros);// single-output rate

  armaVec f(1, fill::zeros);
  armaMat fprime(1, 1, fill::zeros);//gradient wrt params
  armaMat f_over_fprime(1, 1, fill::zeros);
  data_t crit(0);

  armaVec nll(d.n_rows, fill::zeros);

  // Since they are independent, conditioned on state,
  // solve output-by-output (p)
  for (size_t p = 0; p < n[0].n_rows; p++) {  // output loop
    data_t gC_p(1);
    auto c_p = C.submat(p, 0, p, C.n_cols - 1);
    armaVec c_p_new = vectorise(c_p);

    // loop through multiple interations (l)...
    for (size_t l = 0; l <= itersAllowed; l++) {
      f.zeros();
      fprime.zeros();
      cx.zeros();

      for (size_t k = 0; k < x.size(); k++) {          // trial loop
        for (size_t kk = 0; kk < x[k].n_cols; kk++) {  // time samp loop
          cx = c_p * x[k].col(kk);
          r = exp(d[p] + gC_p * cx);
          limit(r, 1e-10, 10);
          f += r * cx - n[k].col(kk)[p] * cx;
          fprime += r * cx * cx.t();
        }  // time
      }    // trial
      f_over_fprime = inv(fprime) * f;
      f_over_fprime *= 1e-1;
      gC_p -= f_over_fprime[0];

      // rescale
      c_p_new = gC_p * c_p.t();
      gC_p = 1.0;  // then reset.
      crit = max(abs(c_p - c_p_new.t()) / abs(c_p));
      c_p = c_p_new.t();  // assign to old val.

      if (crit < tol) break;
      if (l == (itersAllowed - 1))
        cerr << "newtonSolveC_mle failed to converge for output " << p + 1
             << ".\n";
    }  // iterations loop

    // calculate likelihood
    nll[p] = 0;
    bool alreadyInf = false;
    for (size_t k = 0; k < x.size(); k++) {          // trial loop
      for (size_t kk = 0; kk < x[k].n_cols; kk++) {  // time samp loop
        cx = c_p * x[k].col(kk);
        nll[p] += exp(d[p] + cx[0]) - n[k].col(kk)[p] * (d[p] + cx[0]);
      }  // time
    }    // trial
  }      // outputs loop

  return (sum(nll));
}  // newtonSolve_rescaleC_mle

data_t lds::poisson::calcD_newtonSolve_rescaleC_mle(armaMat& C, armaVec& d,
                                                    vector<armaMat>& x,
                                                    vector<armaMat>& n,
                                                    double& dt) {
  // rather than independently solving for elements of C, solve for a scaling
  // factor.
  data_t tol = 1e-3;  // 1e-2;//frac abs change
  size_t itersAllowed =
      0;  // 100; //how many iterations of newtons allowed for convergence

  armaVec cx(1, fill::zeros);//single-output c*x
  armaVec r(1, fill::zeros);//single-output rate

  armaVec f(1, fill::zeros);
  armaMat fprime(1, 1, fill::zeros);//gradient
  armaMat f_over_fprime(1, 1, fill::zeros);
  data_t crit(0);

  armaVec nll(d.n_rows, fill::zeros);

  armaVec sumN(d.n_rows, fill::zeros);
  for (size_t k = 0; k < x.size(); k++) {
    sumN += sum(n[k], 1);
  }

  // Since they are independent, conditioned on state,
  // solve output-by-output (p)
  for (size_t p = 0; p < n[0].n_rows; p++) {  // output loop
    auto d_p = d.subvec(p, p);
    auto sumN_p = sumN[p];
    armaVec d_p_new = d_p;

    data_t gC_p(1);
    auto c_p = C.submat(p, 0, p, C.n_cols - 1);
    armaVec c_p_new = vectorise(c_p);

    // params for convergence evaluation.
    armaVec theta_p = join_vert(vectorise(c_p), d_p);
    armaVec theta_p_new = theta_p;

    // Solving for d analytically:
    armaVec sumR_p(1, fill::zeros);
    armaVec logr_p(1, fill::zeros);
    for (size_t k = 0; k < x.size(); k++) {
      for (size_t kk = 0; kk < x[k].n_cols; kk++) {
        logr_p = c_p * x[k].col(kk);  // + log_dt;
        // limit(logr_p, -50, 50);
        limit(logr_p, -10, 10);
        sumR_p += exp(logr_p);
      }
    }
    d_p_new = log(sumN_p) - log(sumR_p);
    limit(d_p_new, -7, 7);

    // uncomment to use this d in newton's below:
    d_p = d_p_new;

    // loop through multiple intereations (l)...
    for (size_t l = 0; l <= itersAllowed; l++) {
      f.zeros();
      fprime.zeros();
      cx.zeros();
      for (size_t k = 0; k < x.size(); k++) {          // trial loop
        for (size_t kk = 0; kk < x[k].n_cols; kk++) {  // time samp loop
          cx = c_p * x[k].col(kk);
          r = exp(d[p] + gC_p * cx);
          limit(r, 1e-10, 10);
          f += r * cx - n[k].col(kk)[p] * cx;
          fprime += r * cx * cx.t();
        }  // time
      }    // trial
      f_over_fprime = inv(fprime) * f;
      f_over_fprime *= 1e-1;  // for gentler convergence
      gC_p -= f_over_fprime[0];

      // rescale
      c_p_new = gC_p * c_p.t();
      gC_p = 1.0;  // then reset.
      theta_p_new = join_vert(c_p_new, d_p_new);
      crit = max(abs(theta_p - theta_p_new) / abs(theta_p));

      // assign to old val
      c_p = c_p_new.t();
      d_p = d_p_new;
      theta_p = theta_p_new;

      if ((crit < tol)) break;
      if (l == (itersAllowed - 1))
        cerr << "\nnewtonSolveC_mle failed to converge for output " << p + 1
             << ".\n";
    }  // iterations loop

    // calculate likelihood
    nll[p] = 0;
    for (size_t k = 0; k < x.size(); k++) {          // trial loop
      for (size_t kk = 0; kk < x[k].n_cols; kk++) {  // time samp loop
        cx = c_p * x[k].col(kk);
        nll[p] += exp(d_p[0] + cx[0]) - n[k].col(kk)[p] * (d_p[0] + cx[0]);
      }  // time
    }    // trial

  }  // outputs loop

  return (sum(nll));
}  // newtonSolve_rescaleC_calcD_mle

void lds::poisson::calcD_mle(armaMat& C, armaVec& d, vector<armaMat>& x,
                             vector<armaMat>& n, double& dt) {
  armaVec sumR(d.n_rows, fill::zeros);
  armaVec sumN(d.n_rows, fill::zeros);
  armaVec logr(d.n_rows, fill::zeros);

  for (size_t k = 0; k < x.size(); k++) {
    sumN += sum(n[k], 1);
    for (size_t kk = 0; kk < x[k].n_cols; kk++) {
      logr = C * x[k].col(kk);
      limit(logr, -50, 50);
      sumR += exp(logr);
    }
  }

  d = log(sumN) - log(sumR);
  limit(d, -7, 7);
}

void lds::poisson::newtonSolveC_mle(armaMat& C, armaVec& d, vector<armaMat>& x,
                                    vector<armaMat>& n, double& dt) {
  data_t tol = 1e-3;  // 1e-9; //frac abs change
  size_t itersAllowed =
      100;  // how many iterations of newtons allowed for convergence

  armaVec f(x[0].n_rows, fill::zeros);
  armaMat fprime(x[0].n_rows, x[0].n_rows, fill::zeros);//gradient
  armaMat f_over_fprime(1, x[0].n_rows, fill::zeros);
  data_t crit(0);

  // Since they are independent, conditioned on state,
  // solve output-by-output (p)
  for (size_t p = 0; p < n[0].n_rows; p++) {  // output loop
    auto c_p = C.submat(p, 0, p, C.n_cols - 1);
    armaVec c_p_new = vectorise(c_p);

    // loop through multiple intereations (l)...
    for (size_t l = 0; l <= itersAllowed; l++) {
      f.zeros();
      fprime.zeros();

      for (size_t k = 0; k < x.size(); k++) {          // trial loop
        for (size_t kk = 0; kk < x[0].n_cols; kk++) {  // time samp loop
          f += exp(d[p] + c_p * x[k].col(kk))[0] * x[k].col(kk) -
               n[k].col(kk)[p] * x[k].col(kk);
          fprime += exp(d[p] + c_p * x[k].col(kk))[0] * x[k].col(kk) *
                    x[k].col(kk).t();
        }  // time
      }    // trial
      f_over_fprime = inv(fprime) * f;
      // f_over_fprime = solve(fprime,f);

      // optionally could use a step size here to slow down if numerical issues:
      // f_over_fprime *= 0.1;

      c_p_new -= f_over_fprime;
      crit = max(abs(c_p - c_p_new.t()) / abs(c_p));
      c_p = c_p_new.t();  // assign to old val.
      if (crit < tol) break;
      if (l == itersAllowed)
        cerr << "newtonSolveC_mle failed to converge for output " << p + 1
             << ".\n";
    }  // iterations loop
  }    // outputs loop
}  // newtonSolveC_mle
