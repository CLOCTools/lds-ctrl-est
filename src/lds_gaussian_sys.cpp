//===-- lds_gaussian_sys.cpp - GLDS ---------------------------------------===//
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
/// This file implements the type for state estimation (filtering) as well as
/// simulation of Gaussian-output linear dynamical systems
/// (`lds::gaussian::sys_t`). It inherits functionality from the underlying
/// linear dynamical system (`lds::sys_t`).
///
/// \brief GLDS base type
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>


lds::gaussian::System::System(size_t n_u, size_t n_x, size_t n_y, data_t dt, data_t p0,
                   data_t q0, data_t r0)
    : lds::System(n_u, n_x, n_y, dt, p0, q0) {

  R_.zeros(n_y, n_y);
  R_.diag().fill(r0);

  do_recurse_Ke_=true;
};

// recursively estimate Ke
void lds::gaussian::System::RecurseKe() {
  if (!do_recurse_Ke_) {
    return;
  }

  // predict covariance
  P_ = A_ * P_ * A_.t() + Q_;

  // calc Kalman gain
  Ke_ = P_ * C_.t() * inv_sympd(C_ * P_ * C_.t() + R_);

  // update covariance
  // Reference: Ghahramani et Hinton (1996)
  P_ = P_ - Ke_ * C_ * P_;

  if (do_adapt_m) {
    P_m_ += Q_m_;  // A_m = I (i.e., random walk)
    Ke_m_ = P_m_ * C_.t() * inv_sympd(C_ * P_m_ * C_.t() + R_);
    P_m_ = P_m_ - Ke_m_ * C_ * P_m_;
  }
}

// Simulate
const lds::Vector& lds::gaussian::System::Simulate(const Vector& u_tm1){
  f(u_tm1, true);//simulate dynamics with noise added
  h();//output
  z_ = y_ + arma::mvnrnd(Vector(n_y_).fill(0), R_);//measure
  return z_;
}

void lds::gaussian::System::Print() {
  lds::System::Print();
  std::cout << "R: \n" << R_ << "\n";
}
