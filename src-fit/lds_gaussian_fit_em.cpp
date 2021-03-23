//===-- lds_gaussian_fit_em.cpp - GLDS Fit (EM) ---------------------------===//
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

namespace lds {
namespace gaussian {

void FitEM::RecurseKe(Matrix& Ke, Cube& P_pre, Cube& P_post, size_t t) {
  // predict covar
  P_pre.slice(t) = fit_.A() * P_post.slice(t - 1) * fit_.A().t() + fit_.Q();

  // update Ke
  Ke = P_pre.slice(t) * fit_.C().t() *
       inv_sympd(fit_.C() * P_pre.slice(t) * fit_.C().t() + fit_.R());

  // update covar
  // Reference: Ghahramani et Hinton (1996)
  P_post.slice(t) = P_pre.slice(t) - Ke * fit_.C() * P_pre.slice(t);
}

void FitEM::MaximizeOutput() {
  // solve for C+d:
  Matrix sum_zx(n_y_, n_x_ + 1, fill::zeros);
  Vector x1(n_x_ + 1, fill::zeros);
  x1[n_x_] = 1.0;  // augment with one to solve for bias
  Matrix sum_e_x1_x1(n_x_ + 1, n_x_ + 1, fill::zeros);
  for (size_t trial = 0; trial < z_.size(); trial++) {
    for (size_t t = 1; t < n_t_[trial]; t++) {
      x1.subvec(0, n_x_ - 1) = x_[trial].col(t);
      sum_zx += z_.at(trial).col(t) * x1.t();
      sum_e_x1_x1 += x1 * x1.t();
      sum_e_x1_x1.submat(0, 0, n_x_ - 1, n_x_ - 1) += P_[trial].slice(t);
    }
  }
  Matrix cd = sum_zx * inv_sympd(sum_e_x1_x1);
  fit_.set_C(cd.submat(0, 0, n_y_ - 1, n_x_ - 1));
  fit_.set_d(vectorise(cd.submat(0, n_x_, n_y_ - 1, n_x_)));
  std::cout << "C_new[0]: " << fit_.C()[0] << "\n";
  std::cout << "d_new[0]: " << fit_.d()[0] << "\n";
}

void FitEM::MaximizeMeasurement() {
  // Solve for measurement noise covar
  size_t n_t_tot = 0;
  // Ghahgramani, Hinton 1996:
  Matrix sum_zz(n_y_, n_y_, fill::zeros);
  Matrix sum_yz(n_y_, n_y_, fill::zeros);
  for (size_t trial = 0; trial < z_.size(); trial++) {
    for (size_t t = 1; t < n_t_[trial]; t++) {
      sum_zz += z_.at(trial).col(t) * z_.at(trial).col(t).t();
      // Use Cnew:
      sum_yz +=
          (fit_.C() * x_[trial].col(t) + fit_.d()) * z_.at(trial).col(t).t();
      n_t_tot += 1;
    }
  }
  fit_.set_R((sum_zz - sum_yz) / n_t_tot);
  std::cout << "R_new[0]: " << fit_.R()[0] << "\n";
}

}  // namespace gaussian
}  // namespace lds
