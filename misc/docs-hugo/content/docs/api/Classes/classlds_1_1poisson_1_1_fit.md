---
title: lds::poisson::Fit
summary: PLDS Fit Type. 

---

# lds::poisson::Fit



PLDS [Fit]() Type. 
<br /> `#include <lds_poisson_fit.h>`

Inherits from [lds::Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_fit/#function-fit)**() =default |
| | **[Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_fit/#function-fit)**(size_t n_u, size_t n_x, size_t n_y, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) dt)<br>Constructs a new [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_fit/).  |
| virtual View | **[h](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_fit/#function-h)**(Matrix & y, const Matrix & x, size_t t) override<br>output function  |
| virtual void | **[set_R](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_fit/#function-set_r)**(const Matrix & R) override<br>sets output noise covariance (if any)  |
| virtual const Matrix & | **[R](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_fit/#function-r)**() const override |

## Additional inherited members

**Public Functions inherited from [lds::Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/)**

|                | Name           |
| -------------- | -------------- |
| size_t | **[n_u](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-n_u)**() const<br>gets number of inputs  |
| size_t | **[n_x](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-n_x)**() const<br>gets number of states  |
| size_t | **[n_y](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-n_y)**() const<br>gets number of outputs  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) | **[dt](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-dt)**() const<br>gets sample period  |
| const Matrix & | **[A](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-a)**() const<br>gets state matrix  |
| const Matrix & | **[B](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-b)**() const<br>gets input matrix  |
| const Vector & | **[g](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-g)**() const<br>gets input gain  |
| const Vector & | **[m](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-m)**() const<br>gets process disturbance  |
| const Matrix & | **[Q](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-q)**() const<br>gets process noise covariance  |
| const Vector & | **[x0](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-x0)**() const<br>gets initial state estimate  |
| const Matrix & | **[P0](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-p0)**() const<br>gets covariance of initial state estimate  |
| const Matrix & | **[C](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-c)**() const<br>gets output matrix  |
| const Vector & | **[d](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-d)**() const<br>gets output bias  |
| void | **[set_A](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-set_a)**(const Matrix & A)<br>sets state matrix  |
| void | **[set_B](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-set_b)**(const Matrix & B)<br>sets input matrix  |
| void | **[set_g](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-set_g)**(const Vector & g)<br>sets input gain/conversion factor  |
| void | **[set_m](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-set_m)**(const Vector & m)<br>sets process disturbance  |
| void | **[set_Q](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-set_q)**(const Matrix & Q)<br>sets process noise covariance  |
| void | **[set_x0](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-set_x0)**(const Vector & x0)<br>sets initial state estimate  |
| void | **[set_P0](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-set_p0)**(const Matrix & P0)<br>sets intial state estimate covariance  |
| void | **[set_C](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-set_c)**(const Matrix & C)<br>sets output matrix  |
| void | **[set_d](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-set_d)**(const Vector & d)<br>sets output bias  |
| View | **[f](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-f)**(Matrix & x, const Matrix & u, size_t t)<br>system dynamics function  |
| View | **[f](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-f)**(Matrix & x_pre, const Matrix & x_post, const Matrix & u, size_t t)<br>system dynamics function  |

**Protected Attributes inherited from [lds::Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/)**

|                | Name           |
| -------------- | -------------- |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) | **[dt_](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#variable-dt_)** <br>sample period  |
| Matrix | **[A_](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#variable-a_)** <br>state matrix  |
| Matrix | **[B_](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#variable-b_)** <br>input matrix  |
| Vector | **[g_](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#variable-g_)** <br>input gain  |
| Vector | **[m_](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#variable-m_)** <br>process noise mean  |
| Matrix | **[Q_](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#variable-q_)** <br>process noise cov  |
| Matrix | **[C_](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#variable-c_)** <br>output matrix  |
| Vector | **[d_](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#variable-d_)** <br>output bias  |
| Matrix | **[R_](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#variable-r_)** <br>measurement noise  |
| Vector | **[x0_](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#variable-x0_)** <br>initial state  |
| Matrix | **[P0_](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#variable-p0_)** <br>initial covar  |
| size_t | **[n_u_](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#variable-n_u_)** <br>number of inputs  |
| size_t | **[n_x_](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#variable-n_x_)** <br>number of states  |
| size_t | **[n_y_](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#variable-n_y_)** <br>number of outputs  |


---
---
## Public Function Details

### **Fit**

```cpp
Fit() =default
```



---
### **Fit**

```cpp
inline Fit(
    size_t n_u,
    size_t n_x,
    size_t n_y,
    data_t dt
)
```



**Parameters**:

  * **n_u** number of inputs 
  * **n_x** number of states 
  * **n_y** number of outputs 
  * **dt** sample period 


---
### **h**

```cpp
inline virtual View h(
    Matrix & y,
    const Matrix & x,
    size_t t
) override
```



**Parameters**:

  * **y** output estimate (over time) 
  * **x** state estimate (over time) 
  * **t** time index


**Return**: output 

**Reimplements**: [lds::Fit::h](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-h)


---
### **set_R**

```cpp
inline virtual void set_R(
    const Matrix & R
) override
```



**Reimplements**: [lds::Fit::set_R](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-set_r)


---
### **R**

```cpp
inline virtual const Matrix & R() const override
```



**Reimplements**: [lds::Fit::R](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/#function-r)


---


-------------------------------

Updated on 30 March 2021 at 15:49:43 CDT