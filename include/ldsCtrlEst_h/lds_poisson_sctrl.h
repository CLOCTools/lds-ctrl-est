//===-- ldsCtrlEst_h/lds_poisson_sctrl.h - Switched Controller --*- C++ -*-===//
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
/// This file declares and partially defines the type for switched feedback
/// control of a system approximated as multiple discrete Poisson-output linear
/// dynamical systems
/// (`lds::poisson::sctrl_t`).
///
/// \brief PLDS switched controller type
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_POISSON_SWITCHED_CTRL_H
#define LDSCTRLEST_LDS_POISSON_SWITCHED_CTRL_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace poisson {
/// Switched PLDS Controller Type
class sctrl_t : public ctrl_t {
 public:
  /**
   * @brief      Constructs a new switched PLDS controller.
   *
   * @param      nSys         number of systems
   * @param      nU           number of inputs
   * @param      nX           number of states
   * @param      nY           number of outputs
   * @param      uLB          lower bound on control (u)
   * @param      uUB          upper bound on control (u)
   * @param      dt           sample period
   * @param      p0           [optional] initial diagonal elements of state
   *                          estimate covariance (P)
   * @param      q0           [optional] initial diagonal elements of process
   *                          noise covariance (Q)
   * @param      controlType  [optional] control type bit mask
   */
  sctrl_t(std::size_t nSys, std::size_t nU, std::size_t nX, std::size_t nY,
          data_t& uLB, data_t& uUB, data_t& dt, data_t& p0 = DEFAULT_P0,
          data_t& q0 = DEFAULT_Q0, size_t controlType = 0);

  /// Set the parameters of selected sub-system
  /// using previously instantiated GLDS controllers.
  void setSystem(size_t sysIdx, ctrl_t& sys);

  // TODO(mfbolus):
  // This switchSystem(sysIdx) approach is wasteful but simple.
  // Would be faster to toggle between ctrl_t's and replace wrapper u, x, P, m,
  // P_m. However, would have to override or rewrite most controller
  // functionality.

  /// \brief Switch to a different underlying sub-system
  ///
  /// Switch to a different underlying sub-system by reassiging parameters at
  /// switch.
  void switchSystem(size_t sysIdx);

  /// Get the index of currently active sub-system
  size_t getSysIdx() const { return sysIdx; };

 protected:
  size_t nSys;  ///< number of systems
  std::vector<ctrl_t>
      systems;    ///< underlying sub-systems which are toggled between
  size_t sysIdx;  ///< current system index

 private:
  // make all parameter set* methods private and define them to do nothing.
  // force users to use setSystem, with pre-defined systems.

  // from lds_t
  void setDims(std::size_t& nU, std::size_t& nX){};
  void setU(stdVec& uVec){};
  void setU(armaVec& u){};
  void setA(stdVec& aVec){};
  void setA(armaMat& A){};
  void setB(stdVec& bVec){};
  void setB(armaMat& B){};
  void setM(stdVec& mVec){};
  void setM(armaVec& m){};
  // void setG(stdVec& gVec) {};
  // void setG(armaVec& g) {};
  void setQ(stdVec& qVec){};
  void setQ(armaMat& Q){};
  void setQ_m(stdVec& qmVec){};
  void setQ_m(armaMat& Q_m){};
  void setX0(stdVec& x0Vec){};
  void setX0(armaVec& x0){};
  void setP0(stdVec& p0Vec){};
  void setP0(armaMat& P0){};
  void setP0_m(stdVec& p0mVec){};
  void setP0_m(armaMat& P0_m){};

  // from sys_t
  void setC(stdVec& cVec){};
  void setC(armaMat& C){};
  void setD(stdVec& dVec){};
  void setD(armaVec& d){};

  // from ctrl_t
  // void setGDesign(stdVec& gVec) {};
  // void setGDesign(armaVec& g) {};
  void setKc_x(stdVec& Kc_xVec){};
  void setKc_x(armaVec& Kc_x){};
  void setKc_u(stdVec& Kc_uVec){};
  void setKc_u(armaVec& Kc_u){};
  void setKc_inty(stdVec& Kc_intyVec){};
  void setKc_inty(armaVec& Kc_inty){};
  void setControlType(size_t controlType);  // {};

  // // assuming I *want* to maintain same references
  // void setURef(stdVec& uRefVec) {};
  // void setURef(armaVec& uRef) {};
  // void setXRef(stdVec& xRefVec) {};
  // void setXRef(armaVec& xRef) {};
  // void setYRef(stdVec& yRefVec) {};
  // void setYRef(armaVec& yRef) {};
};
}  // namespace poisson
}  // namespace lds

#endif
