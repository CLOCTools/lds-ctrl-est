//===-- ldsCtrlEst_h/lds.h - Linear Dynmical System Namespace ---*- C++ -*-===//
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
/// This file defines the `lds` namespace, which will be an umbrella for linear
/// dynamical systems with Gaussian (`lds::gaussian`) or Poisson
/// (`lds::poisson`) observations.
///
/// \brief `lds` namespace
///
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_H
#define LDSCTRLEST_LDS_H

// #ifndef LDSCTRLEST
// #include <ldsCtrlEst>
// #endif

#include <armadillo>

/// Linear Dynamical Systems (LDS) namespace
namespace lds {
/// Type of all data in library. If need 32b, change `double` to `float`. This
/// could be potentially useful for large scale problems where there are memory
/// constraints.
using data_t = double;  // may change to float (but breaks mex functions)
using Vector = arma::Col<data_t>;
using Matrix = arma::Mat<data_t>;
using Cube = arma::Cube<data_t>;
using View = arma::subview<data_t>;

/// provides fill types for constructing new armadillo vectors, matrices
namespace fill = arma::fill;

/// @defgroup control_masks Control Mode Bit Masks
/// Control mode bit masks. These can be bit-wise OR'd to use in combination.
/// @{

/// Control was designed to penalize change in input
/// (i.e., the state was augmented with input `u`)
///
/// \brief control designed to penalize change in input
static const std::size_t kControlTypeDeltaU = 0x1;

/// Control using integral action
/// (i.e., the state was augmented with output `y` during design)
///
/// \brief control using integral action
static const std::size_t kControlTypeIntY = kControlTypeDeltaU << 1;

/// Adapt control setpoint adapted with re-estimated process disturbance `m`.
///
/// \brief adapt control setpoint with re-estimated disturbance `m`
static const std::size_t kControlTypeAdaptM = kControlTypeDeltaU << 2;
/// @}

/// Some useful numbers
static const data_t kInf = std::numeric_limits<data_t>::infinity();
static const data_t kPi = arma::datum::pi;

/// @defgroup defaults Defaults
/// Default values for common variables
/// (e.g., default diagonal elements of covariances)
/// @{
static const data_t kDefaultP0 = 1e-6;  ///< default state estimate covar
static const data_t kDefaultQ0 = 1e-6;  ///< default process noise covar
static const data_t kDefaultR0 = 1e-2;  ///< default output noise covar
/// @}

/// Weighting options for singular value decomposition performed during subspace
/// identification (SSID)
///
/// Reference:
///
/// van Overschee, de Moor. 1996. Subspace Identification for Linear Systems.
///
/// \brief weighting options for SSID
enum SSIDWt {
  kSSIDNone,   ///< None
  kSSIDMOESP,  ///< MOESP (AKA "robust method" in van Overschee 1996)
  kSSIDCVA     ///< CVA "Canonical Variate Analysis"
};

enum MatrixListFreeDim {
  kMatFreeDimNone,  ///< neither dim free to be hetero in mat list
  kMatFreeDim1,     ///< allow 1st dim of mats in list to be hetero
  kMatFreeDim2      ///< allow 2nd dim of mats in list to be hetero
};

// TODO(mfbolus): for SwitchedController, may want systems to have differing
// numbers of states. Use this enum as template parameter?
// enum SystemListFreeDim {
//   kSysFreeDimNone,
//   kSysFreeDimX  ///< allow state dim (x) of systems in list to be hetero
// };

// place hard limits on contents of vecors/mats
void Limit(std::vector<data_t>& x, data_t lb, data_t ub);
void Limit(Vector& x, data_t lb, data_t ub);
void Limit(Matrix& x, data_t lb, data_t ub);

// in-place assign that errs if there are dimension mismatches:
/**
 * @brief      reassigns contents of some Vector in place
 *
 * @param      some           some Vector
 * @param      other          other Vector
 * @param      parenthetical  optional description provided by caller to ease
 *                            debugging
 */
void Reassign(Vector& some, const Vector& other,
              const std::string& parenthetical = "Reassign");
/**
 * @brief      reassigns contents of some Matrix in place
 *
 * @param      some           some Matrix
 * @param      other          other Matrix
 * @param      parenthetical  optional description provided by caller to ease
 *                            debugging
 */
void Reassign(Matrix& some, const Matrix& other,
              const std::string& parenthetical = "Reassign");

// TODO(mfbolus): this is a fudge, but for some reason, cov mats often going
// numerically asymm.

/**
 * @brief      forces matrix to be symmetric positive-definite
 *
 * @param      X     mutated matrix
 */
void ForceSymPD(Matrix& X);

/**
 * @brief      forces matrix to be symmetric and have a minimum eigenvalue
 *
 * @param      X        mutated matrix
 * @param      eig_min  [optional] minimum eigen value
 */
void ForceSymMinEig(Matrix& X, data_t eig_min = 0);

/**
 * @brief      LQ decomposition
 *
 * @param      L     lower triangle matrix
 * @param      Qt    orthonormal matrix (transposed cf QR decomp)
 * @param      X     matrix being decomposed
 */
void lq(Matrix& L, Matrix& Qt, const Matrix& X);

/**
 * @brief      Calculate covariance matrix
 *
 * @param      A     some matrix
 * @param      B     some other matrix
 *
 * @return     covariance
 */
Matrix calcCov(const Matrix& A, const Matrix& B);

inline void Limit(std::vector<data_t>& x, data_t lb, data_t ub) {
  for (data_t& el : x) {
    el = el < lb ? lb : el;
    el = el > ub ? ub : el;
  }
}
inline void Limit(Vector& x, data_t lb, data_t ub) {
  for (data_t& el : x) {
    el = el < lb ? lb : el;
    el = el > ub ? ub : el;
  }
}
inline void Limit(Matrix& x, data_t lb, data_t ub) {
  for (data_t& el : x) {
    el = el < lb ? lb : el;
    el = el > ub ? ub : el;
  }
}

inline void Reassign(Vector& some, const Vector& other,
                     const std::string& parenthetical) {
  // check dimensions
  if (other.n_elem != some.n_elem) {
    std::ostringstream ss;
    ss << "cannot reassign vector of size " << some.n_elem
       << " with vector of size " << other.n_elem << "(" << parenthetical
       << ")";
    throw std::runtime_error(ss.str());
  }

  for (size_t k = 0; k < some.n_elem; k++) {
    some[k] = other[k];
  }
}

inline void Reassign(Matrix& some, const Matrix& other,
                     const std::string& parenthetical) {
  // check dimensions
  if ((other.n_rows != some.n_rows) || (other.n_cols != some.n_cols)) {
    std::ostringstream ss;
    ss << "cannot reassign matrix of size " << some.n_rows << "x" << some.n_cols
       << " with matrix of size " << other.n_rows << "x" << other.n_cols << "("
       << parenthetical << ")";
    throw std::runtime_error(ss.str());
  }

  for (size_t k = 0; k < some.n_elem; k++) {
    some[k] = other[k];
  }
}

}  // namespace lds

#endif
