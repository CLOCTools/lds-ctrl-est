---
title: lds::poisson::fit_t
summary: PLDS Fit Type. 

---

# lds::poisson::fit_t



PLDS Fit Type. 
<br /> `#include <lds_poisson_fit.h>`

Inherits from [lds::fit_t](/ldsctrlest/docs/api/classes/classlds_1_1fit__t/)

Inherited by [lds::poisson::ssidFit_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ssid_fit__t/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[fit_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1fit__t/#function-fit_t)**() |
| | **[fit_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1fit__t/#function-fit_t)**(armaMat & A, armaMat & B, armaVec & g, armaVec & m, armaMat & Q, armaVec & x0, armaMat & P0, armaMat & C, armaVec & d, data_t dt, std::vector< armaMat > & uTrain, std::vector< armaMat > & zTrain)<br>Constructs a new PLDS fit.  |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| armaMat | **[C](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1fit__t/#variable-c)** <br>output matrix  |
| armaVec | **[d](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1fit__t/#variable-d)** <br>output bias  |
| std::vector< armaMat > | **[uTrain](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1fit__t/#variable-utrain)** <br>input training data  |
| std::vector< armaMat > | **[zTrain](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1fit__t/#variable-ztrain)** <br>measurement training data  |

## Additional inherited members

**Public Attributes inherited from [lds::fit_t](/ldsctrlest/docs/api/classes/classlds_1_1fit__t/)**

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
    armaMat & C,
    armaVec & d,
    data_t dt,
    std::vector< armaMat > & uTrain,
    std::vector< armaMat > & zTrain
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
  * **C** output matrix 
  * **d** output bias 
  * **dt** sample period 
  * **uTrain** input training data 
  * **zTrain** measurement training data 


---


## Public Attribute Details

### **C**

```cpp
armaMat C;
```



---
### **d**

```cpp
armaVec d;
```



---
### **uTrain**

```cpp
std::vector< armaMat > uTrain;
```



---
### **zTrain**

```cpp
std::vector< armaMat > zTrain;
```



---


-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
