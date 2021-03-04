//===-- ldsCtrlEst_h/lds_gaussian_fit.h - Fit Type for GLDS -----*- C++ -*-===//
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
/// This file declares and partially defines the base fit type for a
/// Gaussian-output linear dynamical system. Models are fit by either subspace
/// identification (SSID) or expectation-maximization (EM).
///
/// \brief GLDS fit type
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_GAUSSIAN_FIT_H
#define LDSCTRLEST_LDS_GAUSSIAN_FIT_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace gaussian {
/// GLDS Fit Type
class fit_t : public lds::fit_t {
 public:
  fit_t(){};
  /**
   * @brief      Constructs a new GLDS fit.
   *
   * @param      A       state matrix
   * @param      B       input matrix
   * @param      g       input gain
   * @param      m       process disturbance
   * @param      Q       process noise covariance
   * @param      x0      initial state estimate
   * @param      P0      covariance of initial state estimate
   * @param      C       output matrix
   * @param      D       feedthrough matrix
   * @param      d       output bias
   * @param      R       covariance of output noise
   * @param      dt      sample period
   * @param      uTrain  input training data
   * @param      zTrain  measurement training data
   */
  fit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q, armaVec& x0,
        armaMat& P0, armaMat& C, armaMat& D, armaVec& d, armaMat& R, data_t dt,
        std::vector<armaMat>& uTrain, std::vector<armaMat>& zTrain);

  /**
   * @brief      Constructs a new GLDS fit type.
   *
   * @param      A       state matrix
   * @param      B       input matrix
   * @param      g       input gain
   * @param      m       process disturbance
   * @param      Q       process noise covariance
   * @param      x0      initial state estimate
   * @param      P0      covariance of initial state estimate
   * @param      C       output matrix
   * @param      d       output bias
   * @param      R       covariance of output noise
   * @param      dt      sample period
   * @param      uTrain  input training data
   * @param      zTrain  measurement training data
   */
  fit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q, armaVec& x0,
        armaMat& P0, armaMat& C, armaVec& d, armaMat& R, data_t dt,
        std::vector<armaMat>& uTrain, std::vector<armaMat>& zTrain);

  // Output
  armaMat C;  ///< output matrix
  armaMat D;  ///< feedthrough matrix
  armaVec d;  ///< output bias
  armaMat R;  ///< output noise cov

  // input/output training data
  std::vector<armaMat> uTrain;  ///< input training data
  std::vector<armaMat> zTrain;  ///< measurement training data
};

};  // namespace gaussian
}  // namespace lds

// subspace identification
#include "lds_gaussian_fit_ssid.h"

// expectation maximization
#include "lds_gaussian_fit_em.h"

#endif
