//===-- ldsCtrlEst_h/lds.h - Linear Dynmical System Namespace ---*- C++ -*-===//
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
/// This file defines the `lds` namespace, which will be an umbrella for linear
/// dynamical systems with Gaussian (`lds::gaussian`) or Poisson
/// (`lds::poisson`) observations.
///
/// \brief `lds` namespace
///
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_H
#define LDSCTRLEST_LDS_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

/// Linear Dynamical Systems (LDS) namespace
namespace lds {
/// Precision/type of all data in library. If need 32b, change `double` to
/// `float`.
typedef double data_t;

/// provides fill types for constructing new armadillo vectors, matrices
namespace fill = arma::fill;

typedef std::vector<data_t> stdVec;
typedef arma::Col<data_t> armaVec;
typedef arma::Mat<data_t> armaMat;
typedef arma::Cube<data_t> armaCube;
typedef arma::subview_col<data_t> armaSubVec;
typedef arma::subview<data_t> armaSubMat;

/// @defgroup control_masks Control Mode Bit Masks
/// Control mode bit masks. These can be bit-wise OR'd to use in combination.
/// @{

/// Control was designed to penalize change in input
/// (i.e., the state was augmented with input `u`)
static const std::size_t CONTROL_TYPE_U = 0x1;

/// Control using integral action
/// (i.e., the state was augmented with output `y` during design)
static const std::size_t CONTROL_TYPE_INTY = CONTROL_TYPE_U << 1;

/// Control setpoint adapted with (re-estimated) process disturbance `m`.
static const std::size_t CONTROL_TYPE_ADAPT_M = CONTROL_TYPE_U << 2;
/// @}

/// Some useful numbers
static data_t inf = std::numeric_limits<data_t>::infinity();
static data_t neginf = -inf;
static const data_t pi = arma::datum::pi;

/// @defgroup defaults Defaults
/// Default values for common variables
/// (e.g., default diagonal elements of covariances in case there is need to
/// resolve dimension mismatch)
/// @{
static data_t DEFAULT_P0 = 1e-6;
static data_t DEFAULT_Q0 = 1e-6;
static data_t DEFAULT_R0 = 1e-2;
static std::vector<data_t> DEFAULT_T0 = std::vector<data_t>(1, 0.0);

// Ideally these should be const, but trying to pass reference which means its
// val implicitly must be allowed to change. hopefully no one will try to
// reassign their vals...
static data_t DATA_T_ZERO = (data_t)0;
static data_t DATA_T_ONE = (data_t)1;
static data_t DEFAULT_SOFTSTART = (data_t)0;
static bool FALSE = false;
static bool TRUE = true;
/// @}

/// Weighting options for singular value decomposition performed during
/// subspace identification (SSID): None, MOESP "robust method",
/// CVA "Canonical Variate Analysis")
enum ssidWt { NONE, MOESP, CVA };
}  // namespace lds

// fit and sys with dynamics only
#include "lds_sys.h"
#ifdef LDSCTRLEST_BUILD_FIT
#include "lds_fit.h"
#endif

// Pull in output equation variants
#include "lds_gaussian.h"
#include "lds_poisson.h"

#endif
