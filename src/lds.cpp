//===-- lds.cpp - LDS -----------------------------------------------------===//
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
/// This file implements miscellaneous lds namespace functions not bound to a
/// class.
///
/// \brief misc lds namespace functions
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>

// insert any necessary function definitions here.
namespace lds {

void ForceSymPD(Matrix& X) {
  if (X.is_sympd() || !X.is_square()) {
    return;
  }

  // make symmetric
  X = (X + X.t()) / 2;

  Vector d;
  Matrix u;
  arma::eig_sym(d, u, X, "std");
  Matrix d_diag = arma::diagmat(abs(d));  // force to be positive...
  X = u * d_diag * u.t();
}

void ForceSymMinEig(Matrix& X, data_t eig_min) {
  if (!X.is_square()) {
    return;
  }
  Vector d;
  Matrix u;
  arma::eig_sym(d, u, X);//, "std");
  Limit(d, eig_min, kInf);//enforce lower bound
  Matrix d_diag = arma::diagmat(d);
  X = u * d_diag * u.t();

  // make symmetric
  X = (X + X.t()) / 2;
}

void lq(Matrix& L, Matrix& Qt, const Matrix& X) {
  arma::qr_econ(Qt, L, X.t());
  arma::inplace_trans(L);
  arma::inplace_trans(Qt);
}

Matrix calcCov(const Matrix& A, const Matrix& B) {
  // subtract out mean
  auto m_a = arma::mean(A, 1);
  Matrix a0 = A;
  a0.each_col() -= m_a;

  auto m_b = arma::mean(B, 1);
  Matrix b0 = B;
  b0.each_col() -= m_b;

  Matrix cov = a0 * b0.t() / a0.n_cols;
  return cov;
}

}  // namespace lds
