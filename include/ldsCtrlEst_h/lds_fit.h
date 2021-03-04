//===-- ldsCtrlEst_h/lds_fit.h - Fit Type for LDS ---------------*- C++ -*-===//
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
/// This file declares and partially defines the base fit type for a linear
/// dynamical system. It is expounded upon by variants with Gaussian and Poisson
/// observation assumptions for fitting.
///
/// \brief LDS base fit type
//===----------------------------------------------------------------------===//

#ifndef LDS_FIT_HPP
#define LDS_FIT_HPP

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
/// LDS Fit Type
class fit_t {
 public:
  fit_t(){};
  /**
   * @brief      Constructs a new LDS fit type.
   *
   * @param      A     state matrix
   * @param      B     input matrix
   * @param      g     input gain
   * @param      m     process disturbance
   * @param      Q     process noise covariance
   * @param      x0    initial state estimate
   * @param      P0    covariance of initial state estimate
   * @param      dt    { parameter_description }
   */
  fit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q, armaVec& x0,
        armaMat& P0, data_t& dt);

  // Dynamics
  armaMat A;   ///< state matrix
  armaMat B;   ///< input matrix
  armaVec g;   ///< input gain
  armaVec m;   ///< process noise mean
  armaMat Q;   ///< process noise cov
  armaVec x0;  ///< initial state
  armaMat P0;  ///< initial covar

  data_t dt;  ///< sample period
};

}  // namespace lds
#endif
