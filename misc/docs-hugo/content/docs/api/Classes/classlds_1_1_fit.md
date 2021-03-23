---
title: lds::Fit
summary: LDS Fit Type. 

---

# lds::Fit



LDS [Fit]() Type. 
<br /> `#include <lds_fit.h>`

Inherited by [lds::gaussian::Fit](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1_fit/), [lds::poisson::Fit](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1_fit/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Fit](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-fit)**() =default<br>Constructs a new [Fit](/ldsctrlest/docs/api/classes/classlds_1_1_fit/).  |
| | **[Fit](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-fit)**(size_t n_u, size_t n_x, size_t n_y, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) dt)<br>Constructs a new [Fit](/ldsctrlest/docs/api/classes/classlds_1_1_fit/).  |
| size_t | **[n_u](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-n_u)**() const<br>gets number of inputs  |
| size_t | **[n_x](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-n_x)**() const<br>gets number of states  |
| size_t | **[n_y](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-n_y)**() const<br>gets number of outputs  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) | **[dt](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-dt)**() const<br>gets sample period  |
| const Matrix & | **[A](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-a)**() const<br>gets state matrix  |
| const Matrix & | **[B](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-b)**() const<br>gets input matrix  |
| const Vector & | **[g](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-g)**() const<br>gets input gain  |
| const Vector & | **[m](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-m)**() const<br>gets process disturbance  |
| const Matrix & | **[Q](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-q)**() const<br>gets process noise covariance  |
| const Vector & | **[x0](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-x0)**() const<br>gets initial state estimate  |
| const Matrix & | **[P0](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-p0)**() const<br>gets covariance of initial state estimate  |
| const Matrix & | **[C](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-c)**() const<br>gets output matrix  |
| const Vector & | **[d](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-d)**() const<br>gets output bias  |
| virtual const Matrix & | **[R](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-r)**() const =0 |
| void | **[set_A](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-set_a)**(const Matrix & A)<br>sets state matrix  |
| void | **[set_B](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-set_b)**(const Matrix & B)<br>sets input matrix  |
| void | **[set_g](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-set_g)**(const Vector & g)<br>sets input gain/conversion factor  |
| void | **[set_m](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-set_m)**(const Vector & m)<br>sets process disturbance  |
| void | **[set_Q](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-set_q)**(const Matrix & Q)<br>sets process noise covariance  |
| virtual void | **[set_R](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-set_r)**(const Matrix & R) =0<br>sets output noise covariance (if any)  |
| void | **[set_x0](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-set_x0)**(const Vector & x0)<br>sets initial state estimate  |
| void | **[set_P0](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-set_p0)**(const Matrix & P0)<br>sets intial state estimate covariance  |
| void | **[set_C](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-set_c)**(const Matrix & C)<br>sets output matrix  |
| void | **[set_d](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-set_d)**(const Vector & d)<br>sets output bias  |
| View | **[f](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-f)**(Matrix & x, const Matrix & u, size_t t)<br>system dynamics function  |
| View | **[f](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-f)**(Matrix & x_pre, const Matrix & x_post, const Matrix & u, size_t t)<br>system dynamics function  |
| virtual View | **[h](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#function-h)**(Matrix & y, const Matrix & x, size_t t) =0<br>output function  |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) | **[dt_](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#variable-dt_)** <br>sample period  |
| Matrix | **[A_](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#variable-a_)** <br>state matrix  |
| Matrix | **[B_](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#variable-b_)** <br>input matrix  |
| Vector | **[g_](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#variable-g_)** <br>input gain  |
| Vector | **[m_](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#variable-m_)** <br>process noise mean  |
| Matrix | **[Q_](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#variable-q_)** <br>process noise cov  |
| Matrix | **[C_](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#variable-c_)** <br>output matrix  |
| Vector | **[d_](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#variable-d_)** <br>output bias  |
| Matrix | **[R_](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#variable-r_)** <br>measurement noise  |
| Vector | **[x0_](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#variable-x0_)** <br>initial state  |
| Matrix | **[P0_](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#variable-p0_)** <br>initial covar  |
| size_t | **[n_u_](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#variable-n_u_)** <br>number of inputs  |
| size_t | **[n_x_](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#variable-n_x_)** <br>number of states  |
| size_t | **[n_y_](/ldsctrlest/docs/api/classes/classlds_1_1_fit/#variable-n_y_)** <br>number of outputs  |

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
Fit(
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
### **m**

```cpp
inline const Vector & m() const
```



---
### **Q**

```cpp
inline const Matrix & Q() const
```



---
### **x0**

```cpp
inline const Vector & x0() const
```



---
### **P0**

```cpp
inline const Matrix & P0() const
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
### **R**

```cpp
virtual const Matrix & R() const =0
```



**Reimplemented by**: [lds::gaussian::Fit::R](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1_fit/#function-r), [lds::poisson::Fit::R](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1_fit/#function-r)


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
### **set_g**

```cpp
inline void set_g(
    const Vector & g
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
### **set_Q**

```cpp
inline void set_Q(
    const Matrix & Q
)
```



---
### **set_R**

```cpp
virtual void set_R(
    const Matrix & R
) =0
```



**Reimplemented by**: [lds::gaussian::Fit::set_R](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1_fit/#function-set_r), [lds::poisson::Fit::set_R](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1_fit/#function-set_r)


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
### **f**

```cpp
inline View f(
    Matrix & x,
    const Matrix & u,
    size_t t
)
```



**Parameters**:

  * **x** state estimate (over time) 
  * **u** input (over time) 
  * **t** time index


**Return**: view of updated state 

---
### **f**

```cpp
inline View f(
    Matrix & x_pre,
    const Matrix & x_post,
    const Matrix & u,
    size_t t
)
```



**Parameters**:

  * **x_pre** predicted state est. 
  * **x_post** posterior state est. 
  * **u** input (over time) 
  * **t** time index


**Return**: view of predicted state 

---
### **h**

```cpp
virtual View h(
    Matrix & y,
    const Matrix & x,
    size_t t
) =0
```



**Parameters**:

  * **y** output estimate (over time) 
  * **x** state estimate (over time) 
  * **t** time index


**Return**: output 

**Reimplemented by**: [lds::gaussian::Fit::h](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1_fit/#function-h), [lds::poisson::Fit::h](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1_fit/#function-h)


---


## Protected Attribute Details

### **dt_**

```cpp
data_t dt_ {};
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
### **m_**

```cpp
Vector m_;
```



---
### **Q_**

```cpp
Matrix Q_;
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
### **R_**

```cpp
Matrix R_;
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
### **n_u_**

```cpp
size_t n_u_ {};
```



---
### **n_x_**

```cpp
size_t n_x_ {};
```



---
### **n_y_**

```cpp
size_t n_y_ {};
```



---


-------------------------------

Updated on 23 March 2021 at 09:14:14 CDT
