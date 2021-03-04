---
title: lds::poisson::sctrl_t
summary: Switched PLDS Controller Type. 

---

# lds::poisson::sctrl_t



Switched PLDS Controller Type. 
<br /> `#include <lds_poisson_sctrl.h>`

Inherits from [lds::poisson::ctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/), [lds::poisson::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/), [lds::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[sctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sctrl__t/#function-sctrl_t)**(std::size_t nSys, std::size_t nU, std::size_t nX, std::size_t nY, data_t & uLB, data_t & uUB, data_t & dt, data_t & p0 =DEFAULT_P0, data_t & q0 =DEFAULT_Q0, size_t controlType =0)<br>Constructs a new switched PLDS controller.  |
| void | **[setSystem](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sctrl__t/#function-setsystem)**(size_t sysIdx, [ctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/) & sys) |
| void | **[switchSystem](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sctrl__t/#function-switchsystem)**(size_t sysIdx)<br>Switch to a different underlying sub-system.  |
| size_t | **[getSysIdx](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sctrl__t/#function-getsysidx)**() const<br>Get the index of currently active sub-system.  |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| size_t | **[nSys](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sctrl__t/#variable-nsys)** <br>number of systems  |
| std::vector< [ctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/) > | **[systems](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sctrl__t/#variable-systems)** <br>underlying sub-systems which are toggled between  |
| size_t | **[sysIdx](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sctrl__t/#variable-sysidx)** <br>current system index  |

## Additional inherited members

**Public Functions inherited from [lds::poisson::ctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/)**

|                | Name           |
| -------------- | -------------- |
| | **[ctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-ctrl_t)**(std::size_t nU, std::size_t nX, std::size_t nY, data_t & uLB, data_t & uUB, data_t & dt, data_t & p0 =DEFAULT_P0, data_t & q0 =DEFAULT_Q0, size_t controlType =0)<br>Constructs a new PLDS controller.  |
| [ctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/) & | **[operator=](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-operator=)**(const [ctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/) & sys) |
| void | **[logLin_fbCtrl](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-loglin_fbctrl)**(armaVec & z, bool & gateCtrl =TRUE, bool & gateLock =FALSE, data_t & sigma_softStart =DEFAULT_SOFTSTART, data_t & sigma_uNoise =DATA_T_ZERO, bool & resetAtCtrlOnset =TRUE) |
| void | **[steadyState_logLin_fbCtrl](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-steadystate_loglin_fbctrl)**(armaVec & z, bool & gateCtrl =TRUE, bool & gateEst =TRUE, bool & gateLock =FALSE, data_t & sigma_softStart =DEFAULT_SOFTSTART, data_t & sigma_uNoise =DATA_T_ZERO, bool & resetAtCtrlOnset =TRUE)<br>steady-state log-linear control (single step)  |
| void | **[calc_ssSetPt](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-calc_sssetpt)**() |
| void | **[printSys](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-printsys)**()<br>Print system variables.  |
| void | **[setG](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-setg)**(stdVec & gVec)<br>Set input gain (g)  |
| void | **[setG](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-setg)**(armaVec & g)<br>Set input gain (g)  |
| void | **[setGDesign](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-setgdesign)**(stdVec & gVec)<br>Set input gain used in controller design (gDesign)  |
| void | **[setGDesign](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-setgdesign)**(armaVec & g)<br>Set input gain used in controller design (gDesign)  |
| void | **[setURef](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-seturef)**(stdVec & uRefVec)<br>Set reference input (uRef)  |
| void | **[setURef](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-seturef)**(armaVec & uRef)<br>Set reference input (uRef)  |
| void | **[setXRef](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-setxref)**(stdVec & xRefVec)<br>Set reference state (xRef)  |
| void | **[setXRef](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-setxref)**(armaVec & xRef)<br>Set reference state (xRef)  |
| void | **[setYRef](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-setyref)**(stdVec & yRefVec)<br>Set reference output (yRef)  |
| void | **[setYRef](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-setyref)**(armaVec & yRef)<br>Set reference output (yRef)  |
| void | **[setTauAntiWindup](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-settauantiwindup)**(data_t & tau)<br>Set time constant of anti-integral-windup.  |
| armaMat | **[getKc_u](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-getkc_u)**() const<br>Get input controller gain (Kc_u)  |
| armaMat | **[getKc_x](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-getkc_x)**() const<br>Get input controller gain (Kc_u)  |
| armaMat | **[getKc_inty](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-getkc_inty)**() const<br>Get integral controller gain (Kc_inty)  |
| armaVec | **[getGDesign](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-getgdesign)**() const<br>Get input gain used in controller design (gDesign)  |
| armaVec | **[getURef](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-geturef)**() const<br>Get reference input (uRef)  |
| armaVec | **[getXRef](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-getxref)**() const<br>Get reference state (xRef)  |
| armaVec | **[getLogYRef](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-getlogyref)**() const<br>Get reference log-output (logyRef)  |
| armaVec | **[getYRef](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-getyref)**() const<br>Get reference output (yRef)  |
| armaVec | **[getLogY](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-getlogy)**() const<br>Get log-output (logy)  |
| armaVec | **[getIntE](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-getinte)**() const<br>Get integrated error (intE)  |
| size_t | **[getControlType](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-getcontroltype)**() const<br>Get controller type.  |
| void | **[reset](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-reset)**()<br>Reset system variables.  |

**Protected Functions inherited from [lds::poisson::ctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/)**

|                | Name           |
| -------------- | -------------- |
| void | **[antiWindup](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#function-antiwindup)**()<br>Protect against integral windup.  |

**Protected Attributes inherited from [lds::poisson::ctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/)**

|                | Name           |
| -------------- | -------------- |
| armaVec | **[gDesign](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-gdesign)** <br>input gain of the system used for controller design  |
| armaVec | **[uRef](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-uref)** <br>reference input  |
| armaVec | **[uRef_prev](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-uref_prev)** <br>reference input at previous time step  |
| armaVec | **[xRef](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-xref)** <br>reference state  |
| armaVec | **[logyRef](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-logyref)** <br>reference log-output  |
| armaVec | **[yRef](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-yref)** <br>reference output  |
| armaMat | **[Kc_x](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-kc_x)** <br>state controller gain  |
| armaMat | **[Kc_u](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-kc_u)** <br>input controller gain (when control aims to minimize deltaU)  |
| armaMat | **[Kc_inty](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-kc_inty)** <br>integral controller gain (log-linear)  |
| armaVec | **[duRef](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-duref)**  |
| armaVec | **[dvRef](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-dvref)**  |
| armaVec | **[vRef](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-vref)**  |
| armaVec | **[dv](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-dv)**  |
| armaVec | **[v](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-v)** <br>Control after g inversion (e.g., control in physical units)  |
| armaVec | **[intE](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-inte)** <br>integrated error (log-output)  |
| armaVec | **[intE_awuAdjust](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-inte_awuadjust)** <br>anti-windup adjustment to intE  |
| armaVec | **[uSat](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-usat)** <br>control signal after saturation (for antiWindup)  |
| bool | **[gateCtrl_prev](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-gatectrl_prev)**  |
| bool | **[gateLock_prev](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-gatelock_prev)**  |
| data_t | **[yRefLB](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-yreflb)** <br>lower bound on yRef (to avoid numerical log(0) issue)  |
| bool | **[uSaturated](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-usaturated)** <br>whether control signal has reached saturation limits  |
| data_t & | **[uLB](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-ulb)** <br>lower bound on control  |
| data_t & | **[uUB](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-uub)** <br>upper bound on control  |
| data_t | **[tauAntiWindup](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-tauantiwindup)** <br>antiwindup time constant  |
| data_t | **[kAntiWindup](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-kantiwindup)**  |
| data_t | **[t_since_ctrl_onset](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-t_since_ctrl_onset)** <br>time since control epoch onset  |
| size_t | **[controlType](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/#variable-controltype)** <br>controller type  |

**Public Functions inherited from [lds::poisson::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/)**

|                | Name           |
| -------------- | -------------- |
| void | **[filter](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-filter)**(armaVec & u_tm1, armaVec & z_t)<br>Filter data to produce causal state estimates.  |
| void | **[filter](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-filter)**(armaVec & z)<br>Filter data to produce causal state estimates.  |
| void | **[simMeasurement](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-simmeasurement)**(armaVec & z)<br>Simulate one step of the model and produce a measurement.  |
| | **[sys_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-sys_t)**(std::size_t nU, std::size_t nX, std::size_t nY, data_t & dt, data_t & p0 =DEFAULT_P0, data_t & q0 =DEFAULT_Q0)<br>Constructs a new PLDS.  |
| [sys_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/) & | **[operator=](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-operator=)**(const [sys_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/) & sys) |
| size_t | **[getNy](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-getny)**() const<br>Get number of outputs (y)  |
| armaMat | **[getC](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-getc)**() const<br>Get output matrix (C)  |
| armaVec | **[getD](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-getd)**() const<br>Get output bias (D)  |
| armaVec | **[getY](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-gety)**() const<br>Get output (y)  |
| armaVec | **[getZ](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-getz)**() const<br>Get measurement (z)  |
| void | **[setZ](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-setz)**(stdVec & zVec)<br>Set measurement (z)  |
| void | **[setZ](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-setz)**(armaVec & z)<br>Set measurement (z)  |
| void | **[reset](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-reset)**()<br>Reset system variables.  |
| void | **[printSys](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-printsys)**()<br>Print system variables to stdout.  |

**Protected Functions inherited from [lds::poisson::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/)**

|                | Name           |
| -------------- | -------------- |
| void | **[predict](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-predict)**()<br>One-step prediction.  |
| void | **[h](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-h)**()<br>System output function.  |

**Protected Attributes inherited from [lds::poisson::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/)**

|                | Name           |
| -------------- | -------------- |
| std::size_t | **[nY](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#variable-ny)** <br>number of outputs  |
| armaMat | **[C](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#variable-c)** <br>output matrix  |
| armaVec | **[d](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#variable-d)** <br>output bias  |
| armaVec | **[y](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#variable-y)** <br>output  |
| armaVec | **[logy](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#variable-logy)** <br>logarithm of output  |
| armaVec | **[z](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#variable-z)** <br>measurement  |
| armaMat | **[diag_y](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#variable-diag_y)** <br>diagonal matrix with elements y  |
| armaVec | **[chance](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#variable-chance)** <br>p.r. number for rolling dice if simulating data  |

**Public Functions inherited from [lds::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/)**

|                | Name           |
| -------------- | -------------- |
| | **[sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-sys_t)**(std::size_t nU, std::size_t nX, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & dt, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & p0 =DEFAULT_P0, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & q0 =DEFAULT_Q0)<br>Constructs a new LDS.  |
| [sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/) & | **[operator=](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-operator=)**(const [sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/) & sys) |
| void | **[simPredict](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-simpredict)**()<br>Simulate a one-step prediction.  |
| size_t | **[getNx](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getnx)**() const<br>Get number of states.  |
| size_t | **[getNu](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getnu)**() const<br>Get number of inputs.  |
| armaVec | **[getU](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getu)**() const<br>Get current input (u)  |
| armaVec | **[getX](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getx)**()<br>Get current state (x)  |
| armaVec | **[getG](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getg)**() const<br>Get input gain (g)  |
| armaVec | **[getM](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getm)**() const<br>Get current process disturbance/bias (m)  |
| armaMat | **[getA](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-geta)**() const<br>Get state matrix (A)  |
| armaMat | **[getB](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getb)**() const<br>Get input matrix (B)  |
| armaMat | **[getQ](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getq)**() const<br>Get process noise covariance (Q)  |
| armaMat | **[getQ_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getq_m)**() const<br>Get process noise covariance acting on disturbance evolution (Q_m)  |
| armaMat | **[getP](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getp)**() const<br>Get covariance of state estimate (P)  |
| armaMat | **[getP_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getp_m)**() const<br>Get covariance of process disturbance estimate (P_m)  |
| armaVec | **[getX0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getx0)**() const<br>Get initial state (x0)  |
| armaMat | **[getP0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getp0)**() const<br>Get initial covariance of state estimate (P0)  |
| armaVec | **[getM0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getm0)**() const<br>Get initial process disturbance (m0)  |
| armaMat | **[getP0_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getp0_m)**() const<br>Get initial covariance of process disturbance estimate (P0_m)  |
| void | **[setG](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setg)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & gVec)<br>Set input gain (g)  |
| void | **[setG](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setg)**(armaVec & g)<br>Set input gain (g)  |
| void | **[reset](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-reset)**()<br>Reset system variables.  |
| void | **[printSys](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-printsys)**()<br>Print system variables to stdout.  |

**Protected Functions inherited from [lds::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/)**

|                | Name           |
| -------------- | -------------- |
| void | **[predict](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-predict)**()<br>one-step prediction of state  |
| void | **[reassign](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-reassign)**(armaVec & oldVar, armaVec & newVar, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) defaultVal =0) |
| void | **[reassign](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-reassign)**(armaVec & oldVar, [stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & newVar, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) defaultVal =0) |
| void | **[reassign](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-reassign)**(armaSubVec & oldVar, armaVec & newVar, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) defaultVal =0) |
| void | **[reassign](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-reassign)**(armaSubVec & oldVar, [stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & newVar, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) defaultVal =0) |
| void | **[reassign](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-reassign)**(armaMat & oldVar, armaMat & newVar, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) defaultVal =0) |
| void | **[reassign](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-reassign)**(armaMat & oldVar, [stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & newVar, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) defaultVal =0) |
| void | **[reassign](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-reassign)**(armaSubMat & oldVar, armaMat & newVar, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) defaultVal =0) |
| void | **[reassign](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-reassign)**(armaSubMat & oldVar, [stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & newVar, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) defaultVal =0) |
| void | **[limit](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-limit)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & x, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & lb, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & ub) |
| void | **[limit](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-limit)**(armaVec & x, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & lb, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & ub) |
| void | **[limit](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-limit)**(armaMat & x, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & lb, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & ub) |
| void | **[defaultQ](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-defaultq)**()<br>Reset to default Q (identity matrix with diagonal elements q0)  |
| void | **[checkP](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-checkp)**()<br>check whether estimate covariance fals within plim upper bound  |

**Public Attributes inherited from [lds::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/)**

|                | Name           |
| -------------- | -------------- |
| bool | **[adaptM](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-adaptm)**  |

**Protected Attributes inherited from [lds::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/)**

|                | Name           |
| -------------- | -------------- |
| armaVec | **[u](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-u)** <br>input  |
| armaVec | **[x](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-x)** <br>state  |
| armaMat | **[P](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-p)** <br>covariance of state estimate  |
| armaVec | **[m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-m)** <br>process disturbance  |
| armaMat | **[P_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-p_m)** <br>covariance of disturbance estimate  |
| armaVec | **[x0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-x0)** <br>initial state  |
| armaMat | **[P0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-p0)** <br>covariance of initial state estimate  |
| armaVec | **[m0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-m0)** <br>initial process disturbance  |
| armaMat | **[P0_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-p0_m)** <br>covariance of initial disturbance estimate  |
| armaMat | **[A](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-a)** <br>state matrix  |
| armaMat | **[B](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-b)** <br>input matrix  |
| armaVec | **[g](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-g)** <br>input gain  |
| armaMat | **[Q](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-q)** <br>covariance of process noise  |
| armaMat | **[Q_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-q_m)** <br>covoariance of disturbance evolution process  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & | **[dt](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-dt)** <br>sample period  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & | **[q0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-q0)** <br>default process noise covariance  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & | **[p0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-p0)** <br>default state estimate covariance  |
| std::size_t | **[nX](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-nx)** <br>number of states  |
| std::size_t | **[nU](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-nu)** <br>number of inputs  |
| bool | **[szChanged](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-szchanged)** <br>whether size of system changed (see `setDims`)  |
| const [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) | **[plim](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-plim)** <br>limit for state estimate covariance  |


---
---
## Public Function Details

### **sctrl_t**

```cpp
sctrl_t(
    std::size_t nSys,
    std::size_t nU,
    std::size_t nX,
    std::size_t nY,
    data_t & uLB,
    data_t & uUB,
    data_t & dt,
    data_t & p0 =DEFAULT_P0,
    data_t & q0 =DEFAULT_Q0,
    size_t controlType =0
)
```



**Parameters**:

  * **nSys** number of systems 
  * **nU** number of inputs 
  * **nX** number of states 
  * **nY** number of outputs 
  * **uLB** lower bound on control (u) 
  * **uUB** upper bound on control (u) 
  * **dt** sample period 
  * **p0** [optional] initial diagonal elements of state estimate covariance (P) 
  * **q0** [optional] initial diagonal elements of process noise covariance (Q) 
  * **controlType** [optional] control type bit mask 


---
### **setSystem**

```cpp
void setSystem(
    size_t sysIdx,
    ctrl_t & sys
)
```



Set the parameters of selected sub-system using previously instantiated GLDS controllers. 


---
### **switchSystem**

```cpp
void switchSystem(
    size_t sysIdx
)
```



Switch to a different underlying sub-system by reassiging parameters at switch. 


---
### **getSysIdx**

```cpp
inline size_t getSysIdx() const
```



---


## Protected Attribute Details

### **nSys**

```cpp
size_t nSys;
```



---
### **systems**

```cpp
std::vector< ctrl_t > systems;
```



---
### **sysIdx**

```cpp
size_t sysIdx;
```



---


-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
