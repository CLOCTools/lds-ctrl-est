//===-- ldsCtrlEst_h/lds_gaussian_fit_ssid.h - GLDS Fit (SSID) --*- C++ -*-===//
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
/// models are fit by a subspace identification (SSID) algorithm
/// (`lds::gaussian::ssidFit_t`).
///
/// References:
/// [1] van Overschee P, de Moore B. (1996) Subspace Identification for Linear
/// Systems. Boston: Springer.
///
/// \brief GLDS SSID fit type
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_GAUSSIAN_FIT_SSID_H
#define LDSCTRLEST_LDS_GAUSSIAN_FIT_SSID_H

#include "lds_fit_ssid.h"
#include "lds_gaussian_fit.h"

namespace lds {
namespace gaussian {
/// Subspace Identification (SSID) for GLDS
class FitSSID : public SSID<Fit> {
 public:
  using SSID<Fit>::SSID;
  using SSID<Fit>::Run;

 private:
  using SSID<Fit>::CreateHankelDataMat;
  using SSID<Fit>::CalcSVD;
  using SSID<Fit>::Solve;

  /**
   * @brief      performs QR decomposition of data to get lower-triangular
   *             matrix for solve step
   */
  void DecomposeData() override;

  /**
   * Solves for LDS parameters as in van Overschee, de Moor (1996)
   *
   * Currently unused, but here for archival purposes.
   *
   * @brief      solves for LDS parameters as in van Overschee, de Moor (1996)
   */
  void SolveVanOverschee();
};
}  // namespace gaussian
}  // namespace lds
#endif
