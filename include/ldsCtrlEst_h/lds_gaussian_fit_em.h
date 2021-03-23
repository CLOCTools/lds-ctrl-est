//===-- ldsCtrlEst_h/lds_gaussian_fit_em.h - GLDS Fit (EM) ------*- C++ -*-===//
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
/// This file declares and partially defines a type by which Gaussian-output LDS
/// models are fit by the expectation-maximization (EM) algorithm
/// (`lds::gaussian::emFit_t`).
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

#ifndef LDSCTRLEST_LDS_GAUSSIAN_FIT_EM_H
#define LDSCTRLEST_LDS_GAUSSIAN_FIT_EM_H

#include "lds_fit_em.h"
#include "lds_gaussian_fit.h"

namespace lds {
namespace gaussian {
///
/// @brief      GLDS E-M Fit Type
///
///             This type is used in the process of fitting GLDS models by
///             expectation-maximization (EM).
///
class FitEM : public EM<Fit> {
 public:
  using EM<Fit>::EM;

 private:
  /**
   * @brief      estimate C+d by maximizing likelihood
   */
  void MaximizeOutput() override;

  /**
   * @brief      estimate measurement noise covariance by maximizing likelihood
   */
  void MaximizeMeasurement() override;

  /**
   * @brief      recursively update estimator gain Ke
   *
   * @param      Ke      estimator gain
   * @param      P_pre   cov of predicted state est.
   * @param      P_post  cov of postior sate est.
   * @param[in]  t       time
   */
  void RecurseKe(Matrix& Ke, Cube& P_pre, Cube& P_post, size_t t) override;
};

}  // namespace gaussian
}  // namespace lds

#endif
