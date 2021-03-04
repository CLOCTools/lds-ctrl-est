//===-- ldsCtrlEst_h/lds_poisson_fit_ssid.h - PLDS Fit (SSID) ---*- C++ -*-===//
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
/// This file declares and partially defines a type by which Poisson-output LDS
/// models are fit by a subspace identification (SSID) algorithm
/// (`lds::gaussian::ssidFit_t`). Currently, these models are fit assuming
/// linear/Gaussian output, and then an exponential output is fit by maximum
/// likelihood under the Poisson assumption. Future work should implement SSID
/// after Poisson<->Gaussian moment conversions as outlined in [2].
///
/// References:
/// [1] van Overschee P, de Moore B. (1996) Subspace Identification for Linear
/// Systems. Boston: Springer.
/// [2] Buesing L, Macke JH, Sahani M. (2012) Spectral learning of linear
/// dynamics from generalised-linear observations with application to neural
/// population data. NIPS 25.
///
/// \brief PLDS SSID fit type
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_POISSON_FIT_SSID_H
#define LDSCTRLEST_LDS_POISSON_FIT_SSID_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace poisson {
///
/// @brief      GLDS SSID Fit Type
///
class ssidFit_t : public fit_t {
 public:
  ssidFit_t() : fit_t(){};
  /**
   * @brief      Constructs a new PLDS SSID fit.
   *
   * @param      A            state matrix
   * @param      B            input matrix
   * @param      g            input gain
   * @param      m            process disturbance
   * @param      Q            process noise covariance
   * @param      x0           initial state estimate
   * @param      P0           covariance of initial state estimate
   * @param      C            output matrix
   * @param      d            output bias
   * @param      dt           sample period
   * @param      t_startSSID  start time for the data used in SSID
   * @param      t_stopSSID   stop time for the data used in SSID
   * @param      singVals     singular values from SSID SVD step
   * @param      t0           start times for each trial of input/output data
   * @param      uTrain       training input data
   * @param      zTrain       training measurement data
   */
  ssidFit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q,
            armaVec& x0, armaMat& P0, armaMat& C, armaVec& d, data_t dt,
            data_t t_startSSID, data_t t_stopSSID, armaVec& singVals,
            std::vector<data_t>& t0, std::vector<armaMat>& uTrain,
            std::vector<armaMat>& zTrain);
  std::vector<data_t> t0;
  data_t t_startSSID;
  data_t t_stopSSID;
  armaVec singVals;
};

/**
 * Fit PLDS model by subspace identification (SSID). System matrices (A,B,C) are
 * fit by SSID assuming gaussian observations. Iteratively refits output
 * function parameters (C, d) by Poisson MLE using Newton's method.
 *
 * @brief      Fit a PLDS model using subspace identification (SSID)
 *
 * @param      u               inputs
 * @param      n               { parameter_description }
 * @param      dt              sample period
 * @param      nX              number of states
 * @param      nH              block-Hankel data matrix size
 * @param      d0              previous estimate of output bias (d)
 * @param      wt              chosen weighting for SSID singular value \
 *                             decomposition (NONE, MOESP, CVA)
 * @param      wtG0            chosen weighting for minimizing model error at \
 *                             DC
 * @param      t0              start times for each trial of input/output data
 * @param      t_startSSID     start time for the data used in SSID
 * @param      t_stopSSID      stop time for the data used in SSID
 * @param      assumeGaussian  [NONFUNCTIONAL] whether to assume Gaussian
 *                             observation model and fit by linear SSID first,
 *                             then fit output nonlinearity by Poisson MLE
 * @param      z     measurements
 *
 * @return     tuple<PLDS fit, GLDS fit>
 */
std::tuple<ssidFit_t, lds::gaussian::ssidFit_t> ssidFit(
    std::vector<armaMat>& u, std::vector<armaMat>& z, data_t dt, size_t nX,
    size_t nH = 25, armaVec d0 = armaVec(1).fill(-inf), ssidWt wt = NONE,
    data_t wtG0 = 0.0, std::vector<data_t>& t0 = DEFAULT_T0,
    data_t t_startSSID = -std::numeric_limits<data_t>::infinity(),
    data_t t_stopSSID = std::numeric_limits<data_t>::infinity(),
    bool assumeGaussian = true);

/**
 * Given dynamics and initial guess of output, solves for output matrix (C) by
 * maximizing Poisson maximum likelihood using Newton's method.
 *
 * @brief      Solve for output matrix by Poisson MLE
 *
 * @param      C     output matrix
 * @param      d     output bias
 * @param      x     state data
 * @param      n     measured count data
 * @param      dt    sample period
 */
void newtonSolveC_mle(armaMat& C, armaVec& d, std::vector<armaMat>& x,
                      std::vector<armaMat>& n, data_t& dt);

/**
 * Given dynamics and initial guess of output, solves for a scaling factor to
 * apply to output matrix (C) by maximizing Poisson maximum likelihood using
 * Newton's method.
 *
 * n.b., see Bussgang theorem (1952)
 *
 * @brief      Solve for rescaled output matrix by Poisson MLE
 *
 * @param      C     output matrix
 * @param      d     output bias
 * @param      x     state data
 * @param      n     measured count data
 * @param      dt    sample period
 *
 * @return     negative log-likelihood
 */
data_t newtonSolve_rescaleC_mle(armaMat& C, armaVec& d, std::vector<armaMat>& x,
                                std::vector<armaMat>& n, data_t& dt);

/**
 * @brief      Calculates output bias by Poisson MLE
 *
 * @param      C     output matrix
 * @param      d     output bias
 * @param      x     state data
 * @param      n     measured count data
 * @param      dt    sample period
 */
void calcD_mle(armaMat& C, armaVec& d, std::vector<armaMat>& x,
               std::vector<armaMat>& n, data_t& dt);

/**
 * @brief      Calculates output bias and rescaled output matrix by Poisson MLE
 *
 * @param      C     output matrix
 * @param      d     output bias
 * @param      x     state data
 * @param      n     measured count data
 * @param      dt    sample period
 *
 * @return     negative log-likelihood
 */
data_t calcD_newtonSolve_rescaleC_mle(armaMat& C, armaVec& d,
                                      std::vector<armaMat>& x,
                                      std::vector<armaMat>& n, data_t& dt);

/**
 * @brief      Calculates exponential output function by Poisson MLE
 *
 * @param      C     output matrix
 * @param      d     output bias
 * @param      x     state data
 * @param      n     measured count data
 * @param      dt    sample period
 */
void fitOutput_mle(armaMat& C, armaVec& d, std::vector<armaMat>& x,
                   std::vector<armaMat>& n, data_t& dt);
}  // namespace poisson
}  // namespace lds
#endif
