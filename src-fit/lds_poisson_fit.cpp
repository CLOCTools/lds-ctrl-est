//===-- lds_poisson_fit.h - Fit Type for PLDS -----------------------------===//
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
/// This file implements the base fit type for a Gaussian-output linear
/// dynamical system. Models are fit by either subspace identification (SSID) or
/// expectation-maximization (EM).
///
/// \brief PLDS base fit type
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>

using namespace std;
using namespace plds;

lds::poisson::fit_t::fit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m,
                           armaMat& Q, armaVec& x0, armaMat& P0, armaMat& C,
                           armaVec& d, data_t dt, vector<armaMat>& uTrain,
                           vector<armaMat>& zTrain)
    : lds::fit_t(A, B, g, m, Q, x0, P0, dt) {
  this->C = C;
  this->d = d;

  this->uTrain = uTrain;
  this->zTrain = zTrain;
}
