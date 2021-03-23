//===-- ldsCtrlEst_h/lds_poisson_fit_ssid.h - PLDS Fit (SSID) ---*- C++ -*-===//
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
/// This file declares and partially defines a type by which Poisson-output LDS
/// models are fit by a subspace identification (SSID) algorithm
/// (`lds::gaussian::FitSSID`).
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

#ifndef LDSCTRLEST_LDS_POISSON_FIT_SSID_H
#define LDSCTRLEST_LDS_POISSON_FIT_SSID_H

#include "lds_fit_ssid.h"
#include "lds_poisson_fit.h"

namespace lds {
namespace poisson {
/// Subspace Identification (SSID) for PLDS
class FitSSID : public SSID<Fit> {
 public:
  using SSID<Fit>::SSID;

 private:
  /**
   * @brief      Decomposes the data, after calculating the covariances and
   *             converting from Poisson->Gaussian. See Buesing et al. 2012.
   */
  void DecomposeData() override;

  /**
   * @brief      calculates the covariances and converts from Poisson->Gaussian.
   *             See Buesing et al. 2012.
   */
  void CalcCov();

  /**
   * @brief      converts from Poisson->Gaussian mean/covariance.
   *             See Buesing et al. 2012.
   */
  void PoissonToGaussianMoments();
  Matrix cov_;
};

}  // namespace poisson
}  // namespace lds
#endif
