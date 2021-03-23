//===-- ldsCtrlEst_h/lds_poisson_fit.h - Fit Type for PLDS ------*- C++ -*-===//
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
/// This file declares and partially defines the base fit type for a
/// Gaussian-output linear dynamical system. Models are fit by either subspace
/// identification (SSID) or expectation-maximization (EM).
///
/// \brief PLDS base fit type
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_POISSON_FIT_H
#define LDSCTRLEST_LDS_POISSON_FIT_H

// namespace
#include "lds_poisson.h"
// fit
#include "lds_fit.h"

namespace lds {
namespace poisson {
/// PLDS Fit Type
class Fit : public lds::Fit {
 public:
  Fit() = default;

  /**
   * @brief      Constructs a new Fit.
   *
   * @param  n_u   number of inputs
   * @param  n_x   number of states
   * @param  n_y   number of outputs
   * @param  dt    sample period
   */
  Fit(size_t n_u, size_t n_x, size_t n_y, data_t dt)
      : lds::Fit(n_u, n_x, n_y, dt){};

  /**
   * @brief      output function
   *
   * @param      y     output estimate (over time)
   * @param      x     state estimate (over time)
   * @param      t     time index
   *
   * @return     output
   */
  View h(Matrix& y, const Matrix& x, size_t t) override {
    y.col(t) = exp(C_ * x.col(t) + d_);
    return y.col(t);
  };

  void set_R(const Matrix& R) override {
    std::cerr
        << "WARNING: Cannot set R (R[0] = " << R.at(0)
        << "). No Gaussian measurement noise in Poisson observation model.\n";
  };

  const Matrix& R() const override {
    return R_;
  };

};

};  // namespace poisson
}  // namespace lds

#endif
