//===-- ldsCtrlEst_h/lds_gaussian_fit_em.h - GLDS Fit (EM) ------*- C++ -*-===//
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
/// This file declares and partially defines a type by which Gaussian-output LDS
/// models are fit by the expectation-maximization (EM) algorithm
/// (`lds::gaussian::emFit_t`).
///
/// References:
/// [1] Shumway RH, Stoffer DS. (1982) An Approach to Time Series Smoothing and
/// Forecasting Using the EM Algorithm.Journal of Time Series Analysis 3(2).
///
/// [2] Ghahramani Z, Hinton GE. (1996) Parameter Estimation for Linear
/// Dynamical Systems. Technical Report CRG-TR-96-2.
///
/// \brief GLDS E-M fit type
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_GAUSSIAN_FIT_EM_H
#define LDSCTRLEST_LDS_GAUSSIAN_FIT_EM_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace gaussian {
///
/// @brief      GLDS E-M Fit Type
///
///             This type is used in the process of fitting GLDS models by
///             expectation-maximization (EM).
///
class emFit_t : public fit_t {
 public:
  emFit_t() : fit_t(){};
  /**
   * @brief      Constructs a new EM Fit Type.
   *
   * @param      A       state matrix
   * @param      B       input matrix
   * @param      g       input gain
   * @param      m       process disturbance
   * @param      Q       process noise covariance
   * @param      x0      initial state estimate
   * @param      P0      covariance of initial state estimate
   * @param      C       output matrix
   * @param      d       output bias
   * @param      R       output noise covariance
   * @param      dt      sample period
   * @param      uTrain  training input data
   * @param      zTrain  training measurement data
   */
  emFit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q,
          armaVec& x0, armaMat& P0, armaMat& C, armaVec& d, armaMat& R,
          data_t dt, std::vector<armaMat>& uTrain,
          std::vector<armaMat>& zTrain);
  /**
   * Constructs a new EM Fit Type.
   *
   * Initializes the model to a random walk.
   *
   * @brief      Constructs a new EM Fit Type.
   *
   * @param      dt      sample period
   * @param      uTrain  training input data
   * @param      zTrain  training measurement data
   */
  emFit_t(data_t dt, std::vector<armaMat>& uTrain,
          std::vector<armaMat>& zTrain);
  emFit_t(ssidFit_t& fit0);

  /// Reset EM fit
  void reset(void);

  /**
   * @brief      Run Expectation-Maximization algorithm
   *
   * @param      calcAB       [optional] whether to caclulate dynamics (A, B)
   * @param      calcQ        [optional] whether to calculate process noise
   *                          covariance
   * @param      calcInitial  [optional] whether to calculate initial conditions
   * @param      calcC        [optional] whether to calculate output matrix
   * @param      calcd        [optional] whether to calculate output bias
   * @param      calcR        [optional] whether to calculate output noise
   *                          covariance
   */
  void runEM(bool calcAB = true, bool calcQ = true, bool calcInitial = true,
             bool calcC = true, bool calcd = true, bool calcR = true);

  data_t tol = 1e-3;     ///< tolerance for convergence
  size_t maxIter = 100;  ///< maximum number of EM iterations

  // get methods
  /// Get state estimate
  std::vector<armaMat> getX() { return xHat; };
  /// Get output estimate
  std::vector<armaMat> getY() { return yHat; };

  // these are covariances that can be useful to the end-user
  armaMat get_sum_E_xu_tm1_xu_tm1() { return sum_E_xu_tm1_xu_tm1; };
  armaMat get_sum_E_xu_t_xu_tm1() { return sum_E_xu_t_xu_tm1; };
  armaMat get_sum_E_xu_t_xu_t() { return sum_E_xu_t_xu_t; };
  size_t getT() { return T; };

  /// Set output
  void setY(std::vector<armaMat> yHat) { this->yHat = yHat; };

 protected:
  /// Get recalculated parameters (packed into vector)
  armaVec getTheta();

  /// Kalman filter dataset
  /// Estimate x[t] | z[0...t]
  void kalmanFilt();

  /**
   * Kalman smooth dataset (fixed-interval smoothing)
   *
   * Estimate x[t] | z[0...T]
   *
   * @brief      Kalman smooth dataset (fixed-interval smoothing)
   *
   * @param      forceCommonInitial  whether to force common initial condition
   *                                 for all trials
   */
  void kalmanSmooth(bool forceCommonInitial);

  /**
   * @brief      Expectation step
   *
   * @param      forceCommonInitial  whether to force common initial condition
   *                                 for all trials
   */
  void Estep(bool forceCommonInitial = false);

  /**
   * @brief      Maximization step
   *
   * @param      calcAB       [optional] whether to caclulate dynamics (A, B)
   * @param      calcQ        [optional] whether to calculate process noise
   *                          covariance
   * @param      calcInitial  [optional] whether to calculate initial conditions
   * @param      calcC        [optional] whether to calculate output matrix
   * @param      calcd        [optional] whether to calculate output bias
   * @param      calcR        [optional] whether to calculate output noise
   *                          covariance
   */
  void Mstep(bool calcAB = true, bool calcQ = true, bool calcInitial = false,
             bool calcC = false, bool calcd = false, bool calcR = true);

  std::vector<armaMat> xHat;                ///< state estimate
  std::vector<arma::Cube<data_t>> P;        ///< state estimate covariance
  std::vector<armaMat> yHat;                ///< output estimate
  std::vector<arma::Cube<data_t>> P_t_tm1;  ///< single-lag state covariance

  armaMat sum_E_xu_t_xu_t;      ///< state-input covariance (current time)
  armaMat sum_E_xu_tm1_xu_tm1;  ///< state-input covariance (t-minus-1)
  armaMat sum_E_xu_t_xu_tm1;    ///< single lag state-input covariance
  size_t T;
};

/**
 * @brief      Perform EM for special system where y = x + noise
 *
 * @param      uTrain   training input data
 * @param      zTrain   training measurement data
 * @param      dt       sample period
 * @param      maxIter  [optional] maximum number of iterations
 * @param      tol      [optional] convergence tolerance
 * @param      q0       [optional] initial diagonal elements of process noise
 *                      covariance
 * @param      calcAB   [optional] whether to calculate dynamics (A, B)
 * @param      calcQ    [optional] whether to calculate process noise covariance
 * @param      calcR    [optional] whether to calculate output noise covariance
 *
 * @return     EM fit
 */
emFit_t emFit_x_equals_y(std::vector<armaMat>& uTrain,
                         std::vector<armaMat>& zTrain, data_t dt,
                         std::size_t maxIter = 100, data_t tol = 1e-3,
                         data_t q0 = 1e-6, bool calcAB = true,
                         bool calcQ = true, bool calcR = true);
};  // namespace gaussian
}  // namespace lds

#endif
