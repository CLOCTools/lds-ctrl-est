//===-- lds_fit.cpp - Fit Type for LDS --------------------------------------===//
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
/// This file implements the base fit type for a linear dynamical system. It is
/// expounded upon by variants with Gaussian and Poisson observation
/// assumptions for fitting.
///
/// \brief LDS base fit type
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>

using namespace std;
using namespace lds;

lds::fit_t::fit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q,
                  armaVec& x0, armaMat& P0, data_t& dt) {
  this->A = A;
  this->B = B;
  this->g = g;
  this->m = m;
  this->Q = Q;

  this->x0 = x0;

  if (P0.is_sympd()) {
    this->P0 = P0;
  } else {
    cerr << "Warning: P0 was not symmetric/positive-def. Going to set P0 to "
            "diag mat with default value...\n";
    this->P0 = DEFAULT_P0 * armaMat(A.n_rows, A.n_cols, fill::eye);
  }

  this->dt = dt;
}
