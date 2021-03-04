//===-- ldsCtrlEst_h/lds_poisson_ctrl.h - PLDS Controller -------*- C++ -*-===//
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
/// This file declares and partially defines the type for feedback control of a
/// Poisson-output linear dynamical system (`lds::poisson::ctrl_t`). It inherits
/// functionality from the underlying PLDS model type (`lds::poisson::sys_t`),
/// including state estimation.
///
/// \brief PLDS controller type
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_POISSON_CTRL_H
#define LDSCTRLEST_LDS_POISSON_CTRL_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace poisson {
///
/// @brief      PLDS Controller Type
///
class ctrl_t : public sys_t {
 public:
  /**
   * @brief      Constructs a new PLDS controller.
   *
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
  ctrl_t(std::size_t nU, std::size_t nX, std::size_t nY, data_t& uLB,
         data_t& uUB, data_t& dt, data_t& p0 = DEFAULT_P0,
         data_t& q0 = DEFAULT_Q0, size_t controlType = 0);
  ctrl_t& operator=(const ctrl_t& sys);

  /*
   * Perform a single step of feedback control. Note that this performs control
   * in the log-output space which is linear given the choice of exponential
   * nonlinearity.
   *
   * n.b., Requires user to have set the controller's `xRef` and `uRef`
   * variables.
   *
   * @brief      log-linear feedback control (single-step)
   *
   * @param      z                 measurement
   * @param      gateCtrl          [optional] logical gate of control
   *                               (high=enabled; low=disabled)
   * @param      gateLock          [optional] logical gate of control signal
   *                               lock (if high, locks control at current
   *                               value)
   * @param      sigma_softStart   [optional] standard deviation (sigma) of a
   *                               Gaussian soft-start to control
   * @param      sigma_uNoise      [optional] standard deviation (sigma) of
   *                               Gaussian noise added on top of control signal
   * @param      resetAtCtrlOnset  [optional] whether to reset controller at
   *                               onset of a new control epoch (gateCtrl from
   *                               low to high)
   */
  void logLin_fbCtrl(armaVec& z, bool& gateCtrl = TRUE, bool& gateLock = FALSE,
                     data_t& sigma_softStart = DEFAULT_SOFTSTART,
                     data_t& sigma_uNoise = DATA_T_ZERO,
                     bool& resetAtCtrlOnset = TRUE);

  // Steady-state solution to log-linear control problem. Given user-supplied
  // yRef --> steady state sol'z [xRef, uRef] --> control

  /**
   * Assuming the objective is to track a reference output (`yRef`) at steady
   * state, perform a single step of control in the log-output space, which is
   * linear given the assumed exponential output nonlinearity.
   *
   * n.b., Requires user to have set the controller's `yRef` variable.
   *
   * n.b., If control is enabled (`gateCtrl=true`) but estimator is disabled
   * (`gateEst=true`), this function provides the calculated steady-state input
   * `uRef` in open loop.
   *
   * @brief      steady-state log-linear control (single step)
   *
   * @param      z                 measurement
   * @param      gateCtrl          [optional] logical gate of control
   *                               (high=enabled; low=disabled)
   * @param      gateEst           [optional] logical gate of estimator
   *                               (high=enabled; low=disabled)
   * @param      gateLock          [optional] logical gate of control signal
   *                               lock (if high, locks control at current
   *                               value)
   * @param      sigma_softStart   [optional] standard deviation (sigma) of a
   *                               Gaussian soft-start to control
   * @param      sigma_uNoise      [optional] standard deviation (sigma) of
   *                               Gaussian noise added on top of control
   * @param      resetAtCtrlOnset  [optional] whether to reset controller at
   *                               onset of a new control epoch (gateCtrl from
   *                               low to high)
   */
  void steadyState_logLin_fbCtrl(armaVec& z, bool& gateCtrl = TRUE,
                                 bool& gateEst = TRUE, bool& gateLock = FALSE,
                                 data_t& sigma_softStart = DEFAULT_SOFTSTART,
                                 data_t& sigma_uNoise = DATA_T_ZERO,
                                 bool& resetAtCtrlOnset = TRUE);

  /// Calculate steady-state setpoint [`xRef`; `uRef`], given an output
  /// reference `yRef`.
  void calc_ssSetPt();

  /// Print system variables
  void printSys();

  // set methods
  // make sure to override the setU behavior inherited from sys_t!
  // cannot set u for a controller, as it generates its own.
  /// Set input (u)
  void setU(stdVec& uVec);
  /// Set input (u)
  void setU(armaVec& u);

  /// Set input gain (g)
  void setG(stdVec& gVec);
  /// Set input gain (g)
  void setG(armaVec& g);

  /// Set input gain used in controller design (gDesign)
  void setGDesign(stdVec& gVec);
  /// Set input gain used in controller design (gDesign)
  void setGDesign(armaVec& g);

  /// Set reference input (uRef)
  void setURef(stdVec& uRefVec);
  /// Set reference input (uRef)
  void setURef(armaVec& uRef);

  /// Set reference state (xRef)
  void setXRef(stdVec& xRefVec);
  /// Set reference state (xRef)
  void setXRef(armaVec& xRef);

  /// Set reference output (yRef)
  void setYRef(stdVec& yRefVec);
  /// Set reference output (yRef)
  void setYRef(armaVec& yRef);

  /// Set state controller gain (Kc_x)
  void setKc_x(stdVec& Kc_xVec);
  /// Set state controller gain (Kc_x)
  void setKc_x(armaMat& Kc_x);

  /// Set input controller gain (Kc_u)
  void setKc_u(stdVec& Kc_uVec);
  /// Set input controller gain (Kc_u)
  void setKc_u(armaMat& Kc_u);

  /// Set integral controller gain (Kc_inty)
  void setKc_inty(stdVec& Kc_intyVec);
  /// Set integral controller gain (Kc_inty)
  void setKc_inty(armaMat& Kc_inty);

  /// Set controller type
  void setControlType(size_t controlType);
  /// Set time constant of anti-integral-windup
  void setTauAntiWindup(data_t& tau);

  /// Get input controller gain (Kc_u)
  armaMat getKc_u() const { return Kc_u; };
  /// Get input controller gain (Kc_u)
  armaMat getKc_x() const { return Kc_x; };
  /// Get integral controller gain (Kc_inty)
  armaMat getKc_inty() const { return Kc_inty; };
  /// Get input gain used in controller design (gDesign)
  armaVec getGDesign() const { return gDesign; };
  /// Get reference input (uRef)
  armaVec getURef() const { return uRef; };
  /// Get reference state (xRef)
  armaVec getXRef() const { return xRef; };
  /// Get reference log-output (logyRef)
  armaVec getLogYRef() const { return logyRef; };
  /// Get reference output (yRef)
  armaVec getYRef() const { return yRef; };
  /// Get log-output (logy)
  armaVec getLogY() const { return logy; };
  /// Get integrated error (intE)
  armaVec getIntE() const { return intE; };
  /// Get controller type
  size_t getControlType() const { return controlType; };

  /// Reset system variables
  void reset();

 protected:
  armaVec gDesign;  ///< input gain of the system used for controller design

  //  reference signals
  armaVec uRef;       ///< reference input
  armaVec uRef_prev;  ///< reference input at previous time step
  armaVec xRef;       ///< reference state
  armaVec logyRef;    ///< reference log-output
  armaVec yRef;       ///< reference output

  // Controller gains
  armaMat Kc_x;  ///< state controller gain
  armaMat
      Kc_u;  ///<  input controller gain (when control aims to minimize deltaU)
  armaMat Kc_inty;  ///< integral controller gain (log-linear)

  // control after g inversion (partial fb linearization)
  // do not need set methods for these.
  armaVec duRef;
  armaVec dvRef;
  armaVec vRef;
  armaVec dv;
  armaVec v;  ///< Control after g inversion (e.g., control in physical units)

  // integral error
  // do not need set method for this
  armaVec intE;            ///< integrated error (log-output)
  armaVec intE_awuAdjust;  ///< anti-windup adjustment to intE
  armaVec uSat;            ///< control signal after saturation (for antiWindup)

  bool gateCtrl_prev;
  bool gateLock_prev;

  data_t yRefLB;  ///< lower bound on yRef (to avoid numerical log(0) issue)

  // I think it should be safe to have references here bc there is nothing I
  // need to do (like reset vars) when it changes...
  bool uSaturated;  ///< whether control signal has reached saturation limits

  data_t& uLB;  ///< lower bound on control
  data_t& uUB;  ///< upper bound on control

  data_t tauAntiWindup;  ///< antiwindup time constant
  data_t kAntiWindup;

  /// Protect against integral windup
  ///
  /// Reference:
  ///
  /// Astroem KJ, Rundqwist L. (1989) Integrator Windup and How to Avoid It.
  /// 1989 ACC.
  ///
  /// @brief      Protect against integral windup
  ///
  void antiWindup();

  data_t t_since_ctrl_onset;  ///< time since control epoch onset

  size_t controlType;  ///< controller type

 private:
  void calc_logLinCtrl(bool& gateCtrl, bool& gateEst, bool& gateLock,
                       data_t& sigma_softStart, data_t& sigma_uNoise,
                       bool& resetAtCtrlOnset);
};
}  // namespace poisson
}  // namespace lds

#include "lds_poisson_sctrl.h"

#endif
