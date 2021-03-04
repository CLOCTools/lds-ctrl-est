---
title: lds::fit_t
summary: LDS Fit Type. 

---

# lds::fit_t



LDS Fit Type. 
<br /> `#include <lds_fit.h>`

Inherited by [lds::gaussian::fit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1fit__t/), [lds::poisson::fit_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1fit__t/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[fit_t](/ldsctrlest/docs/api/classes/classlds_1_1fit__t/#function-fit_t)**() |
| | **[fit_t](/ldsctrlest/docs/api/classes/classlds_1_1fit__t/#function-fit_t)**(armaMat & A, armaMat & B, armaVec & g, armaVec & m, armaMat & Q, armaVec & x0, armaMat & P0, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & dt)<br>Constructs a new LDS fit type.  |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| armaMat | **[A](/ldsctrlest/docs/api/classes/classlds_1_1fit__t/#variable-a)** <br>state matrix  |
| armaMat | **[B](/ldsctrlest/docs/api/classes/classlds_1_1fit__t/#variable-b)** <br>input matrix  |
| armaVec | **[g](/ldsctrlest/docs/api/classes/classlds_1_1fit__t/#variable-g)** <br>input gain  |
| armaVec | **[m](/ldsctrlest/docs/api/classes/classlds_1_1fit__t/#variable-m)** <br>process noise mean  |
| armaMat | **[Q](/ldsctrlest/docs/api/classes/classlds_1_1fit__t/#variable-q)** <br>process noise cov  |
| armaVec | **[x0](/ldsctrlest/docs/api/classes/classlds_1_1fit__t/#variable-x0)** <br>initial state  |
| armaMat | **[P0](/ldsctrlest/docs/api/classes/classlds_1_1fit__t/#variable-p0)** <br>initial covar  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) | **[dt](/ldsctrlest/docs/api/classes/classlds_1_1fit__t/#variable-dt)** <br>sample period  |

---
---
## Public Function Details

### **fit_t**

```cpp
inline fit_t()
```



---
### **fit_t**

```cpp
fit_t(
    armaMat & A,
    armaMat & B,
    armaVec & g,
    armaVec & m,
    armaMat & Q,
    armaVec & x0,
    armaMat & P0,
    data_t & dt
)
```



**Parameters**:

  * **A** state matrix 
  * **B** input matrix 
  * **g** input gain 
  * **m** process disturbance 
  * **Q** process noise covariance 
  * **x0** initial state estimate 
  * **P0** covariance of initial state estimate 
  * **dt** { parameter_description } 


---


## Public Attribute Details

### **A**

```cpp
armaMat A;
```



---
### **B**

```cpp
armaMat B;
```



---
### **g**

```cpp
armaVec g;
```



---
### **m**

```cpp
armaVec m;
```



---
### **Q**

```cpp
armaMat Q;
```



---
### **x0**

```cpp
armaVec x0;
```



---
### **P0**

```cpp
armaMat P0;
```



---
### **dt**

```cpp
data_t dt;
```



---


-------------------------------

Updated on  3 March 2021 at 23:06:11 CST
