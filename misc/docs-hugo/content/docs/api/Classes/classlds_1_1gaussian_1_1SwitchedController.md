---
title: lds::gaussian::SwitchedController
summary: Gaussian-observation SwitchedController Type. 

---

# lds::gaussian::SwitchedController



Gaussian-observation [SwitchedController]() Type. 
<br /> `#include <lds_gaussian_sctrl.h>`

Inherits from [lds::SwitchedController< System >](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/), [lds::Controller< System >](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| virtual void | **[set_y_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1switchedcontroller/#function-set-y-ref)**(const Vector & y_ref) override<br>sets reference output  |

## Additional inherited members

**Public Functions inherited from [lds::SwitchedController< System >](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/)**

|                | Name           |
| -------------- | -------------- |
| | **[SwitchedController](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/#function-switchedcontroller)**() =default<br>Constructs a new [SwitchedController](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/).  |
| | **[SwitchedController](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/#function-switchedcontroller)**(const std::vector< [System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/) > & systems, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_lb, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_ub, size_t control_type =0)<br>Constructs a new [SwitchedController](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/).  |
| | **[SwitchedController](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/#function-switchedcontroller)**(std::vector< [System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/) > && systems, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_lb, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_ub, size_t control_type =0)<br>Constructs a new [SwitchedController](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/) (moves systems).  |
| void | **[Switch](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/#function-switch)**(size_t idx, bool do_force_switch =false)<br>Switch to a different sub-system/controller.  |
| void | **[set_Kc](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/#function-set-kc)**(const [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)<> & Kc)<br>sets state feedback gains  |
| void | **[set_Kc](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/#function-set-kc)**([UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)<> && Kc)<br>sets state feedback gains (moving)  |
| void | **[set_Kc_inty](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/#function-set-kc-inty)**(const [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)<> & Kc_inty)<br>sets integral feedback gains  |
| void | **[set_Kc_inty](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/#function-set-kc-inty)**([UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)<> && Kc_inty)<br>sets integral feedback gains (moving)  |
| void | **[set_Kc_u](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/#function-set-kc-u)**(const [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)<> & Kc_u)<br>sets input feedback gains  |
| void | **[set_Kc_u](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/#function-set-kc-u)**([UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)<> && Kc_u)<br>sets input feedback gains (moving)  |
| void | **[set_g_design](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/#function-set-g-design)**(const [UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformvectorlist/) & g)<br>sets input gain used during controller design  |
| void | **[set_g_design](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/#function-set-g-design)**([UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformvectorlist/) && g)<br>sets input gain used during controller design (moving)  |

**Protected Attributes inherited from [lds::SwitchedController< System >](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/)**

|                | Name           |
| -------------- | -------------- |
| std::vector< [System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/) > | **[systems_](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/#variable-systems-)** <br>underlying sub-systems which are switched between  |
| size_t | **[n_sys_](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/#variable-n-sys-)** <br>number of systems  |
| size_t | **[idx_](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/#variable-idx-)** <br>current system/controller index.  |
| [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/) | **[Kc_list_](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/#variable-kc-list-)**  |
| [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/) | **[Kc_inty_list_](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/#variable-kc-inty-list-)**  |
| [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/) | **[Kc_u_list_](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/#variable-kc-u-list-)**  |
| [UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformvectorlist/) | **[g_design_list_](/lds-ctrl-est/docs/api/classes/classlds_1_1switchedcontroller/#variable-g-design-list-)**  |

**Public Functions inherited from [lds::Controller< System >](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/)**

|                | Name           |
| -------------- | -------------- |
| | **[Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-controller)**() =default<br>Constructs a new [Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/).  |
| | **[Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-controller)**(const [System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/) & sys, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_lb, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_ub, size_t control_type =0)<br>Constructs a new [Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/).  |
| | **[Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-controller)**([System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/) && sys, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_lb, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_ub, size_t control_type =0)<br>Constructs a new [Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/) by moving the system object.  |
| const Vector & | **[Control](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-control)**(const Vector & z, bool do_control =true, bool do_lock_control =false, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) sigma_soft_start =0, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) sigma_u_noise =0, bool do_reset_at_control_onset =true)<br>updates control signal (single-step)  |
| const Vector & | **[ControlOutputReference](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-controloutputreference)**(const Vector & z, bool do_control =true, bool do_estimation =true, bool do_lock_control =false, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) sigma_soft_start =0, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) sigma_u_noise =0, bool do_reset_at_control_onset =true)<br>updates control signal, given previously-set (single-step)  |
| const [System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/) & | **[sys](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-sys)**() const |
| const Matrix & | **[Kc](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-kc)**() const<br>Get state feedback controller gain.  |
| const Matrix & | **[Kc_inty](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-kc-inty)**() const<br>Get integral controller gain.  |
| const Matrix & | **[Kc_u](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-kc-u)**() const<br>Get input feedback controller gain.  |
| const Vector & | **[g_design](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-g-design)**() const<br>Get input gain used in controller design.  |
| const Vector & | **[u_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-u-ref)**() const<br>Get reference input.  |
| const Vector & | **[x_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-x-ref)**() const<br>Get reference state.  |
| const Vector & | **[y_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-y-ref)**() const<br>Get reference output.  |
| size_t | **[control_type](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-control-type)**() const<br>Get controller type.  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[tau_awu](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-tau-awu)**() const<br>Get time constant of anti-integral-windup.  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[u_lb](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-u-lb)**() const<br>Get control lower bound.  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[u_ub](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-u-ub)**() const<br>Get control upper bound.  |
| void | **[set_sys](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-set-sys)**(const [System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/) & sys)<br>Set system.  |
| void | **[set_g_design](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-set-g-design)**(const Vector & g_design)<br>Set input gain used in controller design (g_design)  |
| void | **[set_u_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-set-u-ref)**(const Vector & u_ref)<br>Set reference input (u_ref)  |
| void | **[set_x_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-set-x-ref)**(const Vector & x_ref)<br>Set reference state (x_ref)  |
| void | **[set_Kc](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-set-kc)**(const Matrix & Kc)<br>Set state controller gain.  |
| void | **[set_Kc_inty](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-set-kc-inty)**(const Matrix & Kc_inty)<br>Set integral controller gain.  |
| void | **[set_Kc_u](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-set-kc-u)**(const Matrix & Kc_u)<br>Set input controller gain.  |
| void | **[set_tau_awu](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-set-tau-awu)**([data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) tau)<br>Set time constant of anti-integral-windup.  |
| void | **[set_control_type](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-set-control-type)**(size_t control_type)<br>Sets the control type.  |
| void | **[set_u_lb](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-set-u-lb)**([data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_lb)<br>sets control lower bound  |
| void | **[set_u_ub](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-set-u-ub)**([data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_ub)<br>Sets control upper bound.  |
| void | **[Reset](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-reset)**()<br>reset system and control variables.  |
| void | **[Print](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-print)**()<br>prints variables to stdout  |

**Protected Attributes inherited from [lds::Controller< System >](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/)**

|                | Name           |
| -------------- | -------------- |
| [System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/) | **[sys_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-sys-)** <br>underlying LDS  |
| Vector | **[u_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-u-)** <br>control signal  |
| Vector | **[u_return_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-u-return-)** <br>control signal that is _returned_ to user  |
| Vector | **[g_design_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-g-design-)** <br>input gain of the system used for controller design  |
| Vector | **[u_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-u-ref-)** <br>reference input  |
| Vector | **[u_ref_prev_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-u-ref-prev-)** <br>reference input at previous time step  |
| Vector | **[x_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-x-ref-)** <br>reference state  |
| Vector | **[y_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-y-ref-)** <br>reference output  |
| Vector | **[cx_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-cx-ref-)**  |
| Matrix | **[Kc_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-kc-)** <br>state controller gain  |
| Matrix | **[Kc_u_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-kc-u-)** <br>input controller gain (optional when control updates )  |
| Matrix | **[Kc_inty_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-kc-inty-)** <br>integral controller gain  |
| Vector | **[du_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-du-ref-)**  |
| Vector | **[dv_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-dv-ref-)**  |
| Vector | **[v_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-v-ref-)**  |
| Vector | **[dv_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-dv-)**  |
| Vector | **[v_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-v-)** <br>Control after g inversion (e.g., control in physical units)  |
| Vector | **[int_e_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-int-e-)** <br>integrated error  |
| Vector | **[int_e_awu_adjust_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-int-e-awu-adjust-)** <br>anti-windup adjustment to intE  |
| Vector | **[u_sat_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-u-sat-)** <br>control signal after saturation (for antiWindup)  |
| bool | **[do_control_prev_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-do-control-prev-)**  |
| bool | **[do_lock_control_prev_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-do-lock-control-prev-)**  |
| bool | **[u_saturated_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-u-saturated-)** <br>whether control signal has reached saturation limits  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[u_lb_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-u-lb-)** <br>lower bound on control  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[u_ub_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-u-ub-)** <br>upper bound on control  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[tau_awu_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-tau-awu-)** <br>antiwindup time constant  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[k_awu_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-k-awu-)**  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[t_since_control_onset_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-t-since-control-onset-)** <br>time since control epoch onset  |
| size_t | **[control_type_](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#variable-control-type-)** <br>controller type  |


---
---
## Public Function Details

### **set_y_ref**

```cpp
inline virtual void set_y_ref(
    const Vector & y_ref
) override
```



**Reimplements**: [lds::Controller::set_y_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1controller/#function-set-y-ref)


---


-------------------------------

Updated on  4 May 2022 at 16:34:52 Eastern Daylight Time
