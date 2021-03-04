---
title: lds::gaussian::ctrl_t
summary: GLDS Controller Type. 

---

# lds::gaussian::ctrl_t



GLDS Controller Type. 
<br /> `#include <lds_gaussian_ctrl.h>`

Inherits from [lds::gaussian::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/), [lds::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/)

Inherited by [lds::gaussian::sctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sctrl__t/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-ctrl_t)**(std::size_t nU, std::size_t nX, std::size_t nY, data_t & uLB, data_t & uUB, data_t & dt, data_t & p0 =DEFAULT_P0, data_t & q0 =DEFAULT_Q0, data_t & r0 =DEFAULT_R0, size_t controlType =0)<br>Constructs a new GLDS controller.  |
| [ctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/) & | **[operator=](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-operator=)**(const [ctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/) & sys) |
| void | **[fbCtrl](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-fbctrl)**(armaVec & z, bool & gateCtrl =TRUE, bool & gateLock =FALSE, data_t & sigma_softStart =DEFAULT_SOFTSTART, data_t & sigma_uNoise =DATA_T_ZERO, bool & resetAtCtrlOnset =TRUE, bool & doRecurse_Ke =TRUE)<br>feedback control (single step)  |
| void | **[steadyState_fbCtrl](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-steadystate_fbctrl)**(armaVec & z, bool & gateCtrl =TRUE, bool & gateEst =TRUE, bool & gateLock =FALSE, data_t & sigma_softStart =DEFAULT_SOFTSTART, data_t & sigma_uNoise =DATA_T_ZERO, bool & resetAtCtrlOnset =TRUE, bool & doRecurse_Ke =TRUE)<br>steady-state control (single step)  |
| void | **[calc_ssSetPt](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-calc_sssetpt)**() |
| void | **[printSys](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-printsys)**()<br>Print system variables to stdout.  |
| void | **[setDims](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-setdims)**(std::size_t & nU, std::size_t & nX, std::size_t & nY)<br>Set dimensions of system.  |
| void | **[setU](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-setu)**(stdVec & uVec)<br>Set input (u)  |
| void | **[setU](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-setu)**(armaVec & u)<br>Set input (u)  |
| void | **[setG](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-setg)**(stdVec & gVec)<br>Set input gain (g)  |
| void | **[setG](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-setg)**(armaVec & g)<br>Set input gain (g)  |
| void | **[setGDesign](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-setgdesign)**(stdVec & gVec)<br>Set input gain used in controller design (gDesign)  |
| void | **[setGDesign](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-setgdesign)**(armaVec & g)<br>Set input gain used in controller design (gDesign)  |
| void | **[setURef](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-seturef)**(stdVec & uRefVec)<br>Set reference input (uRef)  |
| void | **[setURef](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-seturef)**(armaVec & uRef)<br>Set reference input (uRef)  |
| void | **[setXRef](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-setxref)**(stdVec & xRefVec)<br>Set reference state (xRef)  |
| void | **[setXRef](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-setxref)**(armaVec & xRef)<br>Set reference state (xRef)  |
| void | **[setYRef](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-setyref)**(stdVec & yRefVec)<br>Set reference output (yRef)  |
| void | **[setYRef](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-setyref)**(armaVec & yRef)<br>Set reference output (yRef)  |
| void | **[setKc_x](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-setkc_x)**(stdVec & Kc_x_vec)<br>Set state controller gain (Kc_x)  |
| void | **[setKc_x](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-setkc_x)**(armaMat & Kc_x)<br>Set state controller gain (Kc_x)  |
| void | **[setKc_u](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-setkc_u)**(stdVec & Kc_u_vec)<br>Set input controller gain (Kc_u)  |
| void | **[setKc_u](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-setkc_u)**(armaMat & Kc_u)<br>Set input controller gain (Kc_u)  |
| void | **[setKc_inty](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-setkc_inty)**(stdVec & Kc_inty_vec)<br>Set integral controller gain (Kc_inty)  |
| void | **[setKc_inty](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-setkc_inty)**(armaMat & Kc_inty)<br>Set integral controller gain (Kc_inty)  |
| void | **[setControlType](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-setcontroltype)**(size_t controlType)<br>Set controller type.  |
| void | **[setTauAntiWindup](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-settauantiwindup)**(data_t & tau)<br>Set time constant of anti-integral-windup.  |
| armaMat | **[getKc_u](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-getkc_u)**() const<br>Get input controller gain (Kc_u)  |
| armaMat | **[getKc_x](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-getkc_x)**() const<br>Get state controller gain (Kc_x)  |
| armaMat | **[getKc_inty](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-getkc_inty)**() const<br>Get integral controller gain (Kc_inty)  |
| armaVec | **[getGDesign](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-getgdesign)**() const<br>Get input gain used in controller design (gDesign)  |
| armaVec | **[getURef](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-geturef)**() const<br>Get reference input (uRef)  |
| armaVec | **[getXRef](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-getxref)**() const<br>Get reference state (xRef)  |
| armaVec | **[getYRef](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-getyref)**() const<br>Get reference output (yRef)  |
| armaVec | **[getIntE](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-getinte)**() const<br>Get integrated error (intE)  |
| size_t | **[getControlType](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-getcontroltype)**() const<br>Get controller type.  |
| void | **[reset](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-reset)**()<br>Reset system variables.  |

## Protected Functions

|                | Name           |
| -------------- | -------------- |
| void | **[antiWindup](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#function-antiwindup)**()<br>Protect against integral windup.  |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| armaVec | **[gDesign](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-gdesign)** <br>input gain of the system used for controller design  |
| armaVec | **[uRef](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-uref)** <br>reference input  |
| armaVec | **[uRef_prev](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-uref_prev)** <br>reference input at previous time step  |
| armaVec | **[xRef](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-xref)** <br>reference state  |
| armaVec | **[yRef](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-yref)** <br>reference output  |
| armaMat | **[Kc_x](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-kc_x)** <br>state controller gain  |
| armaMat | **[Kc_u](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-kc_u)** <br>input controller gain (when control aims to minimize deltaU)  |
| armaMat | **[Kc_inty](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-kc_inty)** <br>integral controller gain  |
| armaVec | **[duRef](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-duref)**  |
| armaVec | **[dvRef](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-dvref)**  |
| armaVec | **[vRef](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-vref)**  |
| armaVec | **[dv](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-dv)**  |
| armaVec | **[v](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-v)** <br>Control after g inversion (e.g., control in physical units)  |
| armaVec | **[intE](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-inte)** <br>integrated error  |
| armaVec | **[intE_awuAdjust](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-inte_awuadjust)** <br>anti-windup adjustment to intE  |
| armaVec | **[uSat](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-usat)** <br>control signal after saturation (for antiWindup)  |
| bool | **[gateCtrl_prev](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-gatectrl_prev)**  |
| bool | **[gateLock_prev](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-gatelock_prev)**  |
| bool | **[uSaturated](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-usaturated)** <br>whether control signal has reached saturation limits  |
| data_t & | **[uLB](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-ulb)** <br>lower bound on control  |
| data_t & | **[uUB](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-uub)** <br>upper bound on control  |
| data_t | **[tauAntiWindup](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-tauantiwindup)** <br>antiwindup time constant  |
| data_t | **[kAntiWindup](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-kantiwindup)**  |
| data_t | **[t_since_ctrl_onset](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-t_since_ctrl_onset)** <br>time since control epoch onset  |
| size_t | **[controlType](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/#variable-controltype)** <br>controller type  |

## Additional inherited members

**Public Functions inherited from [lds::gaussian::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/)**

|                | Name           |
| -------------- | -------------- |
| void | **[filter](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-filter)**(armaVec & u_tm1, armaVec & z_t, bool doRecurse_Ke =true)<br>Filter data to produce causal state estimates.  |
| void | **[filter](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-filter)**(armaVec & z, bool doRecurse_Ke =true)<br>Filter data to produce causal state estimates.  |
| void | **[simMeasurement](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-simmeasurement)**(armaVec & z)<br>Simulate system measurement.  |
| | **[sys_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-sys_t)**(std::size_t nU, std::size_t nX, std::size_t nY, data_t & dt, data_t & p0 =DEFAULT_P0, data_t & q0 =DEFAULT_Q0, data_t & r0 =DEFAULT_R0)<br>Constructs a new GLDS.  |
| size_t | **[getNy](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-getny)**() const<br>Get number of outputs (y)  |
| armaMat | **[getC](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-getc)**() const<br>Get output matrix (C)  |
| armaVec | **[getD](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-getd)**() const<br>Get output bias (d)  |
| armaVec | **[getR](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-getr)**() const<br>Get output noise covariance (R)  |
| armaVec | **[getY](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-gety)**() const<br>Get output (y)  |
| armaVec | **[getZ](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-getz)**() const<br>Get measurement (z)  |
| armaMat | **[getKe](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-getke)**() const<br>Get estimator gain (Ke)  |
| armaMat | **[getKe_m](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-getke_m)**() const<br>Get estimator gain for process disturbance (Ke_m)  |
| void | **[setC](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-setc)**(stdVec & cVec)<br>Set output matrix (C)  |
| void | **[setC](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-setc)**(armaMat & C)<br>Set output matrix (C)  |
| void | **[setD](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-setd)**(stdVec & dVec)<br>Set output bias (d)  |
| void | **[setD](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-setd)**(armaVec & d)<br>Set output bias (d)  |
| void | **[setR](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-setr)**(stdVec & rVec)<br>Set output noise covariance (R)  |
| void | **[setR](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-setr)**(armaMat & R)<br>Set output noise covariance (R)  |
| void | **[setZ](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-setz)**(stdVec & zVec)<br>Set measurement (z)  |
| void | **[setZ](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-setz)**(armaVec & z)<br>Set measurement (z)  |
| void | **[setKe](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-setke)**(stdVec & keVec)<br>Set estimator gain (Ke)  |
| void | **[setKe](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-setke)**(armaMat & Ke)<br>Set estimator gain (Ke)  |
| void | **[setKe_m](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-setke_m)**(stdVec & kemVec)<br>Set disturbance estimator gain (Ke_m)  |
| void | **[setKe_m](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-setke_m)**(armaMat & Ke_m)<br>Set disturbance estimator gain (Ke_m)  |

**Protected Functions inherited from [lds::gaussian::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/)**

|                | Name           |
| -------------- | -------------- |
| void | **[recurse_Ke](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-recurse_ke)**()<br>Recursively recalculate Kalman estimator gain (Ke)  |
| void | **[predict](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-predict)**()<br>One-step prediction.  |
| void | **[h](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-h)**()<br>System output function.  |
| void | **[defaultR](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#function-defaultr)**()<br>Reset to default R (identity matrix with diagonal elements r0)  |

**Protected Attributes inherited from [lds::gaussian::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/)**

|                | Name           |
| -------------- | -------------- |
| std::size_t | **[nY](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#variable-ny)** <br>number of outputs  |
| armaMat | **[C](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#variable-c)** <br>output matrix  |
| armaVec | **[d](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#variable-d)** <br>output bias  |
| armaMat | **[R](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#variable-r)** <br>covariance of output noise  |
| data_t & | **[r0](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#variable-r0)** <br>default values for R  |
| armaVec | **[y](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#variable-y)** <br>output  |
| armaVec | **[z](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#variable-z)** <br>measurement  |
| armaMat | **[Ke](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#variable-ke)** <br>state estimator gain  |
| armaMat | **[Ke_m](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/#variable-ke_m)** <br>disturbance estimator gain  |

**Public Functions inherited from [lds::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/)**

|                | Name           |
| -------------- | -------------- |
| | **[sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-sys_t)**(std::size_t nU, std::size_t nX, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & dt, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & p0 =DEFAULT_P0, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & q0 =DEFAULT_Q0)<br>Constructs a new LDS.  |
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
| void | **[setA](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-seta)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & aVec)<br>Set state matrix (A)  |
| void | **[setA](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-seta)**(armaMat & A)<br>Set state matrix (A)  |
| void | **[setB](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setb)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & bVec)<br>Set input matrix (B)  |
| void | **[setB](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setb)**(armaMat & B)<br>Set input matrix (B)  |
| void | **[setM](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setm)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & mVec)<br>Set process disturbance (m)  |
| void | **[setM](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setm)**(armaVec & m)<br>Set process disturbance (m)  |
| void | **[setQ](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setq)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & qVec)<br>Set process noise covariance (Q)  |
| void | **[setQ](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setq)**(armaMat & Q)<br>Set process noise covariance (Q)  |
| void | **[setQ_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setq_m)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & qmVec)<br>Set process noise covariance of disturbance evoluation (Q_m)  |
| void | **[setQ_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setq_m)**(armaMat & Q_m)<br>Set process noise covariance of disturbance evoluation (Q_m)  |
| void | **[setX0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setx0)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & x0Vec)<br>Set initial state (x0)  |
| void | **[setX0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setx0)**(armaVec & x0)<br>Set initial state (x0)  |
| void | **[setP0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setp0)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & p0Vec)<br>Set covariance of initial state (P0)  |
| void | **[setP0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setp0)**(armaMat & P0)<br>Set covariance of initial state (P0)  |
| void | **[setP0_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setp0_m)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & p0mVec)<br>Set covariance of initial process disturbance (P0_m)  |
| void | **[setP0_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setp0_m)**(armaMat & P0_m)<br>Set covariance of initial process disturbance (P0_m)  |

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

### **ctrl_t**

```cpp
ctrl_t(
    std::size_t nU,
    std::size_t nX,
    std::size_t nY,
    data_t & uLB,
    data_t & uUB,
    data_t & dt,
    data_t & p0 =DEFAULT_P0,
    data_t & q0 =DEFAULT_Q0,
    data_t & r0 =DEFAULT_R0,
    size_t controlType =0
)
```



**Parameters**:

  * **nU** number of inputs 
  * **nX** number of states 
  * **nY** number of outputs 
  * **uLB** lower bound on control (u) 
  * **uUB** upper bound on control (u) 
  * **dt** sample period 
  * **p0** [optional] initial diagonal elements of state estimate covariance (P) 
  * **q0** [optional] initial diagonal elements of process noise covariance (Q) 
  * **r0** [optional] initial diagonal elements of output noise covariance (R) 
  * **controlType** [optional] control type bit mask 


---
### **operator=**

```cpp
ctrl_t & operator=(
    const ctrl_t & sys
)
```



---
### **fbCtrl**

```cpp
void fbCtrl(
    armaVec & z,
    bool & gateCtrl =TRUE,
    bool & gateLock =FALSE,
    data_t & sigma_softStart =DEFAULT_SOFTSTART,
    data_t & sigma_uNoise =DATA_T_ZERO,
    bool & resetAtCtrlOnset =TRUE,
    bool & doRecurse_Ke =TRUE
)
```



**Parameters**:

  * **z** measurement 
  * **gateCtrl** [optional] logical gate of control (high=enabled; low=disabled) 
  * **gateLock** [optional] logical gate of control signal lock (if high, locks control at current value) 
  * **sigma_softStart** [optional] standard deviation (sigma) of a Gaussian soft-start to control 
  * **sigma_uNoise** [optional] standard deviation (sigma) of Gaussian noise added on top of control signal 
  * **resetAtCtrlOnset** [optional] whether to reset controller at onset of a new control epoch (gateCtrl from low to high) 
  * **doRecurse_Ke** [optional] whether to recalculate state estimator gain (Ke) recursively 


Perform a single step of feedback control. This is the most flexible option, but requires user to have set the controller's `xRef` and `uRef` variables.


---
### **steadyState_fbCtrl**

```cpp
void steadyState_fbCtrl(
    armaVec & z,
    bool & gateCtrl =TRUE,
    bool & gateEst =TRUE,
    bool & gateLock =FALSE,
    data_t & sigma_softStart =DEFAULT_SOFTSTART,
    data_t & sigma_uNoise =DATA_T_ZERO,
    bool & resetAtCtrlOnset =TRUE,
    bool & doRecurse_Ke =TRUE
)
```



**Parameters**:

  * **z** measurement 
  * **gateCtrl** [optional] logical gate of control (high=enabled; low=disabled) 
  * **gateEst** [optional] logical gate of estimator (high=enabled; low=disabled) 
  * **gateLock** [optional] logical gate of control signal lock (if high, locks control at current value) 
  * **sigma_softStart** [optional] standard deviation (sigma) of a Gaussian soft-start to control 
  * **sigma_uNoise** [optional] standard deviation (sigma) of Gaussian noise added on top of control signal 
  * **resetAtCtrlOnset** [optional] whether to reset controller at onset of a new control epoch (gateCtrl from low to high) 
  * **doRecurse_Ke** [optional] whether to recalculate state estimator gain (Ke) recursively 


Assuming the objective is to track a reference output (`yRef`) at steady state, perform a single step of control. Requires user to have set the controller's `yRef` variable. If control is enabled (`gateCtrl=true`) but estimator is disabled (`gateEst=true`), this function provides the calculated steady-state input `uRef` in open loop.


---
### **calc_ssSetPt**

```cpp
void calc_ssSetPt()
```



Calculate steady-state setpoint [`xRef`; `uRef`], given an output reference `yRef`. 


---
### **printSys**

```cpp
void printSys()
```



---
### **setDims**

```cpp
void setDims(
    std::size_t & nU,
    std::size_t & nX,
    std::size_t & nY
)
```



---
### **setU**

```cpp
void setU(
    stdVec & uVec
)
```



---
### **setU**

```cpp
void setU(
    armaVec & u
)
```



---
### **setG**

```cpp
void setG(
    stdVec & gVec
)
```



---
### **setG**

```cpp
void setG(
    armaVec & g
)
```



---
### **setGDesign**

```cpp
void setGDesign(
    stdVec & gVec
)
```



---
### **setGDesign**

```cpp
void setGDesign(
    armaVec & g
)
```



---
### **setURef**

```cpp
void setURef(
    stdVec & uRefVec
)
```



---
### **setURef**

```cpp
void setURef(
    armaVec & uRef
)
```



---
### **setXRef**

```cpp
void setXRef(
    stdVec & xRefVec
)
```



---
### **setXRef**

```cpp
void setXRef(
    armaVec & xRef
)
```



---
### **setYRef**

```cpp
void setYRef(
    stdVec & yRefVec
)
```



---
### **setYRef**

```cpp
void setYRef(
    armaVec & yRef
)
```



---
### **setKc_x**

```cpp
void setKc_x(
    stdVec & Kc_x_vec
)
```



---
### **setKc_x**

```cpp
void setKc_x(
    armaMat & Kc_x
)
```



---
### **setKc_u**

```cpp
void setKc_u(
    stdVec & Kc_u_vec
)
```



---
### **setKc_u**

```cpp
void setKc_u(
    armaMat & Kc_u
)
```



---
### **setKc_inty**

```cpp
void setKc_inty(
    stdVec & Kc_inty_vec
)
```



---
### **setKc_inty**

```cpp
void setKc_inty(
    armaMat & Kc_inty
)
```



---
### **setControlType**

```cpp
void setControlType(
    size_t controlType
)
```



---
### **setTauAntiWindup**

```cpp
void setTauAntiWindup(
    data_t & tau
)
```



---
### **getKc_u**

```cpp
inline armaMat getKc_u() const
```



---
### **getKc_x**

```cpp
inline armaMat getKc_x() const
```



---
### **getKc_inty**

```cpp
inline armaMat getKc_inty() const
```



---
### **getGDesign**

```cpp
inline armaVec getGDesign() const
```



---
### **getURef**

```cpp
inline armaVec getURef() const
```



---
### **getXRef**

```cpp
inline armaVec getXRef() const
```



---
### **getYRef**

```cpp
inline armaVec getYRef() const
```



---
### **getIntE**

```cpp
inline armaVec getIntE() const
```



---
### **getControlType**

```cpp
inline size_t getControlType() const
```



---
### **reset**

```cpp
void reset()
```



---


## Protected Function Details

### **antiWindup**

```cpp
void antiWindup()
```



Protect against integral windup

Reference:

Astroem KJ, Rundqwist L. (1989) Integrator Windup and How to Avoid It. 1989 ACC. 


---


## Protected Attribute Details

### **gDesign**

```cpp
armaVec gDesign;
```



---
### **uRef**

```cpp
armaVec uRef;
```



---
### **uRef_prev**

```cpp
armaVec uRef_prev;
```



---
### **xRef**

```cpp
armaVec xRef;
```



---
### **yRef**

```cpp
armaVec yRef;
```



---
### **Kc_x**

```cpp
armaMat Kc_x;
```



---
### **Kc_u**

```cpp
armaMat Kc_u;
```



---
### **Kc_inty**

```cpp
armaMat Kc_inty;
```



---
### **duRef**

```cpp
armaVec duRef;
```



---
### **dvRef**

```cpp
armaVec dvRef;
```



---
### **vRef**

```cpp
armaVec vRef;
```



---
### **dv**

```cpp
armaVec dv;
```



---
### **v**

```cpp
armaVec v;
```



---
### **intE**

```cpp
armaVec intE;
```



---
### **intE_awuAdjust**

```cpp
armaVec intE_awuAdjust;
```



---
### **uSat**

```cpp
armaVec uSat;
```



---
### **gateCtrl_prev**

```cpp
bool gateCtrl_prev;
```



---
### **gateLock_prev**

```cpp
bool gateLock_prev;
```



---
### **uSaturated**

```cpp
bool uSaturated;
```



---
### **uLB**

```cpp
data_t & uLB;
```



---
### **uUB**

```cpp
data_t & uUB;
```



---
### **tauAntiWindup**

```cpp
data_t tauAntiWindup;
```



---
### **kAntiWindup**

```cpp
data_t kAntiWindup;
```



---
### **t_since_ctrl_onset**

```cpp
data_t t_since_ctrl_onset;
```



---
### **controlType**

```cpp
size_t controlType;
```



---


-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
