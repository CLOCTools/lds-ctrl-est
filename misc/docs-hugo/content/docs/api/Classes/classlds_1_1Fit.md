---
title: lds::Fit
summary: LDS Fit Type. 

---

# lds::Fit



LDS [Fit]() Type. 
<br /> `#include <lds_fit.h>`

Inherited by [lds::gaussian::Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1fit/), [lds::poisson::Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1fit/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-fit)**() =default<br>Constructs a new [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/).  |
| | **[Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-fit)**(size_t n_u, size_t n_x, size_t n_y, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) dt)<br>Constructs a new [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/).  |
| virtual | **[~Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-~fit)**() =default |
| size_t | **[n_u](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-n-u)**() const<br>gets number of inputs  |
| size_t | **[n_x](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-n-x)**() const<br>gets number of states  |
| size_t | **[n_y](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-n-y)**() const<br>gets number of outputs  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[dt](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-dt)**() const<br>gets sample period  |
| const Matrix & | **[A](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-a)**() const<br>gets state matrix  |
| const Matrix & | **[B](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-b)**() const<br>gets input matrix  |
| const Vector & | **[g](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-g)**() const<br>gets input gain  |
| const Vector & | **[m](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-m)**() const<br>gets process disturbance  |
| const Matrix & | **[Q](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-q)**() const<br>gets process noise covariance  |
| const Vector & | **[x0](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-x0)**() const<br>gets initial state estimate  |
| const Matrix & | **[P0](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-p0)**() const<br>gets covariance of initial state estimate  |
| const Matrix & | **[C](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-c)**() const<br>gets output matrix  |
| const Vector & | **[d](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-d)**() const<br>gets output bias  |
| virtual const Matrix & | **[R](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-r)**() const =0 |
| void | **[set_A](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-set-a)**(const Matrix & A)<br>sets state matrix  |
| void | **[set_B](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-set-b)**(const Matrix & B)<br>sets input matrix  |
| void | **[set_g](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-set-g)**(const Vector & g)<br>sets input gain/conversion factor  |
| void | **[set_m](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-set-m)**(const Vector & m)<br>sets process disturbance  |
| void | **[set_Q](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-set-q)**(const Matrix & Q)<br>sets process noise covariance  |
| virtual void | **[set_R](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-set-r)**(const Matrix & R) =0<br>sets output noise covariance (if any)  |
| void | **[set_x0](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-set-x0)**(const Vector & x0)<br>sets initial state estimate  |
| void | **[set_P0](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-set-p0)**(const Matrix & P0)<br>sets initial state estimate covariance  |
| void | **[set_C](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-set-c)**(const Matrix & C)<br>sets output matrix  |
| void | **[set_d](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-set-d)**(const Vector & d)<br>sets output bias  |
| View | **[f](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-f)**(Matrix & x, const Matrix & u, size_t t)<br>system dynamics function  |
| View | **[f](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-f)**(Matrix & x_pre, const Matrix & x_post, const Matrix & u, size_t t)<br>system dynamics function  |
| virtual View | **[h](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#function-h)**(Matrix & y, const Matrix & x, size_t t) =0<br>output function  |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[dt_](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#variable-dt-)** <br>sample period  |
| Matrix | **[A_](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#variable-a-)** <br>state matrix  |
| Matrix | **[B_](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#variable-b-)** <br>input matrix  |
| Vector | **[g_](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#variable-g-)** <br>input gain  |
| Vector | **[m_](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#variable-m-)** <br>process noise mean  |
| Matrix | **[Q_](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#variable-q-)** <br>process noise cov  |
| Matrix | **[C_](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#variable-c-)** <br>output matrix  |
| Vector | **[d_](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#variable-d-)** <br>output bias  |
| Matrix | **[R_](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#variable-r-)** <br>measurement noise  |
| Vector | **[x0_](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#variable-x0-)** <br>initial state  |
| Matrix | **[P0_](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#variable-p0-)** <br>initial covar  |
| size_t | **[n_u_](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#variable-n-u-)** <br>number of inputs  |
| size_t | **[n_x_](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#variable-n-x-)** <br>number of states  |
| size_t | **[n_y_](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/#variable-n-y-)** <br>number of outputs  |

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
### **~Fit**

```cpp
virtual ~Fit() =default
```



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



**Reimplemented by**: [lds::gaussian::Fit::R](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1fit/#function-r), [lds::poisson::Fit::R](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1fit/#function-r)


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



**Reimplemented by**: [lds::gaussian::Fit::set_R](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1fit/#function-set-r), [lds::poisson::Fit::set_R](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1fit/#function-set-r)


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

**Reimplemented by**: [lds::poisson::Fit::h](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1fit/#function-h), [lds::gaussian::Fit::h](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1fit/#function-h)


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

Updated on  4 May 2022 at 16:34:51 Eastern Daylight Time
