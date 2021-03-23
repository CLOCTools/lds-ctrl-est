---
title: lds::poisson::SwitchedController
summary: Poisson-observation SwitchedController Type. 

---

# lds::poisson::SwitchedController



Poisson-observation [SwitchedController]() Type. 
<br /> `#include <lds_poisson_sctrl.h>`

Inherits from [lds::SwitchedController< System >](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/), [lds::Controller< System >](/ldsctrlest/docs/api/classes/classlds_1_1_controller/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| virtual void | **[set_y_ref](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1_switched_controller/#function-set_y_ref)**(const Vector & y_ref) override<br>Set reference output.  |

## Additional inherited members

**Public Functions inherited from [lds::SwitchedController< System >](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/)**

|                | Name           |
| -------------- | -------------- |
| | **[SwitchedController](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/#function-switchedcontroller)**() =default<br>Constructs a new [SwitchedController](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/).  |
| | **[SwitchedController](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/#function-switchedcontroller)**(const std::vector< [System](/ldsctrlest/docs/api/classes/classlds_1_1_system/) > & systems, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) u_lb, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) u_ub, size_t control_type =0)<br>Constructs a new [SwitchedController](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/).  |
| | **[SwitchedController](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/#function-switchedcontroller)**(std::vector< [System](/ldsctrlest/docs/api/classes/classlds_1_1_system/) > && systems, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) u_lb, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) u_ub, size_t control_type =0)<br>Constructs a new [SwitchedController](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/) (moves systems).  |
| void | **[Switch](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/#function-switch)**(size_t idx, bool do_force_switch =false)<br>Switch to a different sub-system/controller.  |
| void | **[set_Kc](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/#function-set_kc)**(const [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)<> & Kc)<br>sets state feedback gains  |
| void | **[set_Kc](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/#function-set_kc)**([UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)<> && Kc)<br>sets state feedback gains (moving)  |
| void | **[set_Kc_inty](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/#function-set_kc_inty)**(const [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)<> & Kc_inty)<br>sets integral feedback gains  |
| void | **[set_Kc_inty](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/#function-set_kc_inty)**([UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)<> && Kc_inty)<br>sets integral feedback gains (moving)  |
| void | **[set_Kc_u](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/#function-set_kc_u)**(const [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)<> & Kc_u)<br>sets input feedback gains  |
| void | **[set_Kc_u](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/#function-set_kc_u)**([UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)<> && Kc_u)<br>sets input feedback gains (moving)  |
| void | **[set_g_design](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/#function-set_g_design)**(const [UniformVectorList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_vector_list/) & g)<br>sets input gain used during controller design  |
| void | **[set_g_design](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/#function-set_g_design)**([UniformVectorList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_vector_list/) && g)<br>sets input gain used during controller design (moving)  |

**Protected Attributes inherited from [lds::SwitchedController< System >](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/)**

|                | Name           |
| -------------- | -------------- |
| std::vector< [System](/ldsctrlest/docs/api/classes/classlds_1_1_system/) > | **[systems_](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/#variable-systems_)** <br>underlying sub-systems which are switched between  |
| size_t | **[n_sys_](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/#variable-n_sys_)** <br>number of systems  |
| size_t | **[idx_](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/#variable-idx_)** <br>current system/controller index.  |
| [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/) | **[Kc_list_](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/#variable-kc_list_)**  |
| [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/) | **[Kc_inty_list_](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/#variable-kc_inty_list_)**  |
| [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/) | **[Kc_u_list_](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/#variable-kc_u_list_)**  |
| [UniformVectorList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_vector_list/) | **[g_design_list_](/ldsctrlest/docs/api/classes/classlds_1_1_switched_controller/#variable-g_design_list_)**  |

**Public Functions inherited from [lds::Controller< System >](/ldsctrlest/docs/api/classes/classlds_1_1_controller/)**

|                | Name           |
| -------------- | -------------- |
| | **[Controller](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-controller)**() =default<br>Constructs a new [Controller](/ldsctrlest/docs/api/classes/classlds_1_1_controller/).  |
| | **[Controller](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-controller)**(const [System](/ldsctrlest/docs/api/classes/classlds_1_1_system/) & sys, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) u_lb, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) u_ub, size_t control_type =0)<br>Constructs a new [Controller](/ldsctrlest/docs/api/classes/classlds_1_1_controller/).  |
| | **[Controller](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-controller)**([System](/ldsctrlest/docs/api/classes/classlds_1_1_system/) && sys, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) u_lb, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) u_ub, size_t control_type =0)<br>Constructs a new [Controller](/ldsctrlest/docs/api/classes/classlds_1_1_controller/) by moving the system object.  |
| const Vector & | **[Control](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-control)**(const Vector & z, bool do_control =true, bool do_lock_control =false, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) sigma_soft_start =0, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) sigma_u_noise =0, bool do_reset_at_control_onset =true)<br>updates control signal (single-step)  |
| const Vector & | **[ControlOutputReference](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-controloutputreference)**(const Vector & z, bool do_control =true, bool do_estimation =true, bool do_lock_control =false, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) sigma_soft_start =0, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) sigma_u_noise =0, bool do_reset_at_control_onset =true)<br>updates control signal, given previously-set (single-step)  |
| const [System](/ldsctrlest/docs/api/classes/classlds_1_1_system/) & | **[sys](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-sys)**() const |
| const Matrix & | **[Kc](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-kc)**() const<br>Get state feedback controller gain.  |
| const Matrix & | **[Kc_inty](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-kc_inty)**() const<br>Get integral controller gain.  |
| const Matrix & | **[Kc_u](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-kc_u)**() const<br>Get input feedback controller gain.  |
| const Vector & | **[g_design](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-g_design)**() const<br>Get input gain used in controller design.  |
| const Vector & | **[u_ref](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-u_ref)**() const<br>Get reference input.  |
| const Vector & | **[x_ref](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-x_ref)**() const<br>Get reference state.  |
| const Vector & | **[y_ref](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-y_ref)**() const<br>Get reference output.  |
| size_t | **[control_type](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-control_type)**() const<br>Get controller type.  |
| void | **[set_sys](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-set_sys)**(const [System](/ldsctrlest/docs/api/classes/classlds_1_1_system/) & sys)<br>Set system.  |
| void | **[set_g_design](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-set_g_design)**(const Vector & g_design)<br>Set input gain used in controller design (g_design)  |
| void | **[set_u_ref](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-set_u_ref)**(const Vector & u_ref)<br>Set reference input (u_ref)  |
| void | **[set_x_ref](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-set_x_ref)**(const Vector & x_ref)<br>Set reference state (x_ref)  |
| void | **[set_Kc](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-set_kc)**(const Matrix & Kc)<br>Set state controller gain.  |
| void | **[set_Kc_inty](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-set_kc_inty)**(const Matrix & Kc_inty)<br>Set integral controller gain.  |
| void | **[set_Kc_u](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-set_kc_u)**(const Matrix & Kc_u)<br>Set input controller gain.  |
| void | **[set_tau_awu](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-set_tau_awu)**([data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) tau)<br>Set time constant of anti-integral-windup.  |
| void | **[set_control_type](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-set_control_type)**(size_t control_type)<br>Sets the control type.  |
| void | **[Reset](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-reset)**()<br>reset system and control variables.  |
| void | **[Print](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-print)**()<br>prints variables to stdout  |

**Protected Attributes inherited from [lds::Controller< System >](/ldsctrlest/docs/api/classes/classlds_1_1_controller/)**

|                | Name           |
| -------------- | -------------- |
| [System](/ldsctrlest/docs/api/classes/classlds_1_1_system/) | **[sys_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-sys_)** <br>underlying LDS  |
| Vector | **[u_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-u_)** <br>control signal  |
| Vector | **[g_design_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-g_design_)** <br>input gain of the system used for controller design  |
| Vector | **[u_ref_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-u_ref_)** <br>reference input  |
| Vector | **[u_ref_prev_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-u_ref_prev_)** <br>reference input at previous time step  |
| Vector | **[x_ref_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-x_ref_)** <br>reference state  |
| Vector | **[y_ref_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-y_ref_)** <br>reference output  |
| Vector | **[cx_ref_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-cx_ref_)**  |
| Matrix | **[Kc_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-kc_)** <br>state controller gain  |
| Matrix | **[Kc_u_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-kc_u_)** <br>input controller gain (optional when control updates \deltaU)  |
| Matrix | **[Kc_inty_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-kc_inty_)** <br>integral controller gain  |
| Vector | **[du_ref_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-du_ref_)**  |
| Vector | **[dv_ref_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-dv_ref_)**  |
| Vector | **[v_ref_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-v_ref_)**  |
| Vector | **[dv_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-dv_)**  |
| Vector | **[v_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-v_)** <br>Control after g inversion (e.g., control in physical units)  |
| Vector | **[int_e_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-int_e_)** <br>integrated error  |
| Vector | **[int_e_awu_adjust_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-int_e_awu_adjust_)** <br>anti-windup adjustment to intE  |
| Vector | **[u_sat_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-u_sat_)** <br>control signal after saturation (for antiWindup)  |
| bool | **[do_control_prev_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-do_control_prev_)**  |
| bool | **[do_lock_control_prev_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-do_lock_control_prev_)**  |
| bool | **[u_saturated_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-u_saturated_)** <br>whether control signal has reached saturation limits  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) | **[u_lb_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-u_lb_)** <br>lower bound on control  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) | **[u_ub_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-u_ub_)** <br>upper bound on control  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) | **[tau_awu_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-tau_awu_)** <br>antiwindup time constant  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) | **[k_awu_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-k_awu_)**  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) | **[t_since_control_onset_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-t_since_control_onset_)** <br>time since control epoch onset  |
| size_t | **[control_type_](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#variable-control_type_)** <br>controller type  |


---
---
## Public Function Details

### **set_y_ref**

```cpp
inline virtual void set_y_ref(
    const Vector & y_ref
) override
```



**Reimplements**: [lds::Controller::set_y_ref](/ldsctrlest/docs/api/classes/classlds_1_1_controller/#function-set_y_ref)


---


-------------------------------

Updated on 23 March 2021 at 09:14:14 CDT
