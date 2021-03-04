---
title: lds::poisson::sys_t
summary: Poisson LDS Type. 

---

# lds::poisson::sys_t



Poisson LDS Type. 
<br /> `#include <lds_poisson_sys.h>`

Inherits from [lds::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/)

Inherited by [lds::poisson::ctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| void | **[filter](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-filter)**(armaVec & u_tm1, armaVec & z_t)<br>Filter data to produce causal state estimates.  |
| void | **[filter](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-filter)**(armaVec & z)<br>Filter data to produce causal state estimates.  |
| void | **[simMeasurement](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-simmeasurement)**(armaVec & z)<br>Simulate one step of the model and produce a measurement.  |
| | **[sys_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-sys_t)**(std::size_t nU, std::size_t nX, std::size_t nY, data_t & dt, data_t & p0 =DEFAULT_P0, data_t & q0 =DEFAULT_Q0)<br>Constructs a new PLDS.  |
| [sys_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/) & | **[operator=](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-operator=)**(const [sys_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/) & sys) |
| size_t | **[getNy](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-getny)**() const<br>Get number of outputs (y)  |
| armaMat | **[getC](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-getc)**() const<br>Get output matrix (C)  |
| armaVec | **[getD](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-getd)**() const<br>Get output bias (D)  |
| armaVec | **[getY](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-gety)**() const<br>Get output (y)  |
| armaVec | **[getZ](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-getz)**() const<br>Get measurement (z)  |
| void | **[setDims](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-setdims)**(std::size_t & nU, std::size_t & nX, std::size_t & nY)<br>Set dimensions of sytem.  |
| void | **[setC](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-setc)**(stdVec & cVec)<br>Set output matrix (C)  |
| void | **[setC](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-setc)**(armaMat & C)<br>Set output matrix (C)  |
| void | **[setD](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-setd)**(stdVec & dVec)<br>Set output bias (d)  |
| void | **[setD](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-setd)**(armaVec & d)<br>Set output bias (d)  |
| void | **[setZ](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-setz)**(stdVec & zVec)<br>Set measurement (z)  |
| void | **[setZ](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-setz)**(armaVec & z)<br>Set measurement (z)  |
| void | **[reset](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-reset)**()<br>Reset system variables.  |
| void | **[printSys](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-printsys)**()<br>Print system variables to stdout.  |

## Protected Functions

|                | Name           |
| -------------- | -------------- |
| void | **[predict](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-predict)**()<br>One-step prediction.  |
| void | **[h](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#function-h)**()<br>System output function.  |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| std::size_t | **[nY](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#variable-ny)** <br>number of outputs  |
| armaMat | **[C](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#variable-c)** <br>output matrix  |
| armaVec | **[d](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#variable-d)** <br>output bias  |
| armaVec | **[y](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#variable-y)** <br>output  |
| armaVec | **[logy](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#variable-logy)** <br>logarithm of output  |
| armaVec | **[z](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#variable-z)** <br>measurement  |
| armaMat | **[diag_y](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#variable-diag_y)** <br>diagonal matrix with elements y  |
| armaVec | **[chance](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/#variable-chance)** <br>p.r. number for rolling dice if simulating data  |

## Additional inherited members

**Public Functions inherited from [lds::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/)**

|                | Name           |
| -------------- | -------------- |
| void | **[simPredict](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-simpredict)**()<br>Simulate a one-step prediction.  |
| size_t | **[getNx](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getnx)**() const<br>Get number of states.  |
| size_t | **[getNu](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getnu)**() const<br>Get number of inputs.  |
| armaVec | **[getU](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getu)**() const<br>Get current input (u)  |
| armaVec | **[getX](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getx)**()<br>Get current state (x)  |
| armaVec | **[getG](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getg)**() const<br>Get input gain (g)  |
| armaVec | **[getM](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getm)**() const<br>Get current process disturbance/bias (m)  |
| armaMat | **[getA](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-geta)**() const<br>Get state matrix (A)  |
| armaMat | **[getB](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getb)**() const<br>Get input matrix (B)  |
| armaMat | **[getQ](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getq)**() const<br>Get process noise covariance (Q)  |
| armaMat | **[getQ_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getq_m)**() const<br>Get process noise covariance acting on disturbance evolution (Q_m)  |
| armaMat | **[getP](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getp)**() const<br>Get covariance of state estimate (P)  |
| armaMat | **[getP_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getp_m)**() const<br>Get covariance of process disturbance estimate (P_m)  |
| armaVec | **[getX0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getx0)**() const<br>Get initial state (x0)  |
| armaMat | **[getP0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getp0)**() const<br>Get initial covariance of state estimate (P0)  |
| armaVec | **[getM0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getm0)**() const<br>Get initial process disturbance (m0)  |
| armaMat | **[getP0_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-getp0_m)**() const<br>Get initial covariance of process disturbance estimate (P0_m)  |
| void | **[setU](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setu)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & uVec)<br>Set input (u)  |
| void | **[setU](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setu)**(armaVec & u)<br>Set input (u)  |
| void | **[setA](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-seta)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & aVec)<br>Set state matrix (A)  |
| void | **[setA](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-seta)**(armaMat & A)<br>Set state matrix (A)  |
| void | **[setB](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setb)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & bVec)<br>Set input matrix (B)  |
| void | **[setB](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setb)**(armaMat & B)<br>Set input matrix (B)  |
| void | **[setM](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setm)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & mVec)<br>Set process disturbance (m)  |
| void | **[setM](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setm)**(armaVec & m)<br>Set process disturbance (m)  |
| void | **[setG](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setg)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & gVec)<br>Set input gain (g)  |
| void | **[setG](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setg)**(armaVec & g)<br>Set input gain (g)  |
| void | **[setQ](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setq)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & qVec)<br>Set process noise covariance (Q)  |
| void | **[setQ](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setq)**(armaMat & Q)<br>Set process noise covariance (Q)  |
| void | **[setQ_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setq_m)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & qmVec)<br>Set process noise covariance of disturbance evoluation (Q_m)  |
| void | **[setQ_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setq_m)**(armaMat & Q_m)<br>Set process noise covariance of disturbance evoluation (Q_m)  |
| void | **[setX0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setx0)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & x0Vec)<br>Set initial state (x0)  |
| void | **[setX0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setx0)**(armaVec & x0)<br>Set initial state (x0)  |
| void | **[setP0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setp0)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & p0Vec)<br>Set covariance of initial state (P0)  |
| void | **[setP0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setp0)**(armaMat & P0)<br>Set covariance of initial state (P0)  |
| void | **[setP0_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setp0_m)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & p0mVec)<br>Set covariance of initial process disturbance (P0_m)  |
| void | **[setP0_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setp0_m)**(armaMat & P0_m)<br>Set covariance of initial process disturbance (P0_m)  |

**Protected Functions inherited from [lds::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/)**

|                | Name           |
| -------------- | -------------- |
| void | **[reassign](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-reassign)**(armaVec & oldVar, armaVec & newVar, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) defaultVal =0) |
| void | **[reassign](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-reassign)**(armaVec & oldVar, [stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & newVar, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) defaultVal =0) |
| void | **[reassign](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-reassign)**(armaSubVec & oldVar, armaVec & newVar, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) defaultVal =0) |
| void | **[reassign](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-reassign)**(armaSubVec & oldVar, [stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & newVar, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) defaultVal =0) |
| void | **[reassign](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-reassign)**(armaMat & oldVar, armaMat & newVar, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) defaultVal =0) |
| void | **[reassign](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-reassign)**(armaMat & oldVar, [stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & newVar, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) defaultVal =0) |
| void | **[reassign](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-reassign)**(armaSubMat & oldVar, armaMat & newVar, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) defaultVal =0) |
| void | **[reassign](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-reassign)**(armaSubMat & oldVar, [stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & newVar, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) defaultVal =0) |
| void | **[limit](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-limit)**([stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) & x, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & lb, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & ub) |
| void | **[limit](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-limit)**(armaVec & x, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & lb, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & ub) |
| void | **[limit](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-limit)**(armaMat & x, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & lb, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & ub) |
| void | **[defaultQ](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-defaultq)**()<br>Reset to default Q (identity matrix with diagonal elements q0)  |
| void | **[checkP](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-checkp)**()<br>check whether estimate covariance fals within plim upper bound  |

**Public Attributes inherited from [lds::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/)**

|                | Name           |
| -------------- | -------------- |
| bool | **[adaptM](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-adaptm)**  |

**Protected Attributes inherited from [lds::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/)**

|                | Name           |
| -------------- | -------------- |
| armaVec | **[u](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-u)** <br>input  |
| armaVec | **[x](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-x)** <br>state  |
| armaMat | **[P](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-p)** <br>covariance of state estimate  |
| armaVec | **[m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-m)** <br>process disturbance  |
| armaMat | **[P_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-p_m)** <br>covariance of disturbance estimate  |
| armaVec | **[x0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-x0)** <br>initial state  |
| armaMat | **[P0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-p0)** <br>covariance of initial state estimate  |
| armaVec | **[m0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-m0)** <br>initial process disturbance  |
| armaMat | **[P0_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-p0_m)** <br>covariance of initial disturbance estimate  |
| armaMat | **[A](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-a)** <br>state matrix  |
| armaMat | **[B](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-b)** <br>input matrix  |
| armaVec | **[g](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-g)** <br>input gain  |
| armaMat | **[Q](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-q)** <br>covariance of process noise  |
| armaMat | **[Q_m](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-q_m)** <br>covoariance of disturbance evolution process  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & | **[dt](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-dt)** <br>sample period  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & | **[q0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-q0)** <br>default process noise covariance  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & | **[p0](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-p0)** <br>default state estimate covariance  |
| std::size_t | **[nX](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-nx)** <br>number of states  |
| std::size_t | **[nU](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-nu)** <br>number of inputs  |
| bool | **[szChanged](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-szchanged)** <br>whether size of system changed (see `setDims`)  |
| const [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) | **[plim](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-plim)** <br>limit for state estimate covariance  |


---
---
## Public Function Details

### **filter**

```cpp
void filter(
    armaVec & u_tm1,
    armaVec & z_t
)
```



**Parameters**:

  * **u_tm1** input at t-minus-1 
  * **z_t** current measurement 


Given current measurment and input, filter data to produce causal state estimates using a point-process filter, which procedes by predicting the state and subsequently updating.

References:

Smith AC, Brown EN. (2003) Estimating a State-Space Model from Point Process Observations. Neural Computation 15.

Eden UT, ..., Brown EN. (2004) Dynamic Analysis of Neural Encoding by Point Process Adaptive Filtering Neural Computation 16.


---
### **filter**

```cpp
void filter(
    armaVec & z
)
```



**Parameters**:

  * **z** current measurement 


Given current measurment and input, filter data to produce causal state estimates using a point-process filter, which procedes by predicting the state and subsequently updating.

References:

Smith AC, Brown EN. (2003) Estimating a State-Space Model from Point Process Observations. Neural Computation 15.

Eden UT, ..., Brown EN. (2004) Dynamic Analysis of Neural Encoding by Point Process Adaptive Filtering Neural Computation 16.


---
### **simMeasurement**

```cpp
void simMeasurement(
    armaVec & z
)
```



**Parameters**:

  * **z** measurement 


---
### **sys_t**

```cpp
sys_t(
    std::size_t nU,
    std::size_t nX,
    std::size_t nY,
    data_t & dt,
    data_t & p0 =DEFAULT_P0,
    data_t & q0 =DEFAULT_Q0
)
```



**Parameters**:

  * **nU** number of inputs 
  * **nX** number of states 
  * **nY** number of outputs 
  * **dt** sample period 
  * **p0** [optional] initial diagonal elements of state estimate covariance (P) 
  * **q0** [optional] initial diagonal elements of process noise covariance (Q) 


---
### **operator=**

```cpp
sys_t & operator=(
    const sys_t & sys
)
```



---
### **getNy**

```cpp
inline size_t getNy() const
```



---
### **getC**

```cpp
inline armaMat getC() const
```



---
### **getD**

```cpp
inline armaVec getD() const
```



---
### **getY**

```cpp
inline armaVec getY() const
```



---
### **getZ**

```cpp
inline armaVec getZ() const
```



---
### **setDims**

```cpp
void setDims(
    std::size_t & nU,
    std::size_t & nX,
    std::size_t & nY
)
```



---
### **setC**

```cpp
void setC(
    stdVec & cVec
)
```



---
### **setC**

```cpp
void setC(
    armaMat & C
)
```



---
### **setD**

```cpp
void setD(
    stdVec & dVec
)
```



---
### **setD**

```cpp
void setD(
    armaVec & d
)
```



---
### **setZ**

```cpp
void setZ(
    stdVec & zVec
)
```



---
### **setZ**

```cpp
void setZ(
    armaVec & z
)
```



---
### **reset**

```cpp
void reset()
```



---
### **printSys**

```cpp
void printSys()
```



---


## Protected Function Details

### **predict**

```cpp
void predict()
```



---
### **h**

```cpp
void h()
```



---


## Protected Attribute Details

### **nY**

```cpp
std::size_t nY;
```



---
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
### **y**

```cpp
armaVec y;
```



---
### **logy**

```cpp
armaVec logy;
```



---
### **z**

```cpp
armaVec z;
```



---
### **diag_y**

```cpp
armaMat diag_y;
```



---
### **chance**

```cpp
armaVec chance;
```



---


-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
