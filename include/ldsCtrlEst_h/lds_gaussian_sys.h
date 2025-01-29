//===-- ldsCtrlEst_h/lds_gaussian_sys.h - GLDS ------------------*- C++ -*-===//
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
/// This file declares and partially defines the type for state estimation
/// (filtering) as well as simulation of Gaussian-output linear dynamical
/// systems
/// (`lds::gaussian::System`). It inherits functionality from the underlying
/// linear dynamical system (`lds::System`).
///
/// \brief GLDS base type
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_GAUSSIAN_SYS_H
#define LDSCTRLEST_LDS_GAUSSIAN_SYS_H

// namespace
#include "lds_gaussian.h"
// system
#include "lds_sys.h"

namespace lds {
namespace gaussian {
/// Gaussian LDS Type
class System : public lds::System {
 public:
  /**
   * @brief      Constructs a new System.
   */
  System() = default;

  /**
   * @brief      Constructs a new Gaussian System.
   *
   * @param      n_u    number of inputs (u)
   * @param      n_x    number of states (x)
   * @param      n_y    number of outputs (y)
   * @param      dt    sample period
   * @param      p0    [optional] initial diagonal elements of state estimate
   *                   covariance (P)
   * @param      q0    [optional] initial diagonal elements of process noise
   *                   covariance (Q)
   * @param      r0    [optional] initial diagonal elements of output noise
   *                   covariance (R)
   */
  System(std::size_t n_u, std::size_t n_x, std::size_t n_y, data_t dt,
         data_t p0 = kDefaultP0, data_t q0 = kDefaultQ0,
         data_t r0 = kDefaultR0);

  /**
   * Simulate system and produce measurement
   *
   *
   * @brief      Simulate system measurement
   *
   * @param      u_tm1  input at t-1
   *
   * @return     z      measurement
   */
  const Vector& Simulate(const Vector& u_tm1) override;

  // get methods
  /// Get output noise covariance
  const Matrix& R() const { return R_; };

  // set methods
  void set_Q(const Matrix& Q) {
    lds::System::set_Q(Q);
    do_recurse_Ke_ = true;
  }
  /// Set output noise covariance
  void set_R(const Matrix& R) {
    Reassign(R_, R);
    do_recurse_Ke_ = true;
  };

  /// Set estimator gain
  void set_Ke(const Matrix& Ke) {
    Reassign(Ke_, Ke);
    // if users have set Ke, they must not want to calculate it online.
    do_recurse_Ke_ = false;
  };
  /// Set disturbance estimator gain
  void set_Ke_m(const Matrix& Ke_m) {
    Reassign(Ke_m_, Ke_m);
    // if users have set Ke, they must not want to calculate it online.
    do_recurse_Ke_ = false;
  };

  /// Print system variables to stdout
  void Print();

 protected:
  /// System output function
  void h() override {
    cx_ = C_ * x_;
    y_ = cx_ + d_;
  };

  /// System output function: stateless
  Vector h_(Vector x) override { return C_ * x + d_; };

  /// Recursively update estimator gain
  void RecurseKe() override;

  // Gaussian-output-specific
  Matrix R_;              ///< covariance of output noise
  bool do_recurse_Ke_{};  ///< whether to recursively calculate estimator gain
};  // System
}  // namespace gaussian
}  // namespace lds

#endif
