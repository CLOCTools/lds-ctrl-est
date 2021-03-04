---
title: lds::poisson::ssidFit_t
summary: GLDS SSID Fit Type. 

---

# lds::poisson::ssidFit_t



GLDS SSID Fit Type. 
<br /> `#include <lds_poisson_fit_ssid.h>`

Inherits from [lds::poisson::fit_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1fit__t/), [lds::fit_t](/ldsctrlest/docs/api/classes/classlds_1_1fit__t/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[ssidFit_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ssid_fit__t/#function-ssidfit_t)**() |
| | **[ssidFit_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ssid_fit__t/#function-ssidfit_t)**(armaMat & A, armaMat & B, armaVec & g, armaVec & m, armaMat & Q, armaVec & x0, armaMat & P0, armaMat & C, armaVec & d, data_t dt, data_t t_startSSID, data_t t_stopSSID, armaVec & singVals, std::vector< data_t > & t0, std::vector< armaMat > & uTrain, std::vector< armaMat > & zTrain)<br>Constructs a new PLDS SSID fit.  |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| std::vector< data_t > | **[t0](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ssid_fit__t/#variable-t0)**  |
| data_t | **[t_startSSID](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ssid_fit__t/#variable-t_startssid)**  |
| data_t | **[t_stopSSID](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ssid_fit__t/#variable-t_stopssid)**  |
| armaVec | **[singVals](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ssid_fit__t/#variable-singvals)**  |

## Additional inherited members

**Public Functions inherited from [lds::poisson::fit_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1fit__t/)**

|                | Name           |
| -------------- | -------------- |
| | **[fit_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1fit__t/#function-fit_t)**() |
| | **[fit_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1fit__t/#function-fit_t)**(armaMat & A, armaMat & B, armaVec & g, armaVec & m, armaMat & Q, armaVec & x0, armaMat & P0, armaMat & C, armaVec & d, data_t dt, std::vector< armaMat > & uTrain, std::vector< armaMat > & zTrain)<br>Constructs a new PLDS fit.  |

**Public Attributes inherited from [lds::poisson::fit_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1fit__t/)**

|                | Name           |
| -------------- | -------------- |
| armaMat | **[C](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1fit__t/#variable-c)** <br>output matrix  |
| armaVec | **[d](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1fit__t/#variable-d)** <br>output bias  |
| std::vector< armaMat > | **[uTrain](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1fit__t/#variable-utrain)** <br>input training data  |
| std::vector< armaMat > | **[zTrain](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1fit__t/#variable-ztrain)** <br>measurement training data  |

**Public Functions inherited from [lds::fit_t](/ldsctrlest/docs/api/classes/classlds_1_1fit__t/)**

|                | Name           |
| -------------- | -------------- |
| | **[fit_t](/ldsctrlest/docs/api/classes/classlds_1_1fit__t/#function-fit_t)**() |
| | **[fit_t](/ldsctrlest/docs/api/classes/classlds_1_1fit__t/#function-fit_t)**(armaMat & A, armaMat & B, armaVec & g, armaVec & m, armaMat & Q, armaVec & x0, armaMat & P0, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & dt)<br>Constructs a new LDS fit type.  |

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

### **ssidFit_t**

```cpp
inline ssidFit_t()
```



---
### **ssidFit_t**

```cpp
ssidFit_t(
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
    data_t t_startSSID,
    data_t t_stopSSID,
    armaVec & singVals,
    std::vector< data_t > & t0,
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
  * **t_startSSID** start time for the data used in SSID 
  * **t_stopSSID** stop time for the data used in SSID 
  * **singVals** singular values from SSID SVD step 
  * **t0** start times for each trial of input/output data 
  * **uTrain** training input data 
  * **zTrain** training measurement data 


---


## Public Attribute Details

### **t0**

```cpp
std::vector< data_t > t0;
```



---
### **t_startSSID**

```cpp
data_t t_startSSID;
```



---
### **t_stopSSID**

```cpp
data_t t_stopSSID;
```



---
### **singVals**

```cpp
armaVec singVals;
```



---


-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
