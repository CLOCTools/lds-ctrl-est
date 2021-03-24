---
title: lds::poisson::Controller
summary: PLDS Controller Type. 

---

# lds::poisson::Controller



PLDS [Controller]() Type. 
<br /> `#include <lds_poisson_ctrl.h>`

Inherits from [lds::Controller< System >](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| virtual void | **[set_y_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_controller/#function-set_y_ref)**(const Vector & y_ref) override<br>Set reference output.  |

## Additional inherited members

**Public Functions inherited from [lds::Controller< System >](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/)**

|                | Name           |
| -------------- | -------------- |
| | **[Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-controller)**() =default<br>Constructs a new [Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/).  |
| | **[Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-controller)**(const [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) & sys, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) u_lb, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) u_ub, size_t control_type =0)<br>Constructs a new [Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/).  |
| | **[Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-controller)**([System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) && sys, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) u_lb, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) u_ub, size_t control_type =0)<br>Constructs a new [Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/) by moving the system object.  |
| const Vector & | **[Control](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-control)**(const Vector & z, bool do_control =true, bool do_lock_control =false, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) sigma_soft_start =0, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) sigma_u_noise =0, bool do_reset_at_control_onset =true)<br>updates control signal (single-step)  |
| const Vector & | **[ControlOutputReference](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-controloutputreference)**(const Vector & z, bool do_control =true, bool do_estimation =true, bool do_lock_control =false, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) sigma_soft_start =0, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) sigma_u_noise =0, bool do_reset_at_control_onset =true)<br>updates control signal, given previously-set (single-step)  |
| const [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) & | **[sys](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-sys)**() const |
| const Matrix & | **[Kc](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-kc)**() const<br>Get state feedback controller gain.  |
| const Matrix & | **[Kc_inty](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-kc_inty)**() const<br>Get integral controller gain.  |
| const Matrix & | **[Kc_u](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-kc_u)**() const<br>Get input feedback controller gain.  |
| const Vector & | **[g_design](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-g_design)**() const<br>Get input gain used in controller design.  |
| const Vector & | **[u_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-u_ref)**() const<br>Get reference input.  |
| const Vector & | **[x_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-x_ref)**() const<br>Get reference state.  |
| const Vector & | **[y_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-y_ref)**() const<br>Get reference output.  |
| size_t | **[control_type](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-control_type)**() const<br>Get controller type.  |
| void | **[set_sys](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-set_sys)**(const [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) & sys)<br>Set system.  |
| void | **[set_g_design](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-set_g_design)**(const Vector & g_design)<br>Set input gain used in controller design (g_design)  |
| void | **[set_u_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-set_u_ref)**(const Vector & u_ref)<br>Set reference input (u_ref)  |
| void | **[set_x_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-set_x_ref)**(const Vector & x_ref)<br>Set reference state (x_ref)  |
| void | **[set_Kc](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-set_kc)**(const Matrix & Kc)<br>Set state controller gain.  |
| void | **[set_Kc_inty](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-set_kc_inty)**(const Matrix & Kc_inty)<br>Set integral controller gain.  |
| void | **[set_Kc_u](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-set_kc_u)**(const Matrix & Kc_u)<br>Set input controller gain.  |
| void | **[set_tau_awu](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-set_tau_awu)**([data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) tau)<br>Set time constant of anti-integral-windup.  |
| void | **[set_control_type](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-set_control_type)**(size_t control_type)<br>Sets the control type.  |
| void | **[Reset](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-reset)**()<br>reset system and control variables.  |
| void | **[Print](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-print)**()<br>prints variables to stdout  |

**Protected Attributes inherited from [lds::Controller< System >](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/)**

|                | Name           |
| -------------- | -------------- |
| [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) | **[sys_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-sys_)** <br>underlying LDS  |
| Vector | **[u_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-u_)** <br>control signal  |
| Vector | **[g_design_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-g_design_)** <br>input gain of the system used for controller design  |
| Vector | **[u_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-u_ref_)** <br>reference input  |
| Vector | **[u_ref_prev_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-u_ref_prev_)** <br>reference input at previous time step  |
| Vector | **[x_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-x_ref_)** <br>reference state  |
| Vector | **[y_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-y_ref_)** <br>reference output  |
| Vector | **[cx_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-cx_ref_)**  |
| Matrix | **[Kc_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-kc_)** <br>state controller gain  |
| Matrix | **[Kc_u_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-kc_u_)** <br>input controller gain (optional when control updates \deltaU)  |
| Matrix | **[Kc_inty_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-kc_inty_)** <br>integral controller gain  |
| Vector | **[du_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-du_ref_)**  |
| Vector | **[dv_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-dv_ref_)**  |
| Vector | **[v_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-v_ref_)**  |
| Vector | **[dv_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-dv_)**  |
| Vector | **[v_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-v_)** <br>Control after g inversion (e.g., control in physical units)  |
| Vector | **[int_e_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-int_e_)** <br>integrated error  |
| Vector | **[int_e_awu_adjust_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-int_e_awu_adjust_)** <br>anti-windup adjustment to intE  |
| Vector | **[u_sat_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-u_sat_)** <br>control signal after saturation (for antiWindup)  |
| bool | **[do_control_prev_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-do_control_prev_)**  |
| bool | **[do_lock_control_prev_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-do_lock_control_prev_)**  |
| bool | **[u_saturated_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-u_saturated_)** <br>whether control signal has reached saturation limits  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) | **[u_lb_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-u_lb_)** <br>lower bound on control  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) | **[u_ub_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-u_ub_)** <br>upper bound on control  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) | **[tau_awu_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-tau_awu_)** <br>antiwindup time constant  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) | **[k_awu_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-k_awu_)**  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) | **[t_since_control_onset_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-t_since_control_onset_)** <br>time since control epoch onset  |
| size_t | **[control_type_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-control_type_)** <br>controller type  |


---
---
## Public Function Details

### **set_y_ref**

```cpp
inline virtual void set_y_ref(
    const Vector & y_ref
) override
```



**Reimplements**: [lds::Controller::set_y_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-set_y_ref)


---


-------------------------------

Updated on 24 March 2021 at 13:26:05 EDT
