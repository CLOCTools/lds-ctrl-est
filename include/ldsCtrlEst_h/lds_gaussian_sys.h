//===-- ldsCtrlEst_h/lds_gaussian_sys.h - GLDS ------------------*- C++ -*-===//
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
/// (filtering) as well as simulation of Gaussian-output linear dynamical
/// systems
/// (`lds::gaussian::sys_t`). It inherits functionality from the underlying
/// linear dynamical system (`lds::sys_t`).
///
/// \brief GLDS base type
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_GAUSSIAN_SYS_H
#define LDSCTRLEST_LDS_GAUSSIAN_SYS_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace gaussian {
/// Gaussian LDS Type
class sys_t : public lds::sys_t {
 public:
  /**
   * Given current measurment and input, filter data to produce causal state
   * estimates using Kalman filtering, which procedes by predicting the state
   * and subsequently updating.
   *
   * @brief      Filter data to produce causal state estimates
   *
   * @param      u_tm1         input at t-minus-1
   * @param      z_t           current measurement
   * @param      doRecurse_Ke  whether to calculate the Kalman estimator gain
   *                           (Ke) recursively
   */
  void filter(armaVec& u_tm1, armaVec& z_t, bool doRecurse_Ke = true);

  /**
   * Given current measurment, filter data to produce causal state estimates
   * using Kalman filtering, which procedes by predicting the state and
   * subsequently updating.
   *
   * @brief      Filter data to produce causal state estimates
   *
   * @param      z             current measurement
   * @param      doRecurse_Ke  whether to calculate the Kalman estimator gain
   *                           (Ke) recursively
   */
  void filter(armaVec& z, bool doRecurse_Ke = true);

  /**
   * Simulate system measurement: z ~ Poisson(y)
   *
   * n.b., In reality, this is only Poisson where rate `y` and sample period
   * `dt` are sufficiently small there is only ever 0 or 1 events in a period.
   * If either of those is violated, results will be innacurate.
   *
   * @brief      Simulate system measurement
   *
   * @param      z     measurement
   */
  void simMeasurement(armaVec& z);

  /**
   * @brief      Constructs a new GLDS.
   *
   * @param      nU    number of inputs (u)
   * @param      nX    number of states (x)
   * @param      nY    number of outputs (y)
   * @param      dt    sample period
   * @param      p0    [optional] initial diagonal elements of state estimate
   *                   covariance (P)
   * @param      q0    [optional] initial diagonal elements of process noise
   *                   covariance (Q)
   * @param      r0    [optional] initial diagonal elements of output noise
   *                   covariance (R)
   */
  sys_t(std::size_t nU, std::size_t nX, std::size_t nY, data_t& dt,
        data_t& p0 = DEFAULT_P0, data_t& q0 = DEFAULT_Q0,
        data_t& r0 = DEFAULT_R0);
  sys_t& operator=(const sys_t& sys);

  // get methods
  /// Get number of outputs (y)
  size_t getNy() const { return nY; };

  /// Get output matrix (C)
  armaMat getC() const { return C; };

  /// Get output bias (d)
  armaVec getD() const { return d; };

  /// Get output noise covariance (R)
  armaVec getR() const { return R; };

  /// Get output (y)
  armaVec getY() const { return y; };

  /// Get measurement (z)
  armaVec getZ() const { return z; };

  /// Get estimator gain (Ke)
  armaMat getKe() const { return Ke; };
  /// Get estimator gain for process disturbance (Ke_m)
  armaMat getKe_m() const { return Ke_m; };

  // set methods
  /// Set dimensions of system
  void setDims(std::size_t& nU, std::size_t& nX, std::size_t& nY);

  /// Set output matrix (C)
  void setC(stdVec& cVec);
  /// Set output matrix (C)
  void setC(armaMat& C);

  /// Set output bias (d)
  void setD(stdVec& dVec);
  /// Set output bias (d)
  void setD(armaVec& d);

  /// Set output noise covariance (R)
  void setR(stdVec& rVec);
  /// Set output noise covariance (R)
  void setR(armaMat& R);

  /// Set measurement (z)
  void setZ(stdVec& zVec);
  /// Set measurement (z)
  void setZ(armaVec& z);

  /// Set estimator gain (Ke)
  void setKe(stdVec& keVec);
  /// Set estimator gain (Ke)
  void setKe(armaMat& Ke);

  /// Set disturbance estimator gain (Ke_m)
  void setKe_m(stdVec& kemVec);
  /// Set disturbance estimator gain (Ke_m)
  void setKe_m(armaMat& Ke_m);

  /// Reset system variables
  void reset();

  /// Print system variables to stdout
  void printSys();

 protected:
  /// Recursively recalculate Kalman estimator gain (Ke)
  void recurse_Ke();

  /// One-step prediction
  void predict();

  /// System output function
  void h();

  /// Reset to default R (identity matrix with diagonal elements r0)
  void defaultR();

  // output-specific stuff
  std::size_t nY;  ///< number of outputs
  armaMat C;       ///< output matrix
  armaVec d;       ///< output bias
  armaMat R;       ///< covariance of output noise
  data_t& r0;      ///< default values for R
  armaVec y;       ///< output
  armaVec z;       ///< measurement

  armaMat Ke;    ///< state estimator gain
  armaMat Ke_m;  ///< disturbance estimator gain
};               // sys_t
}  // namespace gaussian
}  // namespace lds

#include "lds_gaussian_ctrl.h"

#endif
