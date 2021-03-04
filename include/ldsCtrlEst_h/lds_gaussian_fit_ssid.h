//===-- ldsCtrlEst_h/lds_gaussian_fit_ssid.h - GLDS Fit (SSID) --*- C++ -*-===//
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
/// models are fit by a subspace identification (SSID) algorithm
/// (`lds::gaussian::ssidFit_t`).
///
/// References:
/// [1] van Overschee P, de Moore B. (1996) Subspace Identification for Linear
/// Systems. Boston: Springer.
///
/// \brief GLDS SSID fit type
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_GAUSSIAN_FIT_SSID_H
#define LDSCTRLEST_LDS_GAUSSIAN_FIT_SSID_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace gaussian {
///
/// @brief      GLDS SSID Fit Type
///
///             This type is used in the process of fitting GLDS models by
///             subspace identification (SSID).
///
class ssidFit_t : public fit_t {
 public:
  ssidFit_t() : fit_t(){};
  /**
   * @brief      Constructs a new GLDS SSID fit type.
   *
   * @param      A            state matrix
   * @param      B            input matrix
   * @param      g            input gain
   * @param      m            process disturbance
   * @param      Q            process noise covariance
   * @param      x0           initial state estimate
   * @param      P0           covariance of initial state estimate
   * @param      C            output matrix
   * @param      D            feedthrough matrix
   * @param      d            output bias
   * @param      R            output noise covariance
   * @param      dt           sample period
   * @param      t_startSSID  start time for the data used in SSID
   * @param      t_stopSSID   stop time for the data used in SSID
   * @param      singVals     singular values from SSID SVD step
   * @param      t0           start times for each trial of input/output data
   * @param      uTrain       training input data
   * @param      zTrain       training measurement data
   */
  ssidFit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q,
            armaVec& x0, armaMat& P0, armaMat& C, armaMat& D, armaVec& d,
            armaMat& R, data_t dt, data_t t_startSSID, data_t t_stopSSID,
            armaVec& singVals, std::vector<data_t>& t0,
            std::vector<armaMat>& uTrain, std::vector<armaMat>& zTrain);
  std::vector<data_t> t0;
  data_t t_startSSID;
  data_t t_stopSSID;
  armaVec singVals;
};

/**
 * Fit a GLDS model using subspace identification (SSID)
 *
 * References:
 *
 * van Overschee P, de Moor B. (1996) Subspace Identification for Linear
 * Systems.
 *
 * @brief      Fit a GLDS model using subspace identification (SSID)
 *
 * @param      u                inputs
 * @param      z                measurements
 * @param      dt               sample period
 * @param      nX               number of states
 * @param      nH               block-Hankel data matrix size
 * @param      d0               previous estimate of output bias (d)
 * @param      force_unitNormC  force fitting where C has unit norm
 * @param      wt               chosen weighting for SSID singular value \
 *                              decomposition (NONE, MOESP, CVA)
 * @param      wtG0             chosen weighting for minimizing model error at \
 *                              DC
 * @param      t0               start times for each trial of input/output data
 * @param      t_startSSID      start time for the data used in SSID
 * @param      t_stopSSID       stop time for the data used in SSID
 *
 * @return     the SSID-fit GLDS model.
 */
ssidFit_t ssidFit(std::vector<armaMat>& u, std::vector<armaMat>& z, data_t dt,
                  size_t nX, size_t nH = 25, armaVec d0 = armaVec(1).fill(-inf),
                  bool force_unitNormC = false, ssidWt wt = NONE,
                  data_t wtG0 = 0.0, std::vector<data_t>& t0 = DEFAULT_T0,
                  data_t t_startSSID = -std::numeric_limits<data_t>::infinity(),
                  data_t t_stopSSID = std::numeric_limits<data_t>::infinity());

/**
 * @brief      Using periods of silence in inputs (u), calculates the output \
 *             bias
 *
 * @param      n              measurements
 * @param      u              inputs
 * @param      dt             sample period
 * @param      tSilence       threshold on period of time that qualifies as
 *                            "silence"
 * @param      threshSilence  threshold on input amplitude u that qualifies as
 *                            "silence"
 *
 * @return     d   estimated output bias.
 */
armaVec calcD_silence(std::vector<armaMat>& n, std::vector<armaMat>& u,
                      data_t dt, data_t tSilence = 0.1,
                      data_t threshSilence = 0.001);

/**
 * @brief      Create block-hankel data matrix for N4SID method.
 *
 * @param      u     input data
 * @param      z     measurement data
 * @param      nH    block-Hankel matrix size
 * @param      D     block-Hankel data matrix
 */
void createHankelMat_n4sid(armaMat& u, armaMat& z, std::size_t& nH, armaMat& D);

/**
 * @brief      N4SID method of SSID
 *
 * @param      nX               number of states
 * @param      nU               number of inputs
 * @param      nY               number of outputs
 * @param      nH               block-Hankel matrix size
 * @param      uSSID            input data
 * @param      zSSID            measurement data
 * @param      A                state matrix
 * @param      B                input matrix
 * @param      C                output matrix
 * @param      D                feedthrough matrix
 * @param      Q                process noise covariance
 * @param      R                output noise covariance
 * @param      m                process disturbance
 * @param      d                output bias
 * @param      s                singular values
 * @param      force_unitNormC  whether to force C to be unit-norm
 * @param      wt               weighting method for SVD (`NONE`, `MOESP`,
 *                              `CVA`)
 * @param      wtG0             weight put on minimizing error at DC
 */
void ssid_n4sid(std::size_t& nX, std::size_t& nU, std::size_t& nY,
                std::size_t& nH, armaMat& uSSID, armaMat& zSSID, armaMat& A,
                armaMat& B, armaMat& C, armaMat& D, armaMat& Q, armaMat& R,
                armaVec& m, armaVec& d, armaVec& s, bool force_unitNormC,
                ssidWt wt, data_t wtG0);

/**
 * N4SID method of SSID
 *
 * This is a more direct implementation of van Overschee, de Moor (1996)
 *
 * @brief      N4SID method of SSID
 *
 * @param      nX     number of state
 * @param      nU     number of inputs
 * @param      nY     number of outputs
 * @param      nH     block-Hankel data matrix size
 * @param      uSSID  input data
 * @param      zSSID  measurement data
 * @param      A      state matrix
 * @param      B      input matrix
 * @param      C      output matrix
 * @param      D      feedthrough matrix
 * @param      Q      process noise covariance
 * @param      R      output noise covariance
 * @param      s      singular values
 * @param      wt     weighting method for SVD (`NONE`, `MOESP`, `CVA`)
 */
void ssid_n4sid_vanOverschee(std::size_t& nX, std::size_t& nU, std::size_t& nY,
                             std::size_t& nH, armaMat& uSSID, armaMat& zSSID,
                             armaMat& A, armaMat& B, armaMat& C, armaMat& D,
                             armaMat& Q, armaMat& R, armaVec& s, ssidWt wt);

/**
 * @brief      Recompute extended observability matrix, given A, C.
 *
 * @param      extObs  Extended observability matrix
 * @param      A       state matrix
 * @param      C       output matrix
 * @param      nY      number of outputs
 * @param      nH      block-Hankel data matrix size
 */
void recompute_extObs(armaMat& extObs, armaMat& A, armaMat& C, std::size_t& nY,
                      std::size_t& nH);

/**
 * @brief      LQ decomposition
 *
 * @param      L     lower triangle matrix
 * @param      Qt    orthonormal matrix (transposed cf QR decomp)
 * @param      X     matrix being decomposed
 */
void lq(armaMat& L, armaMat& Qt, armaMat& X);

/**
 * @brief      Calculate covariance matrix
 *
 * @param      A     some matrix
 * @param      B     some other matrix
 *
 * @return     covariance
 */
armaMat calcCov(armaMat& A, armaMat& B);

/**
Approximate sqrtmat by singular value decomposition.

svd(X) --> U * sqrt(s) * V' ~= sqrtmat(X)

@param      X     some matrix

@return     approximate sqrtmat(X)
*/
armaMat sqrtmat_svd(armaMat& X);
}  // namespace gaussian
}  // namespace lds
#endif
