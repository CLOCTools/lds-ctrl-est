---
title: lds::poisson::System
summary: Poisson System type. 

---

# lds::poisson::System



Poisson [System]() type. 
<br /> `#include <lds_poisson_sys.h>`

Inherits from [lds::System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[System](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1system/#function-system)**() =default<br>Constructs a new [System](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1system/).  |
| | **[System](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1system/#function-system)**(std::size_t n_u, std::size_t n_x, std::size_t n_y, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) dt, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) p0 =kDefaultP0, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) q0 =kDefaultQ0)<br>Constructs a new Poisson [System](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1system/).  |
| virtual const Vector & | **[Simulate](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1system/#function-simulate)**(const Vector & u_tm1) override<br>Simulate system measurement.  |

## Protected Functions

|                | Name           |
| -------------- | -------------- |
| virtual void | **[h](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1system/#function-h)**() override<br>[System](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1system/) output function.  |
| virtual void | **[RecurseKe](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1system/#function-recurseke)**() override<br>Recursively recalculate estimator gain (Ke)  |

## Additional inherited members

**Public Functions inherited from [lds::System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/)**

|                | Name           |
| -------------- | -------------- |
| virtual | **[~System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-~system)**() |
| void | **[Filter](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-filter)**(const Vector & u_tm1, const Vector & z)<br>Filter data to produce causal state estimates.  |
| void | **[f](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-f)**(const Vector & u, bool do_add_noise =false)<br>system dynamics function  |
| size_t | **[n_u](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-n-u)**() const<br>Get number of inputs.  |
| size_t | **[n_x](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-n-x)**() const<br>Get number of states.  |
| size_t | **[n_y](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-n-y)**() const<br>Get number of outputs.  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[dt](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-dt)**() const<br>Get sample period.  |
| const Vector & | **[x](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-x)**() const<br>Get current state.  |
| const Matrix & | **[P](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-p)**() const<br>Get covariance of state estimate.  |
| const Vector & | **[m](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-m)**() const<br>Get current process disturbance/bias.  |
| const Matrix & | **[P_m](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-p-m)**() const<br>Get covariance of process disturbance estimate.  |
| const Vector & | **[cx](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-cx)**() const<br>Get C*x.  |
| const Vector & | **[y](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-y)**() const<br>Get output.  |
| const Vector & | **[x0](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-x0)**() const<br>Get initial state.  |
| const Vector & | **[m0](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-m0)**() const<br>Get initial disturbance.  |
| const Matrix & | **[A](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-a)**() const<br>Get state matrix.  |
| const Matrix & | **[B](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-b)**() const<br>Get input matrix.  |
| const Vector & | **[g](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-g)**() const<br>Get input gain/conversion factor.  |
| const Matrix & | **[C](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-c)**() const<br>Get output matrix.  |
| const Vector & | **[d](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-d)**() const<br>Get output bias.  |
| const Matrix & | **[Ke](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-ke)**() const<br>Get estimator gain.  |
| const Matrix & | **[Ke_m](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-ke-m)**() const<br>Get estimator gain for process disturbance (m)  |
| const Matrix & | **[Q](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-q)**()<br>Get process noise covariance.  |
| const Matrix & | **[Q_m](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-q-m)**()<br>Get process noise covariance of disturbance evoluation.  |
| const Matrix & | **[P0](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-p0)**()<br>Get covariance of initial state.  |
| const Matrix & | **[P0_m](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-p0-m)**()<br>Get covariance of initial process disturbance.  |
| void | **[set_A](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-set-a)**(const Matrix & A)<br>Set state matrix.  |
| void | **[set_B](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-set-b)**(const Matrix & B)<br>Set input matrix.  |
| void | **[set_m](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-set-m)**(const Vector & m, bool do_force_assign =false)<br>Set process disturbance.  |
| void | **[set_g](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-set-g)**(const Vector & g)<br>Set input gain.  |
| void | **[set_Q](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-set-q)**(const Matrix & Q)<br>Set process noise covariance.  |
| void | **[set_Q_m](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-set-q-m)**(const Matrix & Q_m)<br>Set process noise covariance of disturbance evoluation.  |
| void | **[set_x0](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-set-x0)**(const Vector & x0)<br>Set initial state.  |
| void | **[set_P0](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-set-p0)**(const Matrix & P0)<br>Set covariance of initial state.  |
| void | **[set_P0_m](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-set-p0-m)**(const Matrix & P0_m)<br>Set covariance of initial process disturbance.  |
| void | **[set_C](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-set-c)**(const Matrix & C)<br>Set output matrix.  |
| void | **[set_d](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-set-d)**(const Vector & d)<br>Set output bias.  |
| void | **[set_x](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-set-x)**(const Vector & x)<br>Set state of system.  |
| void | **[Reset](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-reset)**()<br>Reset system variables.  |
| void | **[Print](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-print)**()<br>Print system variables to stdout.  |

**Protected Functions inherited from [lds::System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/)**

|                | Name           |
| -------------- | -------------- |
| void | **[InitVars](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-initvars)**([data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) p0 =kDefaultP0, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) q0 =kDefaultQ0) |

**Public Attributes inherited from [lds::System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/)**

|                | Name           |
| -------------- | -------------- |
| bool | **[do_adapt_m](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-do-adapt-m)** <br>whether to adaptively estimate disturbance m  |

**Protected Attributes inherited from [lds::System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/)**

|                | Name           |
| -------------- | -------------- |
| std::size_t | **[n_x_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-n-x-)** <br>number of states  |
| std::size_t | **[n_u_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-n-u-)** <br>number of inputs  |
| std::size_t | **[n_y_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-n-y-)** <br>number of outputs  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[dt_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-dt-)** <br>sample period  |
| Vector | **[x_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-x-)** <br>state  |
| Matrix | **[P_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-p-)** <br>covariance of state estimate  |
| Vector | **[m_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-m-)** <br>process disturbance  |
| Matrix | **[P_m_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-p-m-)** <br>covariance of disturbance estimate  |
| Vector | **[cx_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-cx-)** <br>C*x.  |
| Vector | **[y_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-y-)** <br>output  |
| Vector | **[z_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-z-)** <br>measurement  |
| Vector | **[x0_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-x0-)** <br>initial state  |
| Matrix | **[P0_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-p0-)** <br>covariance of initial state estimate  |
| Vector | **[m0_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-m0-)** <br>initial process disturbance  |
| Matrix | **[P0_m_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-p0-m-)** <br>covariance of initial disturbance est.  |
| Matrix | **[A_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-a-)** <br>state matrix  |
| Matrix | **[B_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-b-)** <br>input matrix  |
| Vector | **[g_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-g-)** <br>input gain  |
| Matrix | **[Q_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-q-)** <br>covariance of process noise  |
| Matrix | **[Q_m_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-q-m-)** <br>covariance of disturbance random walk  |
| Matrix | **[C_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-c-)** <br>output matrix  |
| Vector | **[d_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-d-)** <br>output bias  |
| Matrix | **[Ke_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-ke-)** <br>estimator gain  |
| Matrix | **[Ke_m_](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#variable-ke-m-)** <br>estimator gain for process disturbance  |


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
    data_t q0 =kDefaultQ0
)
```



**Parameters**:

  * **n_u** number of inputs 
  * **n_x** number of states 
  * **n_y** number of outputs 
  * **dt** sample period 
  * **p0** [optional] initial diagonal elements of state estimate covariance (P) 
  * **q0** [optional] initial diagonal elements of process noise covariance (Q) 


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

**Reimplements**: [lds::System::Simulate](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-simulate)


Simulate system and produce measurement


---


## Protected Function Details

### **h**

```cpp
inline virtual void h() override
```



**Reimplements**: [lds::System::h](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-h)


---
### **RecurseKe**

```cpp
virtual void RecurseKe() override
```



**Reimplements**: [lds::System::RecurseKe](/lds-ctrl-est/docs/api/classes/classlds_1_1system/#function-recurseke)


Recursively recalculate estimator gain (Ke).

References:

Smith AC, Brown EN. (2003) Estimating a State-Space Model from Point Process Observations. Neural Computation 15.

Eden UT, ..., Brown EN. (2004) Dynamic Analysis of Neural Encoding by Point Process Adaptive Filtering Neural Computation 16. 


---


-------------------------------

Updated on  4 May 2022 at 15:48:58 Eastern Daylight Time
