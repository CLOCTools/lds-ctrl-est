//===-- ldsCtrlEst_h/lds_sys.h - LDS ----------------------------*- C++ -*-===//
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
/// This file declares and partially defines the base type for linear dynamical
/// systems (`lds::sys_t`). Note that this class defines the underlying linear
/// dynamics, but does not have output functions.Gaussian- and Poisson-output
/// variants will be built upon this class.
///
/// \brief LDS base type
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_SYS_H
#define LDSCTRLEST_LDS_SYS_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {

/// Linear Dynamical System Type
class sys_t {
 public:
  /**
   * @brief      Constructs a new LDS.
   *
   * @param      nU    number if inputs (u)
   * @param      nX    number of state (x)
   * @param      dt    sample period
   * @param      p0    [optional] initial diagonal elements of state estimate
   *                   covariance (P)
   * @param      q0    [optional] initial diagonal elements of process noise
   *                   covariance (Q)
   */
  sys_t(std::size_t nU, std::size_t nX, data_t& dt, data_t& p0 = DEFAULT_P0,
        data_t& q0 = DEFAULT_Q0);
  sys_t& operator=(const sys_t& sys);

  /// Simulate a one-step prediction
  void simPredict();

  /// Get number of states
  size_t getNx() const { return nX; };

  /// Get number of inputs
  size_t getNu() const { return nU; };

  /// Get current input (u)
  armaVec getU() const { return u; };

  /// Get current state (x)
  armaVec getX() { return x; };

  /// Get input gain (g)
  armaVec getG() const { return g; };

  /// Get current process disturbance/bias (m)
  armaVec getM() const { return m; };

  /// Get state matrix (A)
  armaMat getA() const { return A; };

  /// Get input matrix (B)
  armaMat getB() const { return B; };

  /// Get process noise covariance (Q)
  armaMat getQ() const { return Q; };

  /// Get process noise covariance acting on disturbance evolution (Q_m)
  armaMat getQ_m() const { return Q_m; };

  /// Get covariance of state estimate (P)
  armaMat getP() const { return P; };

  /// Get covariance of process disturbance estimate (P_m)
  armaMat getP_m() const { return P_m; };

  /// Get initial state (x0)
  armaVec getX0() const { return x0; };

  /// Get initial covariance of state estimate (P0)
  armaMat getP0() const { return P0; };

  /// Get initial process disturbance (m0)
  armaVec getM0() const { return m0; };

  /// Get initial covariance of process disturbance estimate (P0_m)
  armaMat getP0_m() const { return P0_m; };

  /// Set dimensions of system
  void setDims(std::size_t& nU, std::size_t& nX);

  /// Set input (u)
  void setU(stdVec& uVec);
  /// Set input (u)
  void setU(armaVec& u);

  /// Set state matrix (A)
  void setA(stdVec& aVec);
  /// Set state matrix (A)
  void setA(armaMat& A);

  /// Set input matrix (B)
  void setB(stdVec& bVec);
  /// Set input matrix (B)
  void setB(armaMat& B);

  /// Set process disturbance (m)
  void setM(stdVec& mVec);
  /// Set process disturbance (m)
  void setM(armaVec& m);

  /// Set input gain (g)
  void setG(stdVec& gVec);
  /// Set input gain (g)
  void setG(armaVec& g);

  /// Set process noise covariance (Q)
  void setQ(stdVec& qVec);
  /// Set process noise covariance (Q)
  void setQ(armaMat& Q);

  /// Set process noise covariance of disturbance evoluation (Q_m)
  void setQ_m(stdVec& qmVec);
  /// Set process noise covariance of disturbance evoluation (Q_m)
  void setQ_m(armaMat& Q_m);

  /// Set initial state (x0)
  void setX0(stdVec& x0Vec);
  /// Set initial state (x0)
  void setX0(armaVec& x0);

  /// Set covariance of initial state (P0)
  void setP0(stdVec& p0Vec);
  /// Set covariance of initial state (P0)
  void setP0(armaMat& P0);

  /// Set covariance of initial process disturbance (P0_m)
  void setP0_m(stdVec& p0mVec);
  /// Set covariance of initial process disturbance (P0_m)
  void setP0_m(armaMat& P0_m);

  /// Reset system variables
  void reset();

  /// Print system variables to stdout
  void printSys();

  bool adaptM;

 protected:
  armaVec u;    ///< input
  armaVec x;    ///< state
  armaMat P;    ///< covariance of state estimate
  armaVec m;    ///< process disturbance
  armaMat P_m;  ///< covariance of disturbance estimate

  // Parameters:
  armaVec x0;    ///< initial state
  armaMat P0;    ///< covariance of initial state estimate
  armaVec m0;    ///< initial process disturbance
  armaMat P0_m;  ///< covariance of initial disturbance estimate

  armaMat A;    ///< state matrix
  armaMat B;    ///< input matrix
  armaVec g;    ///< input gain
  armaMat Q;    ///< covariance of process noise
  armaMat Q_m;  ///< covoariance of disturbance evolution process

  // it should be safe for dt to be a reference. I should not need to control
  // what the set behavior is.
  data_t& dt;  ///< sample period
  data_t& q0;  ///< default process noise covariance
  data_t& p0;  ///< default state estimate covariance

  // dimensions
  std::size_t nX;  ///< number of states
  std::size_t nU;  ///< number of inputs
  bool szChanged;  ///< whether size of system changed (see `setDims`)

  // max val for elements of P before reset for numerical reasons...
  const data_t plim = 1e2;  ///< limit for state estimate covariance

  /// one-step prediction of state
  void predict();

  // TODO(mfbolus): these are very redundant.
  // Should be able to use templates in some way to make this less type-specific
  void reassign(armaVec& oldVar, armaVec& newVar, data_t defaultVal = 0);
  void reassign(armaVec& oldVar, stdVec& newVar, data_t defaultVal = 0);
  void reassign(armaSubVec& oldVar, armaVec& newVar, data_t defaultVal = 0);
  void reassign(armaSubVec& oldVar, stdVec& newVar, data_t defaultVal = 0);
  void reassign(armaMat& oldVar, armaMat& newVar, data_t defaultVal = 0);
  void reassign(armaMat& oldVar, stdVec& newVar, data_t defaultVal = 0);
  void reassign(armaSubMat& oldVar, armaMat& newVar, data_t defaultVal = 0);
  void reassign(armaSubMat& oldVar, stdVec& newVar, data_t defaultVal = 0);

  void limit(stdVec& x, data_t& lb, data_t& ub);
  void limit(armaVec& x, data_t& lb, data_t& ub);
  void limit(armaMat& x, data_t& lb, data_t& ub);

  /// Reset to default Q (identity matrix with diagonal elements q0)
  void defaultQ();

  /// check whether estimate covariance fals within plim upper bound
  void checkP();
};  // sys_t
}  // namespace lds

#endif
