//===-- lds_gaussian_fit_ssid.cpp - GLDS Fit (SSID)
//-------------------------===//
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
/// This file implements a type by which Gaussian-output LDS models are fit by a
/// subspace identification (SSID) algorithm (`lds::gaussian::ssidFit_t`).
///
/// References:
/// [1] van Overschee P, de Moore B. (1996) Subspace Identification for Linear
/// Systems. Boston: Springer.
///
/// \brief GLDS SSID fit type
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>

using namespace std;
using namespace glds;

lds::gaussian::ssidFit_t::ssidFit_t(
    armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q, armaVec& x0,
    armaMat& P0, armaMat& C, armaMat& D, armaVec& d, armaMat& R, data_t dt,
    data_t t_startSSID, data_t t_stopSSID, armaVec& singVals,
    vector<data_t>& t0, vector<armaMat>& uTrain, vector<armaMat>& zTrain)
    : lds::gaussian::fit_t(A, B, g, m, Q, x0, P0, C, D, d, R, dt, uTrain,
                           zTrain) {
  this->t0 = t0;
  this->t_startSSID = t_startSSID;
  this->t_stopSSID = t_stopSSID;
  this->singVals = singVals;
}

ssidFit_t lds::gaussian::ssidFit(vector<armaMat>& u, vector<armaMat>& z,
                                 data_t dt, size_t nX, size_t nH, armaVec d0,
                                 bool force_unitNormC, ssidWt wt, data_t wtG0,
                                 vector<data_t>& t0, data_t t_startSSID,
                                 data_t t_stopSSID) {
  cout << "************** Fitting order " << nX << " model **************\n\n";
  // dims
  size_t nY = z[0].n_rows;
  size_t nU = u[0].n_rows;
  size_t nTrials = z.size();
  if (t0.size() != nTrials) {
    t0 = vector<data_t>(nTrials, 0.0);
  }

  // Subselect data between times t_startSSID and t_stopSSID
  // (This is for cases where you don't want to fit to all the training data.)
  //
  // TODO(mfbolus): As is, this is a wasteful copy of data. Ideally, there would
  // not be any copying of data until making H could I make zSSID and uSSID
  // composites of submats?
  //
  // TODO(mfbolus): may want to remove this "feature" altogether and just assume
  // users have given only the data they want to fit...which is a good
  // assumption.
  arma::uvec idxStart = arma::uvec(z.size(), fill::zeros);
  arma::uvec idxStop = arma::uvec(z.size(), fill::zeros);
  vector<armaVec> t = vector<armaVec>(
      z.size(), arma::linspace<armaVec>(t0[0], t0[0] + dt * (z[0].n_cols - 1),
                                        z[0].n_cols));
  size_t nSamps = 0;
  for (size_t k = 0; k < z.size(); k++) {
    t[k] = arma::linspace<armaVec>(t0[k], t0[k] + dt * (z[k].n_cols - 1),
                                   z[k].n_cols);
    arma::uvec t_greater_start = t[k] > t_startSSID;
    arma::uvec t_less_stop = t[k] < t_stopSSID;

    arma::uvec idxStart_k = find(t_greater_start, 1, "first");
    arma::uvec idxStop_k = find(t_less_stop, 1, "last");

    idxStart[k] = idxStart_k[0];
    idxStop[k] = idxStop_k[0];
    nSamps += idxStop[k] - idxStart[k] + 1;
  }

  armaMat zSSID(nY, nSamps, fill::zeros);
  armaMat uSSID(nU, nSamps, fill::zeros);
  nSamps = 0;
  for (size_t k = 0; k < z.size(); k++) {
    size_t len = idxStop[k] - idxStart[k] + 1;
    zSSID.submat(0, nSamps, nY - 1, nSamps + len - 1) =
        z[k].submat(0, idxStart[k], nY - 1, idxStop[k]);
    uSSID.submat(0, nSamps, nU - 1, nSamps + len - 1) =
        u[k].submat(0, idxStart[k], nU - 1, idxStop[k]);
    nSamps += len;
  }

  // creating defaults for things not explicitly solved for
  // TODO(mfbolus): at least x0, P0 should be fit by ssid (see van Overschee, de
  // Moor 1996)
  armaVec m(nX, fill::zeros);
  armaVec d(nY, fill::zeros);

  armaVec g(nU, fill::ones);
  armaVec x0(nX, fill::zeros);
  armaMat P0 = armaMat(nX, nX).fill(lds::fill::eye) * lds::DEFAULT_P0;

  // Get output bias if not provided at time of function call.
  if (d0.is_finite() && (d0.n_rows == nY)) {
    // If given a valid seed value for d, use that instead.
    d = d0;
  } else {
    // TODO(mfbolus): implement least-square solution for impulse response with
    // a second input of ones. Data-driven way of accounting for offset *not*
    // driven by an input.
    //
    // For now, calculate output bias (d) as the
    // output wherever the stimulus has not been on for some amount of time.
    // convolve u with rectangle and take all samples. This is a reasonable
    // approach, since often when autonomous systems are fit (i.e., systems with
    // no input), they will subtract off the mean of the output. This
    // essentially amounts to setting output bias to the mean of the output when
    // there is no stimulation.
    d = calcD_silence(z, u, dt);
  }
  zSSID.each_col() -= d;

  // things to be fit below
  armaMat A;
  armaMat B;
  armaMat Q;
  armaMat C;
  armaMat D(nY, nU, fill::zeros);
  armaMat R;
  armaVec s;  // singular values from the SVD at heard off SSID approach

  ssid_n4sid(nX, nU, nY, nH, uSSID, zSSID, A, B, C, D, Q, R, m, d, s,
             force_unitNormC, wt, wtG0);

  // this is a more direct implementation of methods from van Overschee, de Moor
  // 1996:
  //
  // ssid_n4sid_vanOverschee(nX, nU, nY, nH, uSSID, zSSID, A, B, C, D, Q,
  // R, s, wt);

  // just return the first nH singular values
  armaVec singVals = s.subvec(0, nH - 1);

  // write to fit...
  ssidFit_t fit(A, B, g, m, Q, x0, P0, C, D, d, R, dt, t_startSSID, t_stopSSID,
                singVals, t0, u, z);
  cout << "************** Fin **************\n\n";
  return fit;
}

armaVec lds::gaussian::calcD_silence(vector<armaMat>& z, vector<armaMat>& u,
                                     double dt, double tSilence,
                                     data_t threshSilence) {
  armaVec d(z[0].n_rows, fill::zeros);
  armaVec win((size_t)(tSilence / dt), fill::ones);
  for (size_t trial = 0; trial < u.size(); trial++) {
    // find silent samples

    // start by convolving with
    armaVec sumU = vectorise(sum(abs(u[trial]), 0));
    armaVec uConv = conv(sumU, win, "same");

    // get only the samples that are silent...
    arma::uvec ubiSilence = find(uConv <= threshSilence);

    if (ubiSilence.n_elem > 0) {
      armaVec mu_trial = mean(z[trial].cols(ubiSilence), 1);
      d += mu_trial;
    } else {
      cerr << "No silence in trial " << trial << ". Setting d to zero.";
      d *= 0.0;
      break;
    }
  }
  d /= (data_t)z.size();
  return d;
}

void lds::gaussian::createHankelMat_n4sid(armaMat& u, armaMat& z, size_t& nH,
                                          armaMat& H) {
  size_t N = z.n_cols - 2 * nH + 1;  // data length
  size_t nY = z.n_rows;
  size_t nU = u.n_rows;

  // dimensionality check
  if (N < (2 * nH * (nU + nY))) {
    cerr << "Dataset problem! More rows than columns in block-hankel data "
            "matrix: 2*(nU+nY)*nH > data-length! Need higher data-length or "
            "lower nH."
         << endl;
  }

  // block-hankel data matrix
  H = armaMat(2 * nH * (nU + nY), N);
  auto Up = H.submat(0, 0, nH * nU - 1, N - 1);            // past input
  auto Uf = H.submat(nH * nU, 0, 2 * nH * nU - 1, N - 1);  // future input
  auto Yp =
      H.submat(2 * nH * nU, 0, nH * (2 * nU + nY) - 1, N - 1);  // past output
  auto Yf = H.submat(nH * (2 * nU + nY), 0, 2 * nH * (nU + nY) - 1,
                     N - 1);  // future output

  size_t idx = 0;
  for (size_t k = 0; k < N; k++) {
    idx = 0;
    for (size_t kk = k; kk < (nH + k); kk++) {
      Up.col(k).subvec(idx, idx + nU - 1) = u.col(kk);
      idx += nU;
    }

    idx = 0;
    for (size_t kk = (nH + k); kk < (2 * nH + k); kk++) {
      Uf.col(k).subvec(idx, idx + nU - 1) = u.col(kk);
      idx += nU;
    }

    idx = 0;
    for (size_t kk = k; kk < (nH + k); kk++) {
      Yp.col(k).subvec(idx, idx + nY - 1) = z.col(kk);
      idx += nY;
    }

    idx = 0;
    for (size_t kk = (nH + k); kk < (2 * nH + k); kk++) {
      Yf.col(k).subvec(idx, idx + nY - 1) = z.col(kk);
      idx += nY;
    }
  }

  // TODO(mfbolus): subtract mean?
  // armaVec m = mean(H,1);
  // H.each_col() -= m;

  H /= sqrt((double)N);
}

void lds::gaussian::ssid_n4sid_vanOverschee(size_t& nX, size_t& nU, size_t& nY,
                                            size_t& nH, armaMat& uSSID,
                                            armaMat& zSSID, armaMat& A,
                                            armaMat& B, armaMat& C, armaMat& D,
                                            armaMat& Q, armaMat& R, armaVec& s,
                                            ssidWt wt) {
  // van Overschee, de Moor (1996)
  // van Overschee, de Moor `subid.m` re-implementation
  // https://www.mathworks.com/matlabcentral/fileexchange/2290-subspace-identification-for-linear-systems
  // (matlab scripts accompanying book)

  // create hankel mat
  armaMat H;  // data matrix [Up; Uf; Yp; Yf]
  createHankelMat_n4sid(uSSID, zSSID, nH, H);

  size_t N = H.n_cols;

  // do LQ decomp
  // Note that "R" in van Overschee is lower-triangular (L), not "R" in QR
  // decomp. Very confusing.
  armaMat L;
  armaMat Qt;
  lq(L, Qt, H);

  // van Overschee zeros out the other elements.
  L = trimatl(L);

  // submats that will be needed:
  auto R_14_14 = L.submat(0, 0, nH * (2 * nU + nY) - 1, nH * (2 * nU + nY) - 1);
  auto R_11_14 = L.submat(0, 0, nH * nU - 1, nH * (2 * nU + nY) - 1);
  auto R_11_13 = L.submat(0, 0, nH * nU - 1, nH * (2 * nU) - 1);
  auto R_23_13 = L.submat(nH * nU, 0, 2 * nH * nU - 1, 2 * nH * nU - 1);
  auto R_44_14 =
      L.submat(2 * nU * nH, 0, nH * (2 * nU + nY) - 1, nH * (2 * nU + nY) - 1);
  auto R_44_13 =
      L.submat(2 * nU * nH, 0, nH * (2 * nU + nY) - 1, nH * (2 * nU) - 1);
  auto R_44 = L.submat(2 * nU * nH, 2 * nU * nH, nH * (2 * nU + nY) - 1,
                       nH * (2 * nU + nY) - 1);
  auto R_56_14 = L.submat(nH * (2 * nU + nY), 0, nH * (2 * nU + 2 * nY) - 1,
                          nH * (2 * nU + nY) - 1);

  armaMat Lup_Luf_Lyp = R_56_14 * pinv(R_14_14);
  auto Lup = Lup_Luf_Lyp.submat(0, 0, nH * nY - 1, nH * nU - 1);
  auto Luf = Lup_Luf_Lyp.submat(0, nH * nU, nH * nY - 1, 2 * nH * nU - 1);
  auto Lyp =
      Lup_Luf_Lyp.submat(0, 2 * nH * nU, nH * nY - 1, nH * (2 * nU + nY) - 1);

  // more that will be needed later.
  auto R_15_15 =
      L.submat(0, 0, nH * (2 * nU + nY) + nY - 1, nH * (2 * nU + nY) + nY - 1);
  auto R_12_15 = L.submat(0, 0, nH * nU + nU - 1, nH * (2 * nU + nY) + nY - 1);
  auto R_23_15 =
      L.submat(nH * nU, 0, 2 * nH * nU - 1, nH * (2 * nU + nY) + nY - 1);
  auto R_23_16 = L.submat(nH * nU, 0, 2 * nH * nU - 1, 2 * nH * (nU + nY) - 1);
  auto R_45_15 = L.submat(2 * nU * nH, 0, nH * (2 * nU + nY) + nY - 1,
                          nH * (2 * nU + nY) + nY - 1);
  auto R_66_15 = L.submat(nH * (2 * nU + nY) + nY, 0, 2 * nH * (nU + nY) - 1,
                          nH * (2 * nU + nY) + nY - 1);
  auto R_55_15 = L.submat(nH * (2 * nU + nY), 0, nH * (2 * nU + nY) + nY - 1,
                          nH * (2 * nU + nY) + nY - 1);
  auto R_56_15 = L.submat(nH * (2 * nU + nY), 0, 2 * nH * (nU + nY) - 1,
                          nH * (2 * nU + nY) + nY - 1);

  // aka: Rf
  armaMat R_56_16 =
      L.submat(nH * (2 * nU + nY), 0, 2 * nH * (nU + nY) - 1, L.n_cols - 1);
  // from van Overschee subid.m:
  // Rf = R((2*m+l)*i+1:2*(m+l)*i,:); 	% Future outputs

  // wasteful, but for sanity put in same language as van overschee
  auto nCols = L.n_cols;
  armaMat Rf =
      L.submat(nH * (2 * nU + nY), 0, 2 * nH * (nU + nY) - 1, nCols - 1);
  armaMat Rp =
      join_vert(L.submat(0, 0, nH * nU - 1, nCols - 1),
                L.submat(2 * nH * nU, 0, nH * (2 * nU + nY) - 1, nCols - 1));
  armaMat Ru = L.submat(nH * nU, 0, 2 * nH * nU - 1, 2 * nH * nU - 1);

  // Perpendicular future outputs
  auto Rf_subL = Rf.submat(0, 0, Rf.n_rows - 1, Ru.n_cols - 1);
  auto Rf_subR = Rf.submat(0, Ru.n_cols, Rf.n_rows - 1, Rf.n_cols - 1);
  armaMat Rfp = join_horiz(Rf_subL - (Rf_subL * pinv(Ru)) * Ru, Rf_subR);

  auto Rp_subL = Rp.submat(0, 0, Rp.n_rows - 1, Ru.n_cols - 1);
  auto Rp_subR = Rp.submat(0, Ru.n_cols, Rp.n_rows - 1, Rp.n_cols - 1);
  armaMat Rpp = join_horiz(Rp_subL - (Rp_subL * pinv(Ru)) * Ru, Rp_subR);

  armaMat U;
  armaMat V;
  switch (wt) {
    case NONE: {
      // No weighting. (what van Overschee calls "N4SID")
      armaMat O_k_sans_Qt = Lup * R_11_14 + Lyp * R_44_14;
      arma::svd(U, s, V, O_k_sans_Qt);
    } break;
    case MOESP: {
      // MOESP weighting
      // This is what they use in the "robust" algorithm van Overschee 1996
      armaMat Ob = (Rfp * pinv(Rpp)) * Rp;  // good
      auto Ob_subL = Ob.submat(0, 0, Ob.n_rows - 1, Ru.n_cols - 1);
      auto Ob_subR = Ob.submat(0, Ru.n_cols, Ob.n_rows - 1, Ob.n_cols - 1);
      cout << "about to do perpendicular to future outputs..." << endl;
      armaMat O_k_ortho_Uf_sans_Qt =
          join_horiz(Ob_subL - (Ob_subL * pinv(Ru)) * Ru, Ob_subR);
      svd_econ(U, s, V, O_k_ortho_Uf_sans_Qt, "left", "std");
    } break;
    case CVA: {
      // CVA weighting
      // See van Overschee's matlab code (subid.m):
      // https://www.mathworks.com/matlabcentral/fileexchange/2290-subspace-identification-for-linear-systems
      armaMat Ob = (Rfp * pinv(Rpp)) * Rp;
      auto Ob_subL = Ob.submat(0, 0, Ob.n_rows - 1, Ru.n_cols - 1);
      auto Ob_subR = Ob.submat(0, Ru.n_cols, Ob.n_rows - 1, Ob.n_cols - 1);
      armaMat O_k_ortho_Uf_sans_Qt =
          join_horiz(Ob_subL - (Ob_subL * pinv(Ru)) * Ru, Ob_subR);

      armaMat inv_W1;
      armaMat Qt1;
      lq(inv_W1, Qt1, R_56_16);  // lq decomp of R_f (future output data)
      inv_W1 = trimatl(inv_W1);
      inv_W1 = inv_W1.submat(0, 0, nY * nH - 1, nY * nH - 1);

      armaMat WOW = solve(
          inv_W1, O_k_ortho_Uf_sans_Qt);  // alternatively
                                          // pinv(inv_W1)*O_k_ortho_Uf_sans_Qt
      svd(U, s, V, WOW);

      U = inv_W1 * U;
      break;
    }
  }

  // Truncate to model order (heart of SSID method)
  auto sHat = s.subvec(0, nX - 1);
  armaMat sqrtS = diagmat(sqrt(sHat));
  auto Uhat = U.submat(0, 0, U.n_rows - 1, nX - 1);

  // get extended observability and controllability mats
  armaMat extObs_k = Uhat * sqrtS;  // extended observability matrix
  auto extObs_km1 =
      extObs_k.submat(0, 0, extObs_k.n_rows - 1 - nY, extObs_k.n_cols - 1);

  // solve for params using appropriate algorithm:
  // "robust" deterministic/stochastic algorithm in van Overschee 1996
  // Use the algorithm that the authors say "works" in practice.

  // n.b., This is what textbook (1996) says:
  //
  // armaMat Tr = join_vert(pinv(extObs_k) * R_56_15, R_23_15);
  //
  // HOWEVER, do not know why, but have to fill last place with zeros like the
  // authors do in their Matlab code! (see `subid.m` van Overschee code)
  // Otherwise, get ridiculous covariances (although A,C estimates are close to
  // same...)
  armaMat Tr = join_vert(
      join_horiz(pinv(extObs_k) * R_56_14, armaMat(nX, nY, fill::zeros)),
      R_23_15);
  armaMat Tl = join_vert(pinv(extObs_km1) * R_66_15, R_55_15);
  armaMat S = Tl * pinv(Tr);

  // vanOverschee subid.m does this now:
  armaMat err = Tl - S * Tr;

  A = S.submat(0, 0, nX - 1, nX - 1);
  C = S.submat(nX, 0, nX + nY - 1, nX - 1);

  // At this point, van Overschee & de Moor suggest re-calculating extObs_k,
  // extObs_km1 from newly estimated (A, C) because it was just an
  // approximation.
  recompute_extObs(extObs_k, A, C, nY, nH);
  extObs_km1 =
      extObs_k.submat(0, 0, extObs_k.n_rows - 1 - nY,
                      extObs_k.n_cols - 1);  // extended observability matrix
  Tl = join_vert(pinv(extObs_km1) * R_66_15, R_55_15);
  // Tr = join_vert(pinv(extObs_k) * R_56_15, R_23_15);
  Tr = join_vert(
      join_horiz(pinv(extObs_k) * R_56_14, armaMat(nX, nY, fill::zeros)),
      R_23_15);
  S = Tl * pinv(Tr);

  armaMat Lcurly = S.submat(0, 0, nX + nY - 1, nX - 1) * pinv(extObs_k);
  armaMat Mcurly = pinv(extObs_km1);
  armaMat Pcurly = Tl - Lcurly * R_56_15;
  armaVec Pvec = vectorise(Pcurly);
  armaMat Qcurly = R_23_15;

  // Identify [D; B], assuming D=0 and ensuring DC gain is correct
  armaMat sum_QcurlyT_kron_Ncurly((nH * (2 * nU + nY) + nY) * (nY + nX),
                                  nU * (nY + nX), fill::zeros);

  armaMat eye_extObs_km1(nY + extObs_km1.n_rows, nY + extObs_km1.n_cols,
                         fill::eye);
  eye_extObs_km1.submat(nY, nY, eye_extObs_km1.n_rows - 1,
                        eye_extObs_km1.n_cols - 1) = extObs_km1;

  // van Overschee 1996 p. 126
  armaMat N1_Tl = -Lcurly;
  N1_Tl.submat(0, 0, nX - 1, N1_Tl.n_cols - 1) +=
      join_horiz(armaMat(nX, nY, fill::zeros), Mcurly);
  N1_Tl.submat(nX, 0, nX + nY - 1, nY - 1) += armaMat(nY, nY, fill::eye);

  armaMat Nk_Tl(N1_Tl.n_rows, N1_Tl.n_cols, fill::zeros);
  armaMat N_k;
  for (size_t k = 0; k < nH; k++) {
    auto Qcurly_k =
        Qcurly.submat(nU * k, 0, nU * (k + 1) - 1, Qcurly.n_cols - 1);

    Nk_Tl.zeros();
    Nk_Tl.submat(0, 0, nX + nY - 1, Nk_Tl.n_cols - k * nY - 1) =
        N1_Tl.submat(0, k * nY, N1_Tl.n_rows - 1, N1_Tl.n_cols - 1);
    N_k = Nk_Tl * eye_extObs_km1;

    sum_QcurlyT_kron_Ncurly += kron(Qcurly_k.t(), N_k);
  }

  // default way: *no* constraint on G0 or feedthrough (D=0)
  // (cf my tweaked ssid implementation)
  armaVec DBvec = pinv(sum_QcurlyT_kron_Ncurly) * Pvec;
  D = armaMat(DBvec.memptr(), nY, nU);
  B = armaMat(DBvec.memptr() + (nU * nY), nX, nU);

  armaMat covErr = err * err.t();
  Q = covErr.submat(0, 0, nX - 1, nX - 1);
  R = covErr.submat(nX, nX, nX + nY - 1, nX + nY - 1);
}

void lds::gaussian::ssid_n4sid(size_t& nX, size_t& nU, size_t& nY, size_t& nH,
                               armaMat& uSSID, armaMat& zSSID, armaMat& A,
                               armaMat& B, armaMat& C, armaMat& D, armaMat& Q,
                               armaMat& R, armaVec& m, armaVec& d, armaVec& s,
                               bool force_unitNormC, ssidWt wt, data_t wtG0) {
  // van Overschee, de Moor (1996)
  // create hankel mat
  armaMat H;  // data matrix [Up; Uf; Yp; Yf]
  createHankelMat_n4sid(uSSID, zSSID, nH, H);

  size_t N = H.n_cols;

  // do LQ decomp
  armaMat L;
  armaMat Qt;
  lq(L, Qt, H);

  // van Overschee zeros out the other elements.
  L = trimatl(L);

  // submats that will be needed:
  auto R_14_14 = L.submat(0, 0, nH * (2 * nU + nY) - 1, nH * (2 * nU + nY) - 1);
  auto R_11_14 = L.submat(0, 0, nH * nU - 1, nH * (2 * nU + nY) - 1);
  auto R_11_13 = L.submat(0, 0, nH * nU - 1, nH * (2 * nU) - 1);
  auto R_23_13 = L.submat(nH * nU, 0, 2 * nH * nU - 1, 2 * nH * nU - 1);
  auto R_44_14 =
      L.submat(2 * nU * nH, 0, nH * (2 * nU + nY) - 1, nH * (2 * nU + nY) - 1);
  auto R_44_13 =
      L.submat(2 * nU * nH, 0, nH * (2 * nU + nY) - 1, nH * (2 * nU) - 1);
  auto R_44 = L.submat(2 * nU * nH, 2 * nU * nH, nH * (2 * nU + nY) - 1,
                       nH * (2 * nU + nY) - 1);
  auto R_56_14 = L.submat(nH * (2 * nU + nY), 0, nH * (2 * nU + 2 * nY) - 1,
                          nH * (2 * nU + nY) - 1);

  armaMat Lup_Luf_Lyp = R_56_14 * pinv(R_14_14);
  auto Lup = Lup_Luf_Lyp.submat(0, 0, nH * nY - 1, nH * nU - 1);
  auto Luf = Lup_Luf_Lyp.submat(0, nH * nU, nH * nY - 1, 2 * nH * nU - 1);
  auto Lyp =
      Lup_Luf_Lyp.submat(0, 2 * nH * nU, nH * nY - 1, nH * (2 * nU + nY) - 1);

  // more that will be needed later.
  auto R_15_15 =
      L.submat(0, 0, nH * (2 * nU + nY) + nY - 1, nH * (2 * nU + nY) + nY - 1);
  auto R_12_15 = L.submat(0, 0, nH * nU + nU - 1, nH * (2 * nU + nY) + nY - 1);
  auto R_23_15 =
      L.submat(nH * nU, 0, 2 * nH * nU - 1, nH * (2 * nU + nY) + nY - 1);
  auto R_23_16 = L.submat(nH * nU, 0, 2 * nH * nU - 1, 2 * nH * (nU + nY) - 1);
  auto R_45_15 = L.submat(2 * nU * nH, 0, nH * (2 * nU + nY) + nY - 1,
                          nH * (2 * nU + nY) + nY - 1);
  auto R_66_15 = L.submat(nH * (2 * nU + nY) + nY, 0, 2 * nH * (nU + nY) - 1,
                          nH * (2 * nU + nY) + nY - 1);
  auto R_55_15 = L.submat(nH * (2 * nU + nY), 0, nH * (2 * nU + nY) + nY - 1,
                          nH * (2 * nU + nY) + nY - 1);
  auto R_56_15 = L.submat(nH * (2 * nU + nY), 0, 2 * nH * (nU + nY) - 1,
                          nH * (2 * nU + nY) + nY - 1);

  // aka: R_f
  armaMat R_56_16 =
      L.submat(nH * (2 * nU + nY), 0, 2 * nH * (nU + nY) - 1, L.n_cols - 1);
  // from van Overschee subid.m:
  // Rf = R((2*m+l)*i+1:2*(m+l)*i,:); 	% Future outputs

  armaMat U;
  armaMat V;
  switch (wt) {
    case NONE: {
      // No weighting. (what van Overschee calls "N4SID")
      armaMat O_k_sans_Qt = Lup * R_11_14 + Lyp * R_44_14;
      arma::svd(U, s, V, O_k_sans_Qt);
    } break;
    case MOESP: {
      // MOESP weighting
      // This is what they use in the "robust" algorithm van Overschee, de Moor
      // 1996
      armaMat Pi = armaMat(2 * nH * nU, 2 * nH * nU, fill::eye) -
                   R_23_13.t() * inv(R_23_13 * R_23_13.t()) * R_23_13;
      armaMat O_k_ortho_Uf_sans_Qt =
          join_horiz((Lup * R_11_13 + Lyp * R_44_13) * Pi, Lyp * R_44);
      svd(U, s, V, O_k_ortho_Uf_sans_Qt);
    } break;
    case CVA: {
      // CVA weighting
      // See van Overschee's matlab code (subid.m):
      // https://www.mathworks.com/matlabcentral/fileexchange/2290-subspace-identification-for-linear-systems
      armaMat Pi = armaMat(2 * nH * nU, 2 * nH * nU, fill::eye) -
                   R_23_13.t() * inv(R_23_13 * R_23_13.t()) * R_23_13;
      armaMat O_k_ortho_Uf_sans_Qt =
          join_horiz((Lup * R_11_13 + Lyp * R_44_13) * Pi, Lyp * R_44);

      armaMat inv_W1;
      armaMat Qt1;
      lq(inv_W1, Qt1, R_56_16);  // lq decomp of R_f (future output data)
      inv_W1 = trimatl(inv_W1);
      inv_W1 = inv_W1.submat(0, 0, nY * nH - 1, nY * nH - 1);
      armaMat WOW = solve(
          inv_W1, O_k_ortho_Uf_sans_Qt);  // alternatively
                                          // pinv(inv_W1)*O_k_ortho_Uf_sans_Qt
      svd(U, s, V, WOW);

      U = inv_W1 * U;
      break;
    }
  }

  // Truncate to model order (heart of ssid method)
  auto sHat = s.subvec(0, nX - 1);
  armaMat sqrtS = diagmat(sqrt(sHat));
  auto Uhat = U.submat(0, 0, U.n_rows - 1, nX - 1);

  // get extended observability and controllability mats
  armaMat extObs_k = Uhat * sqrtS;  // extended observability matrix
  auto extObs_km1 =
      extObs_k.submat(0, 0, extObs_k.n_rows - 1 - nY,
                      extObs_k.n_cols - 1);  // extended observability matrix

  // solve for params using appropriate algorithm:
  // robust deterministic/stochastic algorithm in van Overschee 1996
  // algorithm that the authors say "works" in practice.

  // This is what textbook (1996) says:
  //
  // armaMat Tr = join_vert(pinv(extObs_k) * R_56_15, R_23_15);
  //
  // HOWEVER, do not know why but have to fill the last place with zeros like
  // authors' matlab implementation (see `subid.m`)
  // Otherwise, get ridiculous covariances (although A,C estimates are close to
  // same...)
  armaMat Tr = join_vert(
      join_horiz(pinv(extObs_k) * R_56_14, armaMat(nX, nY, fill::zeros)),
      R_23_15);
  armaMat Tl = join_vert(pinv(extObs_km1) * R_66_15, R_55_15);
  armaMat S = Tl * pinv(Tr);

  // Use alternative in van Overschee 1996, p. 129. Apparently, should ensure
  // stability.
  C = extObs_k.submat(0, 0, nY - 1, extObs_k.n_cols - 1);
  armaMat extObs_kp1 = join_vert(
      extObs_k.submat(nY, 0, extObs_k.n_rows - 1, extObs_k.n_cols - 1),
      armaMat(nY, extObs_k.n_cols, fill::zeros));
  A = pinv(extObs_k) * extObs_kp1;

  if (force_unitNormC) {
    C = C / arma::norm(C);

    // If nX=nY=1, forces y = x + noise
    if ((nX == 1) & (nX == nY)) {
      C = C[0] * C;  // in case C = -1 after C/||C||
    }
  }

  // At this point, van Overschee & de Moor suggest re-calculating extObs_k,
  // extObs_km1 from (A, C) because it was just an approximation. This is
  // *especially* important if I have messed with C (above)
  recompute_extObs(extObs_k, A, C, nY, nH);
  extObs_km1 =
      extObs_k.submat(0, 0, extObs_k.n_rows - 1 - nY,
                      extObs_k.n_cols - 1);  // extended observability matrix
  Tl = join_vert(pinv(extObs_km1) * R_66_15, R_55_15);
  Tr = join_vert(
      join_horiz(pinv(extObs_k) * R_56_14, armaMat(nX, nY, fill::zeros)),
      R_23_15);
  S = Tl * pinv(Tr);

  armaMat Lcurly = S.submat(0, 0, nX + nY - 1, nX - 1) * pinv(extObs_k);
  armaMat Mcurly = pinv(extObs_km1);
  armaMat Pcurly = Tl - Lcurly * R_56_15;
  armaVec Pvec = vectorise(Pcurly);
  armaMat Qcurly = R_23_15;

  // Identify [D; B], assuming D=0 and ensuring DC gain is correct
  armaMat sum_QcurlyT_kron_Ncurly((nH * (2 * nU + nY) + nY) * (nY + nX),
                                  nU * (nY + nX), fill::zeros);

  armaMat eye_extObs_km1(nY + extObs_km1.n_rows, nY + extObs_km1.n_cols,
                         fill::eye);
  eye_extObs_km1.submat(nY, nY, eye_extObs_km1.n_rows - 1,
                        eye_extObs_km1.n_cols - 1) = extObs_km1;

  // van Overschee p. 126
  armaMat N1_Tl = -Lcurly;
  N1_Tl.submat(0, 0, nX - 1, N1_Tl.n_cols - 1) +=
      join_horiz(armaMat(nX, nY, fill::zeros), Mcurly);
  N1_Tl.submat(nX, 0, nX + nY - 1, nY - 1) += armaMat(nY, nY, fill::eye);

  armaMat Nk_Tl(N1_Tl.n_rows, N1_Tl.n_cols, fill::zeros);
  armaMat N_k;
  for (size_t k = 0; k < nH; k++) {
    auto Qcurly_k =
        Qcurly.submat(nU * k, 0, nU * (k + 1) - 1, Qcurly.n_cols - 1);

    Nk_Tl.zeros();
    Nk_Tl.submat(0, 0, nX + nY - 1, Nk_Tl.n_cols - k * nY - 1) =
        N1_Tl.submat(0, k * nY, N1_Tl.n_rows - 1, N1_Tl.n_cols - 1);
    N_k = Nk_Tl * eye_extObs_km1;

    sum_QcurlyT_kron_Ncurly += kron(Qcurly_k.t(), N_k);
  }

  const bool doConstrainDB = true;  // TODO(mfbolus): expose this as option?
  armaMat errVec;
  if (doConstrainDB) {
    // Constraints enforced by weighted least squares
    //
    // Reference:
    //
    // Privara S, ..., Ferkl L. (2010) Subspace Identification of Poorly Excited
    // Industrial Systems. Conference in Decision and Control.

    // constraint 1: assume D=0 --> remove the components for Dvec
    // (this is actually a hard constraint in that it ignores D)
    armaMat sum_QcurlyT_kron_Ncurly_db = sum_QcurlyT_kron_Ncurly;
    sum_QcurlyT_kron_Ncurly =
        armaMat(sum_QcurlyT_kron_Ncurly_db.n_rows, nX * nU);

    size_t kkk = 0;
    for (size_t k = 1; k < (nU + 1); k++) {
      size_t startIdx = k * (nY + nX) - nX;
      for (size_t kk = 0; kk < nX; kk++) {
        sum_QcurlyT_kron_Ncurly.col(kkk) =
            sum_QcurlyT_kron_Ncurly_db.col(startIdx + kk);
        kkk++;
      }
    }

    // constraint 2: Make sure DC I/O gain is correct
    // For some reason, IO gain was not always right.
    armaMat G0 = zSSID * pinv(uSSID);
    // cout << "G0 = \n" << G0 << endl;

    armaMat b_to_g0 = C * inv(armaMat(nX, nX, fill::eye) - A);
    armaMat Pvec_Gvec = join_vert(Pvec, vectorise(G0));
    armaMat eye_kron_b_to_g0 = kron(armaMat(nU, nU, fill::eye), b_to_g0);
    armaMat sum_QcurlyT_kron_Ncurly_b_to_g0 =
        join_vert(sum_QcurlyT_kron_Ncurly, eye_kron_b_to_g0);

    // WEIGHTED LS
    // Important in practice because I care a lot about at least getting the DC
    // gain correct. Put x weight on minimizing error at DC, relative to others
    armaMat W(sum_QcurlyT_kron_Ncurly_b_to_g0.n_rows,
              sum_QcurlyT_kron_Ncurly_b_to_g0.n_rows, fill::eye);
    // Make weight on minimizing DC error immense so at least that
    // should be nailed.
    size_t startRow = sum_QcurlyT_kron_Ncurly.n_rows;
    size_t startCol = sum_QcurlyT_kron_Ncurly.n_rows;
    size_t stopRow = W.n_rows - 1;
    size_t stopCol = W.n_cols - 1;
    // W.submat(startRow, startCol, stopRow, stopCol) *= wtG0*N;// scale weight
    // with data length?
    W.submat(startRow, startCol, stopRow, stopCol) *= wtG0;
    armaVec Bvec = inv(sum_QcurlyT_kron_Ncurly_b_to_g0.t() * W *
                       sum_QcurlyT_kron_Ncurly_b_to_g0) *
                   sum_QcurlyT_kron_Ncurly_b_to_g0.t() * W * Pvec_Gvec;

    B = armaMat(Bvec.memptr(), nX, nU);
    D = armaMat(nY, nU, fill::zeros);

    // Calculate residuals and their cov.
    // Because I've added constraints, I need to re-calculate the right term
    // with Bvec instead of how van Overschee do in final algorithm.
    errVec = Pvec - sum_QcurlyT_kron_Ncurly * Bvec;
  } else {
    // default way: *no* constraint on G0 or D=0
    armaVec DBvec = pinv(sum_QcurlyT_kron_Ncurly) * Pvec;
    D = armaMat(DBvec.memptr(), nY, nU);
    B = armaMat(DBvec.memptr() + (nU * nY), nX, nU);
    errVec = Pvec - sum_QcurlyT_kron_Ncurly * DBvec;
  }

  // armaMat err = armaMat(errVec.memptr(), Pcurly.n_rows, Pcurly.n_cols);

  // TODO(mfbolus): Something is wrong with the error calculation above.
  // Use the way van overschee does it in `subid.m`
  // WARNING: this ignores any above constraints, so Q, R will be approximate...
  armaMat err = Tl - S * Tr;
  armaMat covErr = err * err.t();
  Q = covErr.submat(0, 0, nX - 1, nX - 1);
  R = covErr.submat(nX, nX, nX + nY - 1, nX + nY - 1);
}

void lds::gaussian::recompute_extObs(armaMat& extObs, armaMat& A, armaMat& C,
                                     size_t& nY, size_t& nH) {
  extObs.submat(0, 0, nY - 1, extObs.n_cols - 1) = C;
  for (size_t k = 2; k < (nH + 1); k++) {
    extObs.submat((k - 1) * nY, 0, k * nY - 1, extObs.n_cols - 1) =
        extObs.submat((k - 2) * nY, 0, (k - 1) * nY - 1, extObs.n_cols - 1) * A;
  }
}

void lds::gaussian::lq(armaMat& L, armaMat& Qt, armaMat& X) {
  arma::qr_econ(Qt, L, X.t());
  arma::inplace_trans(L);
  arma::inplace_trans(Qt);
}

armaMat lds::gaussian::calcCov(armaMat& A, armaMat& B) {
  // subtract out mean
  auto mA = mean(A, 1);
  armaMat A0 = A;
  A0.each_col() -= mA;

  auto mB = mean(B, 1);
  armaMat B0 = B;
  B0.each_col() -= mB;

  armaMat cov = A0 * B0.t() / A0.n_cols;
  return cov;
}

// approximate matrix sqrt by svd --> U * sqrt(s) * V'
armaMat lds::gaussian::sqrtmat_svd(armaMat& X) {
  armaMat U;
  armaVec s;
  armaMat V;

  arma::svd(U, s, V, X);

  armaMat sqrtX = U * diagmat(sqrt(s)) * V.t();
  return sqrtX;
}
