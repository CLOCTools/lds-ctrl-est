---
title: ldsCtrlEst_h/lds_poisson_ctrl.h
summary: PLDS controller type. 

---

# ldsCtrlEst_h/lds_poisson_ctrl.h

PLDS controller type.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/ldsctrlest/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |
| **[lds::poisson](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/)** <br>Linear Dynamical Systems with Poisson observations.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::poisson::ctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/)** <br>PLDS Controller Type.  |

## Detailed Description



This file declares and partially defines the type for feedback control of a Poisson-output linear dynamical system (`[lds::poisson::ctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/)`). It inherits functionality from the underlying PLDS model type (`[lds::poisson::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/)`), including state estimation. 





## Source code

```cpp
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
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_POISSON_CTRL_H
#define LDSCTRLEST_LDS_POISSON_CTRL_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace poisson {
class ctrl_t : public sys_t {
 public:
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

  void steadyState_logLin_fbCtrl(armaVec& z, bool& gateCtrl = TRUE,
                                 bool& gateEst = TRUE, bool& gateLock = FALSE,
                                 data_t& sigma_softStart = DEFAULT_SOFTSTART,
                                 data_t& sigma_uNoise = DATA_T_ZERO,
                                 bool& resetAtCtrlOnset = TRUE);

  void calc_ssSetPt();

  void printSys();

  // set methods
  // make sure to override the setU behavior inherited from sys_t!
  // cannot set u for a controller, as it generates its own.
  void setU(stdVec& uVec);
  void setU(armaVec& u);

  void setG(stdVec& gVec);
  void setG(armaVec& g);

  void setGDesign(stdVec& gVec);
  void setGDesign(armaVec& g);

  void setURef(stdVec& uRefVec);
  void setURef(armaVec& uRef);

  void setXRef(stdVec& xRefVec);
  void setXRef(armaVec& xRef);

  void setYRef(stdVec& yRefVec);
  void setYRef(armaVec& yRef);

  void setKc_x(stdVec& Kc_xVec);
  void setKc_x(armaMat& Kc_x);

  void setKc_u(stdVec& Kc_uVec);
  void setKc_u(armaMat& Kc_u);

  void setKc_inty(stdVec& Kc_intyVec);
  void setKc_inty(armaMat& Kc_inty);

  void setControlType(size_t controlType);
  void setTauAntiWindup(data_t& tau);

  armaMat getKc_u() const { return Kc_u; };
  armaMat getKc_x() const { return Kc_x; };
  armaMat getKc_inty() const { return Kc_inty; };
  armaVec getGDesign() const { return gDesign; };
  armaVec getURef() const { return uRef; };
  armaVec getXRef() const { return xRef; };
  armaVec getLogYRef() const { return logyRef; };
  armaVec getYRef() const { return yRef; };
  armaVec getLogY() const { return logy; };
  armaVec getIntE() const { return intE; };
  size_t getControlType() const { return controlType; };

  void reset();

 protected:
  armaVec gDesign;  

  //  reference signals
  armaVec uRef;       
  armaVec uRef_prev;  
  armaVec xRef;       
  armaVec logyRef;    
  armaVec yRef;       

  // Controller gains
  armaMat Kc_x;  
  armaMat
      Kc_u;  
  armaMat Kc_inty;  

  // control after g inversion (partial fb linearization)
  // do not need set methods for these.
  armaVec duRef;
  armaVec dvRef;
  armaVec vRef;
  armaVec dv;
  armaVec v;  

  // integral error
  // do not need set method for this
  armaVec intE;            
  armaVec intE_awuAdjust;  
  armaVec uSat;            

  bool gateCtrl_prev;
  bool gateLock_prev;

  data_t yRefLB;  

  // I think it should be safe to have references here bc there is nothing I
  // need to do (like reset vars) when it changes...
  bool uSaturated;  

  data_t& uLB;  
  data_t& uUB;  

  data_t tauAntiWindup;  
  data_t kAntiWindup;

  void antiWindup();

  data_t t_since_ctrl_onset;  

  size_t controlType;  

 private:
  void calc_logLinCtrl(bool& gateCtrl, bool& gateEst, bool& gateLock,
                       data_t& sigma_softStart, data_t& sigma_uNoise,
                       bool& resetAtCtrlOnset);
};
}  // namespace poisson
}  // namespace lds

#include "lds_poisson_sctrl.h"

#endif
```


-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
