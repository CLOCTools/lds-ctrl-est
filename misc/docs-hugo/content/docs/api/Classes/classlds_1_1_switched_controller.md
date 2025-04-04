---
title: lds::SwitchedController
summary: SwitchedController Type. 

---

# lds::SwitchedController



[SwitchedController]() Type.  [More...](#detailed-description)


<br /> `#include <lds_sctrl.h>`

Inherits from [lds::Controller< System >](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/)

Inherited by [lds::gaussian::SwitchedController](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_switched_controller/), [lds::poisson::SwitchedController](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_switched_controller/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[SwitchedController](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/#function-switchedcontroller)**() =default<br>Constructs a new [SwitchedController](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/).  |
| | **[SwitchedController](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/#function-switchedcontroller)**(const std::vector< [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) > & systems, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_lb, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_ub, size_t control_type =0)<br>Constructs a new [SwitchedController](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/).  |
| | **[SwitchedController](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/#function-switchedcontroller)**(std::vector< [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) > && systems, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_lb, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_ub, size_t control_type =0)<br>Constructs a new [SwitchedController](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/) (moves systems).  |
| void | **[Switch](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/#function-switch)**(size_t idx, bool do_force_switch =false)<br>Switch to a different sub-system/controller.  |
| void | **[set_Kc](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/#function-set-kc)**(const [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)<> & Kc)<br>sets state feedback gains  |
| void | **[set_Kc](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/#function-set-kc)**([UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)<> && Kc)<br>sets state feedback gains (moving)  |
| void | **[set_Kc_inty](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/#function-set-kc-inty)**(const [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)<> & Kc_inty)<br>sets integral feedback gains  |
| void | **[set_Kc_inty](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/#function-set-kc-inty)**([UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)<> && Kc_inty)<br>sets integral feedback gains (moving)  |
| void | **[set_Kc_u](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/#function-set-kc-u)**(const [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)<> & Kc_u)<br>sets input feedback gains  |
| void | **[set_Kc_u](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/#function-set-kc-u)**([UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)<> && Kc_u)<br>sets input feedback gains (moving)  |
| void | **[set_g_design](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/#function-set-g-design)**(const [UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/) & g)<br>sets input gain used during controller design  |
| void | **[set_g_design](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/#function-set-g-design)**([UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/) && g)<br>sets input gain used during controller design (moving)  |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| std::vector< [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) > | **[systems_](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/#variable-systems-)** <br>underlying sub-systems which are switched between  |
| size_t | **[n_sys_](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/#variable-n-sys-)** <br>number of systems  |
| size_t | **[idx_](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/#variable-idx-)** <br>current system/controller index.  |
| [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/) | **[Kc_list_](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/#variable-kc-list-)**  |
| [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/) | **[Kc_inty_list_](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/#variable-kc-inty-list-)**  |
| [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/) | **[Kc_u_list_](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/#variable-kc-u-list-)**  |
| [UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/) | **[g_design_list_](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/#variable-g-design-list-)**  |

## Additional inherited members

**Public Functions inherited from [lds::Controller< System >](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/)**

|                | Name           |
| -------------- | -------------- |
| | **[Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-controller)**() =default<br>Constructs a new [Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/).  |
| | **[Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-controller)**(const [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) & sys, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_lb, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_ub, size_t control_type =0)<br>Constructs a new [Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/).  |
| | **[Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-controller)**([System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) && sys, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_lb, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_ub, size_t control_type =0)<br>Constructs a new [Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/) by moving the system object.  |
| const Vector & | **[Control](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-control)**(const Vector & z, bool do_control =true, bool do_lock_control =false, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) sigma_soft_start =0, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) sigma_u_noise =0, bool do_reset_at_control_onset =true)<br>updates control signal (single-step)  |
| const Vector & | **[ControlOutputReference](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-controloutputreference)**(const Vector & z, bool do_control =true, bool do_estimation =true, bool do_lock_control =false, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) sigma_soft_start =0, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) sigma_u_noise =0, bool do_reset_at_control_onset =true)<br>updates control signal, given previously-set (single-step)  |
| const [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) & | **[sys](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-sys)**() const |
| const Matrix & | **[Kc](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-kc)**() const<br>Get state feedback controller gain.  |
| const Matrix & | **[Kc_inty](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-kc-inty)**() const<br>Get integral controller gain.  |
| const Matrix & | **[Kc_u](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-kc-u)**() const<br>Get input feedback controller gain.  |
| const Vector & | **[g_design](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-g-design)**() const<br>Get input gain used in controller design.  |
| const Vector & | **[u_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-u-ref)**() const<br>Get reference input.  |
| const Vector & | **[x_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-x-ref)**() const<br>Get reference state.  |
| const Vector & | **[y_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-y-ref)**() const<br>Get reference output.  |
| size_t | **[control_type](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-control-type)**() const<br>Get controller type.  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[tau_awu](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-tau-awu)**() const<br>Get time constant of anti-integral-windup.  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[u_lb](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-u-lb)**() const<br>Get control lower bound.  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[u_ub](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-u-ub)**() const<br>Get control upper bound.  |
| void | **[set_sys](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-set-sys)**(const [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) & sys)<br>Set system.  |
| void | **[set_u_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-set-u-ref)**(const Vector & u_ref)<br>Set reference input (u_ref)  |
| void | **[set_x_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-set-x-ref)**(const Vector & x_ref)<br>Set reference state (x_ref)  |
| virtual void | **[set_y_ref](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-set-y-ref)**(const Vector & y_ref)<br>Set reference output (y_ref)  |
| void | **[set_tau_awu](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-set-tau-awu)**([data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) tau)<br>Set time constant of anti-integral-windup.  |
| void | **[set_control_type](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-set-control-type)**(size_t control_type)<br>Sets the control type.  |
| void | **[set_u_lb](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-set-u-lb)**([data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_lb)<br>sets control lower bound  |
| void | **[set_u_ub](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-set-u-ub)**([data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) u_ub)<br>Sets control upper bound.  |
| void | **[Reset](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-reset)**()<br>reset system and control variables.  |
| void | **[Print](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#function-print)**()<br>prints variables to stdout  |

**Protected Attributes inherited from [lds::Controller< System >](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/)**

|                | Name           |
| -------------- | -------------- |
| [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) | **[sys_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-sys-)** <br>underlying LDS  |
| Vector | **[u_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-u-)** <br>control signal  |
| Vector | **[u_return_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-u-return-)** <br>control signal that is _returned_ to user  |
| Vector | **[g_design_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-g-design-)** <br>input gain of the system used for controller design  |
| Vector | **[u_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-u-ref-)** <br>reference input  |
| Vector | **[u_ref_prev_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-u-ref-prev-)** <br>reference input at previous time step  |
| Vector | **[x_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-x-ref-)** <br>reference state  |
| Vector | **[y_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-y-ref-)** <br>reference output  |
| Vector | **[cx_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-cx-ref-)**  |
| Matrix | **[Kc_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-kc-)** <br>state controller gain  |
| Matrix | **[Kc_u_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-kc-u-)** <br>input controller gain (optional when control updates \deltaU)  |
| Matrix | **[Kc_inty_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-kc-inty-)** <br>integral controller gain  |
| Vector | **[du_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-du-ref-)**  |
| Vector | **[dv_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-dv-ref-)**  |
| Vector | **[v_ref_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-v-ref-)**  |
| Vector | **[dv_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-dv-)**  |
| Vector | **[v_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-v-)** <br>Control after g inversion (e.g., control in physical units)  |
| Vector | **[int_e_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-int-e-)** <br>integrated error  |
| Vector | **[int_e_awu_adjust_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-int-e-awu-adjust-)** <br>anti-windup adjustment to intE  |
| Vector | **[u_sat_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-u-sat-)** <br>control signal after saturation (for antiWindup)  |
| bool | **[do_control_prev_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-do-control-prev-)**  |
| bool | **[do_lock_control_prev_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-do-lock-control-prev-)**  |
| bool | **[u_saturated_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-u-saturated-)** <br>whether control signal has reached saturation limits  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[u_lb_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-u-lb-)** <br>lower bound on control  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[u_ub_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-u-ub-)** <br>upper bound on control  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[tau_awu_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-tau-awu-)** <br>antiwindup time constant  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[k_awu_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-k-awu-)**  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[t_since_control_onset_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-t-since-control-onset-)** <br>time since control epoch onset  |
| size_t | **[control_type_](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/#variable-control-type-)** <br>controller type  |


## Detailed Description

```cpp
template <typename System >
class lds::SwitchedController;
```


---
---
## Public Function Details

### **SwitchedController**

```cpp
SwitchedController() =default
```



---
### **SwitchedController**

```cpp
inline SwitchedController(
    const std::vector< System > & systems,
    data_t u_lb,
    data_t u_ub,
    size_t control_type =0
)
```



**Parameters**:

  * **systems** vector of sub-systems 
  * **u_lb** lower bound on control (u) 
  * **u_ub** upper bound on control (u) 
  * **control_type** [optional] control type bit mask 


---
### **SwitchedController**

```cpp
inline SwitchedController(
    std::vector< System > && systems,
    data_t u_lb,
    data_t u_ub,
    size_t control_type =0
)
```



**Parameters**:

  * **systems** vector of sub-systems 
  * **u_lb** lower bound on control (u) 
  * **u_ub** upper bound on control (u) 
  * **control_type** [optional] control type bit mask 


---
### **Switch**

```cpp
inline void Switch(
    size_t idx,
    bool do_force_switch =false
)
```



**Parameters**:

  * **idx** index 
  * **do_force_switch** whether to force a system switch even if already there. 


---
### **set_Kc**

```cpp
inline void set_Kc(
    const UniformMatrixList<> & Kc
)
```



---
### **set_Kc**

```cpp
inline void set_Kc(
    UniformMatrixList<> && Kc
)
```



---
### **set_Kc_inty**

```cpp
inline void set_Kc_inty(
    const UniformMatrixList<> & Kc_inty
)
```



---
### **set_Kc_inty**

```cpp
inline void set_Kc_inty(
    UniformMatrixList<> && Kc_inty
)
```



---
### **set_Kc_u**

```cpp
inline void set_Kc_u(
    const UniformMatrixList<> & Kc_u
)
```



---
### **set_Kc_u**

```cpp
inline void set_Kc_u(
    UniformMatrixList<> && Kc_u
)
```



---
### **set_g_design**

```cpp
inline void set_g_design(
    const UniformVectorList & g
)
```



---
### **set_g_design**

```cpp
inline void set_g_design(
    UniformVectorList && g
)
```



---


## Protected Attribute Details

### **systems_**

```cpp
std::vector< System > systems_;
```



---
### **n_sys_**

```cpp
size_t n_sys_ {};
```



---
### **idx_**

```cpp
size_t idx_ {};
```



---
### **Kc_list_**

```cpp
UniformMatrixList Kc_list_;
```



---
### **Kc_inty_list_**

```cpp
UniformMatrixList Kc_inty_list_;
```



---
### **Kc_u_list_**

```cpp
UniformMatrixList Kc_u_list_;
```



---
### **g_design_list_**

```cpp
UniformVectorList g_design_list_;
```



---


-------------------------------

Updated on  3 April 2025 at 13:48:29 EDT
