//===-- lds.cpp - LDS -----------------------------------------------------===//
//
// Copyright 2021 Michael Bolus
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

#include <ldsCtrlEst_h/lds.h>

// insert any necessary function definitions here.
namespace lds {

void ForceSymPD(Matrix& X) {
  if (X.is_sympd() || !X.is_square()) {
    return;
  }

  // make symmetric
  X = (X + X.t()) / 2;

  // for eigenval decomp
  bool did_succeed(true);
  Vector d;
  Matrix u;

  // see first method (which may not be ideal):
  // https://nhigham.com/2021/02/16/diagonally-perturbing-a-symmetric-matrix-to-make-it-positive-definite/
  size_t k(1);
  bool is_sympd = X.is_sympd();

  while (!is_sympd) {
    if (k > 100) {
      did_succeed = arma::eig_sym(d, u, X, "std");
      data_t min_eig = arma::min(d);
      std::cerr << "After multiple iterations, min eigen val = " << min_eig
                << ".\n";
      throw std::runtime_error(
          "Failed to make matrix symmetric positive definite.");
      return;
    }

    did_succeed = arma::eig_sym(d, u, X, "std");
    if (!did_succeed) {
      throw std::runtime_error("ForceSymPD failed.");
    }

    data_t min_eig = arma::min(d);
    data_t max_abs_eig = arma::max(arma::abs(d));
    data_t scale_eps = std::max(arma::datum::eps, arma::eps(max_abs_eig));
    X.diag() += std::abs(min_eig) + scale_eps;

    // make sure symm:
    X = (X + X.t()) / 2;
    is_sympd = X.is_sympd();
    k++;
  }
}

void ForceSymMinEig(Matrix& X, data_t eig_min) {
  if (!X.is_square()) {
    return;
  }

  // make symmetric
  X = (X + X.t()) / 2;

  bool did_succeed(true);
  Vector d;
  Matrix u;
  did_succeed = arma::eig_sym(d, u, X, "std");
  if (!did_succeed) {
    throw std::runtime_error("ForceSymMinEig failed.");
  }
  Limit(d, eig_min + arma::eps(eig_min), kInf);  // enforce lower bound
  Matrix d_diag = arma::diagmat(d);
  X = u * d_diag * u.t();

  // double check symmetric
  X = (X + X.t()) / 2;
}

void lq(Matrix& L, Matrix& Qt, const Matrix& X) {
  bool did_succeed(true);
  did_succeed = arma::qr_econ(Qt, L, X.t());
  if (!did_succeed) {
    throw std::runtime_error("LQ decomposition failed.");
  }
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
