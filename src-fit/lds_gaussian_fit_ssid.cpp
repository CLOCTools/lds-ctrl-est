//===-- lds_gaussian_fit_ssid.cpp - GLDS Fit (SSID) -----------------------===//
//
// Copyright 2021 Michael Bolus
// Copyright 2021 Georgia Institute of Technology
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
/// This file partially defines a type by which Gaussian-output LDS models are
/// fit by a subspace identification (SSID) algorithm
/// (`lds::gaussian::FitSSID`).
///
/// References:
/// [1] van Overschee P, de Moore B. (1996) Subspace Identification for Linear
/// Systems. Boston: Springer.
///
/// \brief GLDS SSID fit type
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst_h/lds_gaussian_fit_ssid.h>

namespace lds {
namespace gaussian {
void FitSSID::DecomposeData() {
  // // do LQ decomp
  // // Note that "R" in van Overschee is lower-triangular (L), not "R" in QR
  // // decomp. Very confusing.
  // Matrix q_t;
  // lq(L_, q_t, D_);
  // // van Overschee zeros out the other elements.
  // L_ = trimatl(L_);

  // Depending on dataset, this may be faster:
  // Buesing CoreSSID/ssidN4SIDsmall.m:
  // "Cholesky decomposition of SIG, corresponds to QR of data matrix"
  Matrix cov = arma::cov(D_.t(), 1) * D_.n_cols;  // undo normalization
  ForceSymPD(cov);
  L_ = arma::chol(cov, "lower");
}

void FitSSID::SolveVanOverschee() {
  // van Overschee, de Moor (1996)
  // van Overschee, de Moor `subid.m` re-implementation
  // https://www.mathworks.com/matlabcentral/fileexchange/2290-subspace-identification-for-linear-systems
  // (matlab scripts accompanying book)

  auto ext_obs_tm1 = ext_obs_t_.submat(0, 0, ext_obs_t_.n_rows - 1 - n_y_,
                                       ext_obs_t_.n_cols - 1);

  // required submats
  auto R_56_14 =
      L_.submat(n_h_ * (2 * n_u_ + n_y_), 0, n_h_ * (2 * n_u_ + 2 * n_y_) - 1,
                n_h_ * (2 * n_u_ + n_y_) - 1);
  auto R_23_15 = L_.submat(n_h_ * n_u_, 0, 2 * n_h_ * n_u_ - 1,
                           n_h_ * (2 * n_u_ + n_y_) + n_y_ - 1);
  auto R_66_15 = L_.submat(n_h_ * (2 * n_u_ + n_y_) + n_y_, 0,
                           2 * n_h_ * (n_u_ + n_y_) - 1,
                           n_h_ * (2 * n_u_ + n_y_) + n_y_ - 1);
  auto R_55_15 = L_.submat(n_h_ * (2 * n_u_ + n_y_), 0,
                           n_h_ * (2 * n_u_ + n_y_) + n_y_ - 1,
                           n_h_ * (2 * n_u_ + n_y_) + n_y_ - 1);
  auto R_56_15 =
      L_.submat(n_h_ * (2 * n_u_ + n_y_), 0, 2 * n_h_ * (n_u_ + n_y_) - 1,
                n_h_ * (2 * n_u_ + n_y_) + n_y_ - 1);

  // Solve for params using appropriate algorithm:
  // "robust" deterministic/stochastic algorithm in van Overschee 1996
  // Use the algorithm that the authors say "works" in practice.

  // n.b., This is what textbook (1996) says:
  //
  // Matrix Tr = join_vert(pinv(ext_obs_t_) * R_56_15, R_23_15);
  //
  // HOWEVER, do not know why, but have to fill last place with zeros like the
  // authors do in their Matlab code! (see `subid.m` van Overschee code)
  // Otherwise, get ridiculous covariances (although A,C estimates are close to
  // same...)
  Matrix Tr = join_vert(
      join_horiz(pinv(ext_obs_t_) * R_56_14, Matrix(n_x_, n_y_, fill::zeros)),
      R_23_15);
  Matrix Tl = join_vert(pinv(ext_obs_tm1) * R_66_15, R_55_15);
  Matrix S = Tl * pinv(Tr);

  // vanOverschee subid.m does this now:
  Matrix err = Tl - S * Tr;

  fit_.set_A(S.submat(0, 0, n_x_ - 1, n_x_ - 1));
  fit_.set_C(S.submat(n_x_, 0, n_x_ + n_y_ - 1, n_x_ - 1));

  // At this point, van Overschee & de Moor suggest re-calculating ext_obs_t_,
  // ext_obs_tm1 from newly estimated (A, C) because it was just an
  // approximation.
  RecomputeExtObs();
  ext_obs_tm1 = ext_obs_t_.submat(
      0, 0, ext_obs_t_.n_rows - 1 - n_y_,
      ext_obs_t_.n_cols - 1);  // extended observability matrix
  Tl = join_vert(pinv(ext_obs_tm1) * R_66_15, R_55_15);
  // Tr = join_vert(pinv(ext_obs_t_) * R_56_15, R_23_15);
  Tr = join_vert(
      join_horiz(pinv(ext_obs_t_) * R_56_14, Matrix(n_x_, n_y_, fill::zeros)),
      R_23_15);
  S = Tl * pinv(Tr);

  Matrix Lcurly = S.submat(0, 0, n_x_ + n_y_ - 1, n_x_ - 1) * pinv(ext_obs_t_);
  Matrix Mcurly = pinv(ext_obs_tm1);
  Matrix Pcurly = Tl - Lcurly * R_56_15;
  Vector Pvec = vectorise(Pcurly);
  Matrix Qcurly = R_23_15;

  // Identify [D; B]
  Matrix sum_QcurlyT_kron_Ncurly(
      (n_h_ * (2 * n_u_ + n_y_) + n_y_) * (n_y_ + n_x_), n_u_ * (n_y_ + n_x_),
      fill::zeros);

  Matrix eye_ext_obs_tm1(n_y_ + ext_obs_tm1.n_rows, n_y_ + ext_obs_tm1.n_cols,
                         fill::eye);
  eye_ext_obs_tm1.submat(n_y_, n_y_, eye_ext_obs_tm1.n_rows - 1,
                         eye_ext_obs_tm1.n_cols - 1) = ext_obs_tm1;

  // van Overschee 1996 p. 126
  Matrix N1_Tl = -Lcurly;
  N1_Tl.submat(0, 0, n_x_ - 1, N1_Tl.n_cols - 1) +=
      join_horiz(Matrix(n_x_, n_y_, fill::zeros), Mcurly);
  N1_Tl.submat(n_x_, 0, n_x_ + n_y_ - 1, n_y_ - 1) +=
      Matrix(n_y_, n_y_, fill::eye);

  Matrix Nk_Tl(N1_Tl.n_rows, N1_Tl.n_cols, fill::zeros);
  Matrix N_k;
  for (size_t k = 0; k < n_h_; k++) {
    auto Qcurly_k =
        Qcurly.submat(n_u_ * k, 0, n_u_ * (k + 1) - 1, Qcurly.n_cols - 1);

    Nk_Tl.zeros();
    Nk_Tl.submat(0, 0, n_x_ + n_y_ - 1, Nk_Tl.n_cols - k * n_y_ - 1) =
        N1_Tl.submat(0, k * n_y_, N1_Tl.n_rows - 1, N1_Tl.n_cols - 1);
    N_k = Nk_Tl * eye_ext_obs_tm1;

    sum_QcurlyT_kron_Ncurly += kron(Qcurly_k.t(), N_k);
  }

  // default way: *no* constraint on G0 or feedthrough (D=0)
  // (cf my tweaked ssid implementation)
  Vector db_vec = pinv(sum_QcurlyT_kron_Ncurly) * Pvec;
  // TODO(mfbolus): this gets thrown away:
  // Matrix D = Matrix(db_vec.memptr(), n_y_, n_u_);
  fit_.set_B(Matrix(db_vec.memptr() + (n_u_ * n_y_), n_x_, n_u_));

  Matrix cov_err = err * err.t();
  fit_.set_Q(cov_err.submat(0, 0, n_x_ - 1, n_x_ - 1));
  fit_.set_R(cov_err.submat(n_x_, n_x_, n_x_ + n_y_ - 1, n_x_ + n_y_ - 1));
}  // SolveVanOverschee

}  // namespace gaussian
}  // namespace lds
