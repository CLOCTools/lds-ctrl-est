//===-- ldsCtrlEst_h/lds_poisson_sys.h - PLDS -------------------*- C++ -*-===//
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
/// (filtering) as well as simulation of Poisson-output linear dynamical systems
/// (`lds::poisson::System`). It inherits functionality from the underlying
/// linear dynamical system (`lds::System`).
///
/// \brief PLDS base type
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_POISSON_SYS_H
#define LDSCTRLEST_LDS_POISSON_SYS_H

// namespace
#include "lds_poisson.h"
// system
#include "lds_sys.h"

// needed for Poisson random number generation
#include <random>

namespace lds {
namespace poisson {

/// Poisson System type
class System : public lds::System {
 public:
  /**
   * @brief      Constructs a new System.
   */
  System() = default;

  /**
   * @brief      Constructs a new Poisson System.
   *
   * @param      n_u    number of inputs
   * @param      n_x    number of states
   * @param      n_y    number of outputs
   * @param      dt    sample period
   * @param      p0    [optional] initial diagonal elements of state estimate
   *                   covariance (P)
   * @param      q0    [optional] initial diagonal elements of process noise
   *                   covariance (Q)
   */
  System(std::size_t n_u, std::size_t n_x, std::size_t n_y, data_t dt,
         data_t p0 = kDefaultP0, data_t q0 = kDefaultQ0);

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

 protected:
  /// System output function
  void h() override {
    cx_ = C_ * x_;
    y_ = exp(cx_ + d_);
    diag_y_.diag() = y_;
  };

  /// System output function: stateless
  Vector h_(Vector x) override { return exp(C_ * x + d_); };

  /**
   * Recursively recalculate estimator gain (Ke).
   *
   * References:
   *
   * Smith AC, Brown EN. (2003) Estimating a State-Space Model from Point
   * Process Observations. Neural Computation 15.
   *
   * Eden UT, ..., Brown EN. (2004) Dynamic Analysis of Neural Encoding by Point
   * Process Adaptive Filtering Neural Computation 16.
   *
   * @brief      Recursively recalculate estimator gain (Ke)
   */
  void RecurseKe() override;

 private:
  // Poisson-output-specific
  Matrix diag_y_;  ///< diagonal matrix with elements y
  std::poisson_distribution<size_t>
      pd_;  ///< poisson distribution for simulating data
};  // System
}  // namespace poisson
}  // namespace lds

#endif
