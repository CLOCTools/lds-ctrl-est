//===-- lds_fit.cpp - Fit Type for LDS ------------------------------------===//
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
/// This file implements the base fit type for a linear dynamical system. It is
/// expounded upon by variants with Gaussian and Poisson observation
/// assumptions for fitting.
///
/// \brief LDS base fit type
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>

namespace lds {
Fit::Fit(size_t n_u, size_t n_x, size_t n_y, data_t dt) {
  dt_ = dt;
  n_x_ = n_x;
  n_y_ = n_y;
  n_u_ = n_u;

  A_ = Matrix(n_x_, n_x_, fill::eye);
  B_ = Matrix(n_x_, n_u_, fill::zeros);
  g_ = Vector(n_u_, fill::ones);
  m_ = Vector(n_x_, fill::zeros);
  Q_ = kDefaultQ0 * Matrix(n_x_, n_x_, fill::eye);

  C_ = Matrix(n_y_, n_x_, fill::eye);
  d_ = Vector(n_y_, fill::zeros);

  x0_ = Vector(n_x_, fill::zeros);
  P0_ = kDefaultP0 * Matrix(n_x_, n_x_, fill::eye);
}
}  // namespace lds
