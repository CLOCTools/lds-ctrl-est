//===-- lds_gaussian_fit.cpp - Fit Type for GLDS --------------------------===//
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
/// This file implements the base fit type for a Gaussian-output linear
/// dynamical system. Models are fit by either subspace identification (SSID) or
/// expectation-maximization (EM).
///
/// \brief GLDS fit type
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>

namespace lds {
namespace gaussian {

Fit::Fit(size_t n_u, size_t n_x, size_t n_y, data_t dt)
    : lds::Fit(n_u, n_x, n_y, dt) {
  R_ = kDefaultR0 * Matrix(n_y_, n_y_, fill::eye);
}

}  // namespace gaussian
}  // namespace lds
