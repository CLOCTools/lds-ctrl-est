//===-- lds_poisson_fit_ssid.cpp - PLDS Fit (SSID) ------------------------===//
//
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
/// This file implements a type by which Poisson-output LDS models are fit by a
/// subspace identification (SSID) algorithm (`lds::gaussian::FitSSID`).
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

namespace lds {
namespace poisson {

void FitSSID::PoissonToGaussianMoments() {
  // for numerical reasons, Buesing 2012 adds small number to diagonal
  cov_.diag() += 1e-5;

  // get the across-time mean
  Vector mu_y(n_y_);
  for (size_t trial = 0; trial < n_trials_; trial++) {
    mu_y += arma::mean(z_.at(trial), 1);
  }
  mu_y /= n_trials_;

  // make sure average rates are greater than zero
  bool is_zero = false;
  for (size_t k = 0; k < n_y_; k++) {
    is_zero = mu_y[k] <= 0;
  }
  if (is_zero) {
    throw std::runtime_error(
        "One or more output channels have zero event rates. Consider removing "
        "those dimensions.");
  }
  // will need this below:
  Matrix mu_yy = arma::repmat(mu_y, 2 * n_h_, 1);

  // needed submats:
  // input covariances
  auto cov_uu = cov_.submat(0, 0, 2 * n_h_ * n_u_ - 1, 2 * n_h_ * n_u_ - 1);
  // output covariances
  auto cov_yy =
      cov_.submat(2 * n_h_ * n_u_, 2 * n_h_ * n_u_,
                  2 * n_h_ * (n_u_ + n_y_) - 1, 2 * n_h_ * (n_u_ + n_y_) - 1);
  auto cov_yu = cov_.submat(2 * n_h_ * n_u_, 0, 2 * n_h_ * (n_u_ + n_y_) - 1,
                            2 * n_h_ * n_u_ - 1);
  auto cov_uy = cov_.submat(0, 2 * n_h_ * n_u_, 2 * n_h_ * n_u_ - 1,
                            2 * n_h_ * (n_u_ + n_y_) - 1);

  // Convert from poisson to gaussian moments:
  // First, the output-only pieces of cov.
  // see CoreSSID/PoissonMomentsToGaussMoments.m

  // Buesing makes sure minimum Fano factor at least 1.02 by default
  // TODO(mfbolus): make this a user option?
  const data_t ff_min = 1.01;  // 1.01;
  Vector ff = cov_yy.diag() / mu_yy;
  Vector ff_upscale_vec = ff_min / ff;
  Limit(ff_upscale_vec, 1.0, kInf);
  Matrix ff_upscale_mat(cov_yy.n_rows, cov_yy.n_cols, fill::ones);
  ff_upscale_mat.each_col() %= arma::sqrt(ff_upscale_vec);
  cov_yy = cov_yy % ff_upscale_mat % ff_upscale_mat.t();

  // Buesing makes minimums second moment 1e-3 (for short datasets)
  // Alternatively, Buesing sets `minMoment` to 5/allT.
  // TODO(mfbolus): make this a user option?
  const data_t m_min = 1 / static_cast<data_t>(D_.n_cols);
  // second moments
  Matrix m(cov_yy.n_rows, cov_yy.n_cols, fill::zeros);
  // take care of diagonel elements (variances)
  auto m_diag = m.diag();
  Vector alpha(cov_yy.n_rows, fill::zeros);
  Vector beta(cov_yy.n_rows, fill::zeros);
  for (size_t k = 0; k < cov_yy.n_rows; k++) {
    data_t tmp = cov_yy(k, k) + pow(mu_yy[k], 2);
    m_diag[k] = tmp < m_min ? m_min : tmp;
    alpha[k] = pow(mu_yy[k], 2) / sqrt(m_diag[k] - mu_yy[k]);
    beta[k] = sqrt(m_diag[k] - mu_yy[k]) / mu_yy[k];
  }
  Vector mu_gauss_yy = arma::log(alpha);
  cov_yy.diag() = 2 * arma::log(beta);

  // now cross terms
  for (size_t k = 0; k < cov_yy.n_rows; k++) {
    data_t tmp = 0;
    for (size_t kk = k + 1; kk < cov_yy.n_cols; kk++) {
      if (cov_yy(k, kk) != 0) {
        tmp = cov_yy(k, kk) + mu_yy[k] * mu_yy[kk];
        m(k, kk) = tmp < m_min ? m_min : tmp;
        cov_yy(kk, k) =
            (2 * (log(m(k, kk)) - mu_gauss_yy[k] - mu_gauss_yy[kk]) -
             cov_yy(k, k) - cov_yy(kk, kk)) /
            2;
        cov_yy(k, kk) = cov_yy(kk, k);
      }
    }  // kk
  }    // k

  // the "mixed" input/output crossterms:
  // see CoreSSID/PoissonMomentsToGaussMomentsMixed.m
  // TODO(mfbolus): not sure if indexing is correct here.
  // Buesing used (k < 2*n_h_*n_u_) here
  for (size_t k = 0; k < cov_yu.n_rows; k++) {
    cov_uy.col(k) *= exp(-0.5 * cov_yy(k, k) * mu_gauss_yy(k));
    cov_yu.row(k) = cov_uy.col(k).t();
  }

  // Buesing makes sure transformed cov_yy has a min eigen value
  ForceSymMinEig(cov_, 1e-4);
}

void FitSSID::CalcCov() {
  // TODO(mfbolus): more efficient way to do this.
  // Calculate covariance of the data matrix
  cov_ = D_ * D_.t();

  // retro-actively do mean subtraction
  Vector mu_d = arma::mean(D_, 1);
  cov_ -= mu_d * mu_d.t() * D_.n_cols;

  // // calculate I/O gain @ DC while data in convenient form
  // Matrix u_tmp = D_.submat(0, 0, n_u_ - 1, D_.n_cols - 1);
  // Matrix z_tmp = D_.submat(2 * n_h_ * n_u_, 0,
  //                          2 * n_h_ * n_u_ + n_y_ - 1, D_.n_cols - 1);
  // g_dc_ = arma::log(z_tmp) * pinv(u_tmp);

  PoissonToGaussianMoments();
}

void FitSSID::DecomposeData() {
  // calculate covariance and perform Poisson -> Gaussian conversion
  CalcCov();
  // Buesing CoreSSID/ssidN4SIDsmall.m:
  // "Cholesky decomposition of SIG, corresponds to QR of data matrix"
  L_ = arma::chol(cov_, "lower");
  // van Overschee zeros out the other elements.
  L_ = trimatl(L_);
}
}  // namespace poisson
}  // namespace lds
