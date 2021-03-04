//===-- ldsCtrlEst_h/lds_poisson_sys.h - PLDS -------------------*- C++ -*-===//
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
/// This file declares and partially defines the type for state estimation
/// (filtering) as well as simulation of Poisson-output linear dynamical systems
/// (`lds::poisson::sys_t`). It inherits functionality from the underlying
/// linear dynamical system (`lds::sys_t`).
///
/// \brief PLDS base type
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_POISSON_SYS_H
#define LDSCTRLEST_LDS_POISSON_SYS_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace poisson {
///
/// @brief      Poisson LDS Type
///
class sys_t : public lds::sys_t {
 public:
  /**
   * Given current measurment and input, filter data to produce causal state
   * estimates using a point-process filter, which procedes by predicting the
   * state and subsequently updating.
   *
   * References:
   *
   * Smith AC, Brown EN. (2003) Estimating a State-Space Model from Point
   * Process Observations. Neural Computation 15.
   *
   * Eden UT, ..., Brown EN. (2004) Dynamic Analysis of Neural Encoding by Point
   * Process Adaptive Filtering Neural Computation 16.
   *
   * @brief      Filter data to produce causal state estimates
   *
   * @param      u_tm1  input at t-minus-1
   * @param      z_t    current measurement
   */
  void filter(armaVec& u_tm1, armaVec& z_t);

  /**
   * Given current measurment and input, filter data to produce causal state
   * estimates using a point-process filter, which procedes by predicting the
   * state and subsequently updating.
   *
   * References:
   *
   * Smith AC, Brown EN. (2003) Estimating a State-Space Model from Point
   * Process Observations. Neural Computation 15.
   *
   * Eden UT, ..., Brown EN. (2004) Dynamic Analysis of Neural Encoding by Point
   * Process Adaptive Filtering Neural Computation 16.
   *
   * @brief      Filter data to produce causal state estimates
   *
   * @param      z    current measurement
   */
  void filter(armaVec& z);

  /**
   * @brief      Simulate one step of the model and produce a measurement
   *
   * @param      z     measurement
   */
  void simMeasurement(armaVec& z);

  /**
   * @brief      Constructs a new PLDS.
   *
   * @param      nU    number of inputs
   * @param      nX    number of states
   * @param      nY    number of outputs
   * @param      dt    sample period
   * @param      p0    [optional] initial diagonal elements of state estimate
   *                   covariance (P)
   * @param      q0    [optional] initial diagonal elements of process noise
   *                   covariance (Q)
   */
  sys_t(std::size_t nU, std::size_t nX, std::size_t nY, data_t& dt,
        data_t& p0 = DEFAULT_P0, data_t& q0 = DEFAULT_Q0);
  sys_t& operator=(const sys_t& sys);

  // get methods
  /// Get number of outputs (y)
  size_t getNy() const { return nY; };

  /// Get output matrix (C)
  armaMat getC() const { return C; };

  /// Get output bias (D)
  armaVec getD() const { return d; };

  /// Get output (y)
  armaVec getY() const { return y; };

  /// Get measurement (z)
  armaVec getZ() const { return z; };

  // set methods
  /// Set dimensions of sytem
  void setDims(std::size_t& nU, std::size_t& nX, std::size_t& nY);

  /// Set output matrix (C)
  void setC(stdVec& cVec);
  /// Set output matrix (C)
  void setC(armaMat& C);

  /// Set output bias (d)
  void setD(stdVec& dVec);
  /// Set output bias (d)
  void setD(armaVec& d);

  /// Set measurement (z)
  void setZ(stdVec& zVec);
  /// Set measurement (z)
  void setZ(armaVec& z);

  /// Reset system variables
  void reset();
  /// Print system variables to stdout
  void printSys();

 protected:
  /// One-step prediction
  void predict();
  /// System output function
  void h();  // output nonlinearity

  // output-specific stuff
  std::size_t nY;  ///< number of outputs
  armaMat C;       ///< output matrix
  armaVec d;       ///< output bias
  armaVec y;       ///< output
  armaVec logy;    ///< logarithm of output
  armaVec z;       ///< measurement

  armaMat diag_y;  ///< diagonal matrix with elements y
  armaVec chance;  ///< p.r. number for rolling dice if simulating data
};                 // sys_t
}  // namespace poisson
}  // namespace lds

#include "lds_poisson_ctrl.h"

#endif
