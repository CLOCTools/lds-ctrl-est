---
title: lds::gaussian::System
summary: Gaussian LDS Type. 

---

# lds::gaussian::System



Gaussian LDS Type. 
<br /> `#include <lds_gaussian_sys.h>`

Inherits from [lds::System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[System](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/#function-system)**() =default<br>Constructs a new [System](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/).  |
| | **[System](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/#function-system)**(std::size_t n_u, std::size_t n_x, std::size_t n_y, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) dt, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) p0 =kDefaultP0, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) q0 =kDefaultQ0, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) r0 =kDefaultR0)<br>Constructs a new Gaussian [System](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/).  |
| virtual const Vector & | **[Simulate](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/#function-simulate)**(const Vector & u_tm1) override<br>Simulate system measurement.  |
| const Matrix & | **[R](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/#function-r)**() const<br>Get output noise covariance.  |
| void | **[set_Q](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/#function-set_q)**(const Matrix & Q) |
| void | **[set_R](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/#function-set_r)**(const Matrix & R)<br>Set output noise covariance.  |
| void | **[set_Ke](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/#function-set_ke)**(const Matrix & Ke)<br>Set estimator gain.  |
| void | **[set_Ke_m](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/#function-set_ke_m)**(const Matrix & Ke_m)<br>Set disturbance estimator gain.  |
| void | **[Print](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/#function-print)**()<br>Print system variables to stdout.  |

## Protected Functions

|                | Name           |
| -------------- | -------------- |
| virtual void | **[h](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/#function-h)**() override<br>[System](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/) output function.  |
| virtual void | **[RecurseKe](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/#function-recurseke)**() override<br>Recursively update estimator gain.  |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| Matrix | **[R_](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/#variable-r_)** <br>covariance of output noise  |
| bool | **[do_recurse_Ke_](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/#variable-do_recurse_ke_)** <br>whether to recursively calculate estimator gain  |

## Additional inherited members

**Public Functions inherited from [lds::System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/)**

|                | Name           |
| -------------- | -------------- |
| void | **[Filter](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-filter)**(const Vector & u_tm1, const Vector & z)<br>Filter data to produce causal state estimates.  |
| void | **[f](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-f)**(const Vector & u, bool do_add_noise =false)<br>system dynamics function  |
| size_t | **[n_u](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-n_u)**() const<br>Get number of inputs.  |
| size_t | **[n_x](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-n_x)**() const<br>Get number of states.  |
| size_t | **[n_y](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-n_y)**() const<br>Get number of outputs.  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) | **[dt](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-dt)**() const<br>Get sample period.  |
| const Vector & | **[x](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-x)**() const<br>Get current state.  |
| const Matrix & | **[P](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-p)**() const<br>Get covariance of state estimate.  |
| const Vector & | **[m](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-m)**() const<br>Get current process disturbance/bias.  |
| const Matrix & | **[P_m](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-p_m)**() const<br>Get covariance of process disturbance estimate.  |
| const Vector & | **[cx](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-cx)**() const<br>Get C*x.  |
| const Vector & | **[y](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-y)**() const<br>Get output.  |
| const Vector & | **[x0](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-x0)**() const<br>Get initial state.  |
| const Vector & | **[m0](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-m0)**() const<br>Get initial disturbance.  |
| const Matrix & | **[A](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-a)**() const<br>Get state matrix.  |
| const Matrix & | **[B](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-b)**() const<br>Get input matrix.  |
| const Vector & | **[g](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-g)**() const<br>Get input gain/conversion factor.  |
| const Matrix & | **[C](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-c)**() const<br>Get output matrix.  |
| const Vector & | **[d](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-d)**() const<br>Get output bias.  |
| const Matrix & | **[Ke](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-ke)**() const<br>Get estimator gain.  |
| const Matrix & | **[Ke_m](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-ke_m)**() const<br>Get estimator gain for process disturbance (m)  |
| void | **[set_A](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-set_a)**(const Matrix & A)<br>Set state matrix.  |
| void | **[set_B](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-set_b)**(const Matrix & B)<br>Set input matrix.  |
| void | **[set_m](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-set_m)**(const Vector & m)<br>Set process disturbance.  |
| void | **[set_g](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-set_g)**(const Vector & g)<br>Set input gain.  |
| void | **[set_Q_m](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-set_q_m)**(const Matrix & Q_m)<br>Set process noise covariance of disturbance evoluation.  |
| void | **[set_x0](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-set_x0)**(const Vector & x0)<br>Set initial state.  |
| void | **[set_P0](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-set_p0)**(const Matrix & P0)<br>Set covariance of initial state.  |
| void | **[set_P0_m](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-set_p0_m)**(const Matrix & P0_m)<br>Set covariance of initial process disturbance.  |
| void | **[set_C](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-set_c)**(const Matrix & C)<br>Set output matrix.  |
| void | **[set_d](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-set_d)**(const Vector & d)<br>Set output bias.  |
| void | **[Reset](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-reset)**()<br>Reset system variables.  |

**Protected Functions inherited from [lds::System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/)**

|                | Name           |
| -------------- | -------------- |
| void | **[InitVars](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-initvars)**([data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) p0 =kDefaultP0, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) q0 =kDefaultQ0) |

**Public Attributes inherited from [lds::System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/)**

|                | Name           |
| -------------- | -------------- |
| bool | **[do_adapt_m](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-do_adapt_m)** <br>whether to adaptively estimate disturbance m  |

**Protected Attributes inherited from [lds::System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/)**

|                | Name           |
| -------------- | -------------- |
| std::size_t | **[n_x_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-n_x_)** <br>number of states  |
| std::size_t | **[n_u_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-n_u_)** <br>number of inputs  |
| std::size_t | **[n_y_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-n_y_)** <br>number of outputs  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) | **[dt_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-dt_)** <br>sample period  |
| Vector | **[x_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-x_)** <br>state  |
| Matrix | **[P_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-p_)** <br>covariance of state estimate  |
| Vector | **[m_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-m_)** <br>process disturbance  |
| Matrix | **[P_m_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-p_m_)** <br>covariance of disturbance estimate  |
| Vector | **[cx_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-cx_)** <br>C*x.  |
| Vector | **[y_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-y_)** <br>output  |
| Vector | **[z_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-z_)** <br>measurement  |
| Vector | **[x0_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-x0_)** <br>initial state  |
| Matrix | **[P0_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-p0_)** <br>covariance of initial state estimate  |
| Vector | **[m0_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-m0_)** <br>initial process disturbance  |
| Matrix | **[P0_m_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-p0_m_)** <br>covariance of initial disturbance est.  |
| Matrix | **[A_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-a_)** <br>state matrix  |
| Matrix | **[B_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-b_)** <br>input matrix  |
| Vector | **[g_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-g_)** <br>input gain  |
| Matrix | **[Q_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-q_)** <br>covariance of process noise  |
| Matrix | **[Q_m_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-q_m_)** <br>covariance of disturbance random walk  |
| Matrix | **[C_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-c_)** <br>output matrix  |
| Vector | **[d_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-d_)** <br>output bias  |
| Matrix | **[Ke_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-ke_)** <br>estimator gain  |
| Matrix | **[Ke_m_](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-ke_m_)** <br>estimator gain for process disturbance  |


---
---
## Public Function Details

### **System**

```cpp
System() =default
```



---
### **System**

```cpp
System(
    std::size_t n_u,
    std::size_t n_x,
    std::size_t n_y,
    data_t dt,
    data_t p0 =kDefaultP0,
    data_t q0 =kDefaultQ0,
    data_t r0 =kDefaultR0
)
```



**Parameters**:

  * **n_u** number of inputs (u) 
  * **n_x** number of states (x) 
  * **n_y** number of outputs (y) 
  * **dt** sample period 
  * **p0** [optional] initial diagonal elements of state estimate covariance (P) 
  * **q0** [optional] initial diagonal elements of process noise covariance (Q) 
  * **r0** [optional] initial diagonal elements of output noise covariance (R) 


---
### **Simulate**

```cpp
virtual const Vector & Simulate(
    const Vector & u_tm1
) override
```



**Parameters**:

  * **u_tm1** input at t-1


**Return**: z measurement 

**Reimplements**: [lds::System::Simulate](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-simulate)


Simulate system and produce measurement


---
### **R**

```cpp
inline const Matrix & R() const
```



---
### **set_Q**

```cpp
inline void set_Q(
    const Matrix & Q
)
```



---
### **set_R**

```cpp
inline void set_R(
    const Matrix & R
)
```



---
### **set_Ke**

```cpp
inline void set_Ke(
    const Matrix & Ke
)
```



---
### **set_Ke_m**

```cpp
inline void set_Ke_m(
    const Matrix & Ke_m
)
```



---
### **Print**

```cpp
void Print()
```



---


## Protected Function Details

### **h**

```cpp
inline virtual void h() override
```



**Reimplements**: [lds::System::h](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-h)


---
### **RecurseKe**

```cpp
virtual void RecurseKe() override
```



**Reimplements**: [lds::System::RecurseKe](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-recurseke)


---


## Protected Attribute Details

### **R_**

```cpp
Matrix R_;
```



---
### **do_recurse_Ke_**

```cpp
bool do_recurse_Ke_ {};
```



---


-------------------------------

Updated on 24 March 2021 at 13:11:19 EDT
