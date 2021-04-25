---
title: lds::System
summary: Linear Dynamical System Type. 

---

# lds::System



Linear Dynamical [System]() Type. 
<br /> `#include <lds_sys.h>`

Inherited by [lds::gaussian::System](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/), [lds::poisson::System](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_system/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-system)**() =default<br>Constructs a new [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/).  |
| | **[System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-system)**(size_t n_u, size_t n_x, size_t n_y, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) dt, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) p0 =kDefaultP0, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) q0 =kDefaultQ0)<br>constructs a new [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) |
| void | **[Filter](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-filter)**(const Vector & u_tm1, const Vector & z)<br>Filter data to produce causal state estimates.  |
| virtual const Vector & | **[Simulate](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-simulate)**(const Vector & u_tm1) =0<br>simulates system (single time step)  |
| void | **[f](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-f)**(const Vector & u, bool do_add_noise =false)<br>system dynamics function  |
| virtual void | **[h](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-h)**() =0<br>system output function  |
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
| void | **[set_Q](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-set_q)**(const Matrix & Q)<br>Set process noise covariance.  |
| void | **[set_Q_m](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-set_q_m)**(const Matrix & Q_m)<br>Set process noise covariance of disturbance evoluation.  |
| void | **[set_x0](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-set_x0)**(const Vector & x0)<br>Set initial state.  |
| void | **[set_P0](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-set_p0)**(const Matrix & P0)<br>Set covariance of initial state.  |
| void | **[set_P0_m](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-set_p0_m)**(const Matrix & P0_m)<br>Set covariance of initial process disturbance.  |
| void | **[set_C](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-set_c)**(const Matrix & C)<br>Set output matrix.  |
| void | **[set_d](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-set_d)**(const Vector & d)<br>Set output bias.  |
| void | **[Reset](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-reset)**()<br>Reset system variables.  |
| void | **[Print](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-print)**()<br>Print system variables to stdout.  |

## Protected Functions

|                | Name           |
| -------------- | -------------- |
| virtual void | **[RecurseKe](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-recurseke)**() =0<br>Recursively recalculate estimator gain (Ke)  |
| void | **[InitVars](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#function-initvars)**([data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) p0 =kDefaultP0, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) q0 =kDefaultQ0) |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| bool | **[do_adapt_m](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/#variable-do_adapt_m)** <br>whether to adaptively estimate disturbance m  |

## Protected Attributes

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
    size_t n_u,
    size_t n_x,
    size_t n_y,
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
  * **p0** diagonal elements for state estimate covariance 
  * **q0** diagonal elements for process noise covariance 


---
### **Filter**

```cpp
void Filter(
    const Vector & u_tm1,
    const Vector & z
)
```



**Parameters**:

  * **u_tm1** input at t-minus-1 
  * **z_t** current measurement 


Given current measurment and input, filter data to produce causal state estimates using Kalman filtering, which procedes by predicting the state and subsequently updating.


---
### **Simulate**

```cpp
virtual const Vector & Simulate(
    const Vector & u_tm1
) =0
```



**Parameters**:

  * **u_tm1** input at time t-1


**Return**: simulated measurement at time t 

**Reimplemented by**: [lds::gaussian::System::Simulate](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/#function-simulate), [lds::poisson::System::Simulate](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_system/#function-simulate)


---
### **f**

```cpp
inline void f(
    const Vector & u,
    bool do_add_noise =false
)
```



**Parameters**:

  * **u** input 
  * **do_add_noise** whether to add simulated process noise 


---
### **h**

```cpp
virtual void h() =0
```



**Reimplemented by**: [lds::gaussian::System::h](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/#function-h), [lds::poisson::System::h](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_system/#function-h)


---
### **n_u**

```cpp
inline size_t n_u() const
```



---
### **n_x**

```cpp
inline size_t n_x() const
```



---
### **n_y**

```cpp
inline size_t n_y() const
```



---
### **dt**

```cpp
inline data_t dt() const
```



---
### **x**

```cpp
inline const Vector & x() const
```



---
### **P**

```cpp
inline const Matrix & P() const
```



---
### **m**

```cpp
inline const Vector & m() const
```



---
### **P_m**

```cpp
inline const Matrix & P_m() const
```



---
### **cx**

```cpp
inline const Vector & cx() const
```



---
### **y**

```cpp
inline const Vector & y() const
```



---
### **x0**

```cpp
inline const Vector & x0() const
```



---
### **m0**

```cpp
inline const Vector & m0() const
```



---
### **A**

```cpp
inline const Matrix & A() const
```



---
### **B**

```cpp
inline const Matrix & B() const
```



---
### **g**

```cpp
inline const Vector & g() const
```



---
### **C**

```cpp
inline const Matrix & C() const
```



---
### **d**

```cpp
inline const Vector & d() const
```



---
### **Ke**

```cpp
inline const Matrix & Ke() const
```



---
### **Ke_m**

```cpp
inline const Matrix & Ke_m() const
```



---
### **set_A**

```cpp
inline void set_A(
    const Matrix & A
)
```



---
### **set_B**

```cpp
inline void set_B(
    const Matrix & B
)
```



---
### **set_m**

```cpp
inline void set_m(
    const Vector & m
)
```



---
### **set_g**

```cpp
inline void set_g(
    const Vector & g
)
```



---
### **set_Q**

```cpp
inline void set_Q(
    const Matrix & Q
)
```



---
### **set_Q_m**

```cpp
inline void set_Q_m(
    const Matrix & Q_m
)
```



---
### **set_x0**

```cpp
inline void set_x0(
    const Vector & x0
)
```



---
### **set_P0**

```cpp
inline void set_P0(
    const Matrix & P0
)
```



---
### **set_P0_m**

```cpp
inline void set_P0_m(
    const Matrix & P0_m
)
```



---
### **set_C**

```cpp
inline void set_C(
    const Matrix & C
)
```



---
### **set_d**

```cpp
inline void set_d(
    const Vector & d
)
```



---
### **Reset**

```cpp
void Reset()
```



---
### **Print**

```cpp
void Print()
```



---


## Protected Function Details

### **RecurseKe**

```cpp
virtual void RecurseKe() =0
```



**Reimplemented by**: [lds::gaussian::System::RecurseKe](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_system/#function-recurseke), [lds::poisson::System::RecurseKe](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_system/#function-recurseke)


---
### **InitVars**

```cpp
void InitVars(
    data_t p0 =kDefaultP0,
    data_t q0 =kDefaultQ0
)
```



---


## Public Attribute Details

### **do_adapt_m**

```cpp
bool do_adapt_m {};
```



---


## Protected Attribute Details

### **n_x_**

```cpp
std::size_t n_x_ {};
```



---
### **n_u_**

```cpp
std::size_t n_u_ {};
```



---
### **n_y_**

```cpp
std::size_t n_y_ {};
```



---
### **dt_**

```cpp
data_t dt_ {};
```



---
### **x_**

```cpp
Vector x_;
```



---
### **P_**

```cpp
Matrix P_;
```



---
### **m_**

```cpp
Vector m_;
```



---
### **P_m_**

```cpp
Matrix P_m_;
```



---
### **cx_**

```cpp
Vector cx_;
```



---
### **y_**

```cpp
Vector y_;
```



---
### **z_**

```cpp
Vector z_;
```



---
### **x0_**

```cpp
Vector x0_;
```



---
### **P0_**

```cpp
Matrix P0_;
```



---
### **m0_**

```cpp
Vector m0_;
```



---
### **P0_m_**

```cpp
Matrix P0_m_;
```



---
### **A_**

```cpp
Matrix A_;
```



---
### **B_**

```cpp
Matrix B_;
```



---
### **g_**

```cpp
Vector g_;
```



---
### **Q_**

```cpp
Matrix Q_;
```



---
### **Q_m_**

```cpp
Matrix Q_m_;
```



---
### **C_**

```cpp
Matrix C_;
```



---
### **d_**

```cpp
Vector d_;
```



---
### **Ke_**

```cpp
Matrix Ke_;
```



---
### **Ke_m_**

```cpp
Matrix Ke_m_;
```



---


-------------------------------

Updated on 25 April 2021 at 11:04:30 EDT
