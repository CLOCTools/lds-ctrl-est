---
title: lds::gaussian::emFit_t
summary: GLDS E-M Fit Type. 

---

# lds::gaussian::emFit_t



GLDS E-M Fit Type.  [More...](#detailed-description)


<br /> `#include <lds_gaussian_fit_em.h>`

Inherits from [lds::gaussian::fit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1fit__t/), [lds::fit_t](/ldsctrlest/docs/api/classes/classlds_1_1fit__t/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[emFit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#function-emfit_t)**() |
| | **[emFit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#function-emfit_t)**(armaMat & A, armaMat & B, armaVec & g, armaVec & m, armaMat & Q, armaVec & x0, armaMat & P0, armaMat & C, armaVec & d, armaMat & R, data_t dt, std::vector< armaMat > & uTrain, std::vector< armaMat > & zTrain)<br>Constructs a new EM Fit Type.  |
| | **[emFit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#function-emfit_t)**(data_t dt, std::vector< armaMat > & uTrain, std::vector< armaMat > & zTrain)<br>Constructs a new EM Fit Type.  |
| | **[emFit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#function-emfit_t)**([ssidFit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ssid_fit__t/) & fit0) |
| void | **[reset](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#function-reset)**(void )<br>Reset EM fit.  |
| void | **[runEM](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#function-runem)**(bool calcAB =true, bool calcQ =true, bool calcInitial =true, bool calcC =true, bool calcd =true, bool calcR =true)<br>Run Expectation-Maximization algorithm.  |
| std::vector< armaMat > | **[getX](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#function-getx)**()<br>Get state estimate.  |
| std::vector< armaMat > | **[getY](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#function-gety)**()<br>Get output estimate.  |
| armaMat | **[get_sum_E_xu_tm1_xu_tm1](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#function-get_sum_e_xu_tm1_xu_tm1)**() |
| armaMat | **[get_sum_E_xu_t_xu_tm1](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#function-get_sum_e_xu_t_xu_tm1)**() |
| armaMat | **[get_sum_E_xu_t_xu_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#function-get_sum_e_xu_t_xu_t)**() |
| size_t | **[getT](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#function-gett)**() |
| void | **[setY](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#function-sety)**(std::vector< armaMat > yHat)<br>Set output.  |

## Protected Functions

|                | Name           |
| -------------- | -------------- |
| armaVec | **[getTheta](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#function-gettheta)**()<br>Get recalculated parameters (packed into vector)  |
| void | **[kalmanFilt](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#function-kalmanfilt)**() |
| void | **[kalmanSmooth](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#function-kalmansmooth)**(bool forceCommonInitial)<br>Kalman smooth dataset (fixed-interval smoothing)  |
| void | **[Estep](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#function-estep)**(bool forceCommonInitial =false)<br>Expectation step.  |
| void | **[Mstep](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#function-mstep)**(bool calcAB =true, bool calcQ =true, bool calcInitial =false, bool calcC =false, bool calcd =false, bool calcR =true)<br>Maximization step.  |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| data_t | **[tol](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#variable-tol)** <br>tolerance for convergence  |
| size_t | **[maxIter](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#variable-maxiter)** <br>maximum number of EM iterations  |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| std::vector< armaMat > | **[xHat](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#variable-xhat)** <br>state estimate  |
| std::vector< arma::Cube< data_t > > | **[P](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#variable-p)** <br>state estimate covariance  |
| std::vector< armaMat > | **[yHat](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#variable-yhat)** <br>output estimate  |
| std::vector< arma::Cube< data_t > > | **[P_t_tm1](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#variable-p_t_tm1)** <br>single-lag state covariance  |
| armaMat | **[sum_E_xu_t_xu_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#variable-sum_e_xu_t_xu_t)** <br>state-input covariance (current time)  |
| armaMat | **[sum_E_xu_tm1_xu_tm1](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#variable-sum_e_xu_tm1_xu_tm1)** <br>state-input covariance (t-minus-1)  |
| armaMat | **[sum_E_xu_t_xu_tm1](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#variable-sum_e_xu_t_xu_tm1)** <br>single lag state-input covariance  |
| size_t | **[T](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/#variable-t)**  |

## Additional inherited members

**Public Functions inherited from [lds::gaussian::fit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1fit__t/)**

|                | Name           |
| -------------- | -------------- |
| | **[fit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1fit__t/#function-fit_t)**() |
| | **[fit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1fit__t/#function-fit_t)**(armaMat & A, armaMat & B, armaVec & g, armaVec & m, armaMat & Q, armaVec & x0, armaMat & P0, armaMat & C, armaMat & D, armaVec & d, armaMat & R, data_t dt, std::vector< armaMat > & uTrain, std::vector< armaMat > & zTrain)<br>Constructs a new GLDS fit.  |
| | **[fit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1fit__t/#function-fit_t)**(armaMat & A, armaMat & B, armaVec & g, armaVec & m, armaMat & Q, armaVec & x0, armaMat & P0, armaMat & C, armaVec & d, armaMat & R, data_t dt, std::vector< armaMat > & uTrain, std::vector< armaMat > & zTrain)<br>Constructs a new GLDS fit type.  |

**Public Attributes inherited from [lds::gaussian::fit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1fit__t/)**

|                | Name           |
| -------------- | -------------- |
| armaMat | **[C](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1fit__t/#variable-c)** <br>output matrix  |
| armaMat | **[D](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1fit__t/#variable-d)** <br>feedthrough matrix  |
| armaVec | **[d](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1fit__t/#variable-d)** <br>output bias  |
| armaMat | **[R](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1fit__t/#variable-r)** <br>output noise cov  |
| std::vector< armaMat > | **[uTrain](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1fit__t/#variable-utrain)** <br>input training data  |
| std::vector< armaMat > | **[zTrain](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1fit__t/#variable-ztrain)** <br>measurement training data  |

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


## Detailed Description

```cpp
class lds::gaussian::emFit_t;
```





```
        This type is used in the process of fitting GLDS models by
        expectation-maximization (EM).
```

---
---
## Public Function Details

### **emFit_t**

```cpp
inline emFit_t()
```



---
### **emFit_t**

```cpp
emFit_t(
    armaMat & A,
    armaMat & B,
    armaVec & g,
    armaVec & m,
    armaMat & Q,
    armaVec & x0,
    armaMat & P0,
    armaMat & C,
    armaVec & d,
    armaMat & R,
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
  * **R** output noise covariance 
  * **dt** sample period 
  * **uTrain** training input data 
  * **zTrain** training measurement data 


---
### **emFit_t**

```cpp
emFit_t(
    data_t dt,
    std::vector< armaMat > & uTrain,
    std::vector< armaMat > & zTrain
)
```



**Parameters**:

  * **dt** sample period 
  * **uTrain** training input data 
  * **zTrain** training measurement data 


Constructs a new EM Fit Type.

Initializes the model to a random walk.


---
### **emFit_t**

```cpp
emFit_t(
    ssidFit_t & fit0
)
```



---
### **reset**

```cpp
void reset(
    void 
)
```



---
### **runEM**

```cpp
void runEM(
    bool calcAB =true,
    bool calcQ =true,
    bool calcInitial =true,
    bool calcC =true,
    bool calcd =true,
    bool calcR =true
)
```



**Parameters**:

  * **calcAB** [optional] whether to caclulate dynamics (A, B) 
  * **calcQ** [optional] whether to calculate process noise covariance 
  * **calcInitial** [optional] whether to calculate initial conditions 
  * **calcC** [optional] whether to calculate output matrix 
  * **calcd** [optional] whether to calculate output bias 
  * **calcR** [optional] whether to calculate output noise covariance 


---
### **getX**

```cpp
inline std::vector< armaMat > getX()
```



---
### **getY**

```cpp
inline std::vector< armaMat > getY()
```



---
### **get_sum_E_xu_tm1_xu_tm1**

```cpp
inline armaMat get_sum_E_xu_tm1_xu_tm1()
```



---
### **get_sum_E_xu_t_xu_tm1**

```cpp
inline armaMat get_sum_E_xu_t_xu_tm1()
```



---
### **get_sum_E_xu_t_xu_t**

```cpp
inline armaMat get_sum_E_xu_t_xu_t()
```



---
### **getT**

```cpp
inline size_t getT()
```



---
### **setY**

```cpp
inline void setY(
    std::vector< armaMat > yHat
)
```



---


## Protected Function Details

### **getTheta**

```cpp
armaVec getTheta()
```



---
### **kalmanFilt**

```cpp
void kalmanFilt()
```



Kalman filter dataset Estimate x[t] | z[0...t] 


---
### **kalmanSmooth**

```cpp
void kalmanSmooth(
    bool forceCommonInitial
)
```



**Parameters**:

  * **forceCommonInitial** whether to force common initial condition for all trials 


Kalman smooth dataset (fixed-interval smoothing)

Estimate x[t] | z[0...T]


---
### **Estep**

```cpp
void Estep(
    bool forceCommonInitial =false
)
```



**Parameters**:

  * **forceCommonInitial** whether to force common initial condition for all trials 


---
### **Mstep**

```cpp
void Mstep(
    bool calcAB =true,
    bool calcQ =true,
    bool calcInitial =false,
    bool calcC =false,
    bool calcd =false,
    bool calcR =true
)
```



**Parameters**:

  * **calcAB** [optional] whether to caclulate dynamics (A, B) 
  * **calcQ** [optional] whether to calculate process noise covariance 
  * **calcInitial** [optional] whether to calculate initial conditions 
  * **calcC** [optional] whether to calculate output matrix 
  * **calcd** [optional] whether to calculate output bias 
  * **calcR** [optional] whether to calculate output noise covariance 


---


## Public Attribute Details

### **tol**

```cpp
data_t tol = 1e-3;
```



---
### **maxIter**

```cpp
size_t maxIter = 100;
```



---


## Protected Attribute Details

### **xHat**

```cpp
std::vector< armaMat > xHat;
```



---
### **P**

```cpp
std::vector< arma::Cube< data_t > > P;
```



---
### **yHat**

```cpp
std::vector< armaMat > yHat;
```



---
### **P_t_tm1**

```cpp
std::vector< arma::Cube< data_t > > P_t_tm1;
```



---
### **sum_E_xu_t_xu_t**

```cpp
armaMat sum_E_xu_t_xu_t;
```



---
### **sum_E_xu_tm1_xu_tm1**

```cpp
armaMat sum_E_xu_tm1_xu_tm1;
```



---
### **sum_E_xu_t_xu_tm1**

```cpp
armaMat sum_E_xu_t_xu_tm1;
```



---
### **T**

```cpp
size_t T;
```



---


-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
