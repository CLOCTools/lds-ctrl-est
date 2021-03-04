---
title: lds::sys_t
summary: Linear Dynamical System Type. 

---

# lds::sys_t



Linear Dynamical System Type. 
<br /> `#include <lds_sys.h>`

Inherited by [lds::gaussian::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/), [lds::poisson::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-sys_t)**(std::size_t nU, std::size_t nX, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & dt, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & p0 =DEFAULT_P0, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) & q0 =DEFAULT_Q0)<br>Constructs a new LDS.  |
| [sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/) & | **[operator=](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-operator=)**(const [sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/) & sys) |
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
| void | **[setDims](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-setdims)**(std::size_t & nU, std::size_t & nX)<br>Set dimensions of system.  |
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
| void | **[reset](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-reset)**()<br>Reset system variables.  |
| void | **[printSys](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-printsys)**()<br>Print system variables to stdout.  |

## Protected Functions

|                | Name           |
| -------------- | -------------- |
| void | **[predict](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#function-predict)**()<br>one-step prediction of state  |
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

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| bool | **[adaptM](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/#variable-adaptm)**  |

## Protected Attributes

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

### **sys_t**

```cpp
sys_t(
    std::size_t nU,
    std::size_t nX,
    data_t & dt,
    data_t & p0 =DEFAULT_P0,
    data_t & q0 =DEFAULT_Q0
)
```



**Parameters**:

  * **nU** number if inputs (u) 
  * **nX** number of state (x) 
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
### **simPredict**

```cpp
void simPredict()
```



---
### **getNx**

```cpp
inline size_t getNx() const
```



---
### **getNu**

```cpp
inline size_t getNu() const
```



---
### **getU**

```cpp
inline armaVec getU() const
```



---
### **getX**

```cpp
inline armaVec getX()
```



---
### **getG**

```cpp
inline armaVec getG() const
```



---
### **getM**

```cpp
inline armaVec getM() const
```



---
### **getA**

```cpp
inline armaMat getA() const
```



---
### **getB**

```cpp
inline armaMat getB() const
```



---
### **getQ**

```cpp
inline armaMat getQ() const
```



---
### **getQ_m**

```cpp
inline armaMat getQ_m() const
```



---
### **getP**

```cpp
inline armaMat getP() const
```



---
### **getP_m**

```cpp
inline armaMat getP_m() const
```



---
### **getX0**

```cpp
inline armaVec getX0() const
```



---
### **getP0**

```cpp
inline armaMat getP0() const
```



---
### **getM0**

```cpp
inline armaVec getM0() const
```



---
### **getP0_m**

```cpp
inline armaMat getP0_m() const
```



---
### **setDims**

```cpp
void setDims(
    std::size_t & nU,
    std::size_t & nX
)
```



---
### **setU**

```cpp
void setU(
    stdVec & uVec
)
```



---
### **setU**

```cpp
void setU(
    armaVec & u
)
```



---
### **setA**

```cpp
void setA(
    stdVec & aVec
)
```



---
### **setA**

```cpp
void setA(
    armaMat & A
)
```



---
### **setB**

```cpp
void setB(
    stdVec & bVec
)
```



---
### **setB**

```cpp
void setB(
    armaMat & B
)
```



---
### **setM**

```cpp
void setM(
    stdVec & mVec
)
```



---
### **setM**

```cpp
void setM(
    armaVec & m
)
```



---
### **setG**

```cpp
void setG(
    stdVec & gVec
)
```



---
### **setG**

```cpp
void setG(
    armaVec & g
)
```



---
### **setQ**

```cpp
void setQ(
    stdVec & qVec
)
```



---
### **setQ**

```cpp
void setQ(
    armaMat & Q
)
```



---
### **setQ_m**

```cpp
void setQ_m(
    stdVec & qmVec
)
```



---
### **setQ_m**

```cpp
void setQ_m(
    armaMat & Q_m
)
```



---
### **setX0**

```cpp
void setX0(
    stdVec & x0Vec
)
```



---
### **setX0**

```cpp
void setX0(
    armaVec & x0
)
```



---
### **setP0**

```cpp
void setP0(
    stdVec & p0Vec
)
```



---
### **setP0**

```cpp
void setP0(
    armaMat & P0
)
```



---
### **setP0_m**

```cpp
void setP0_m(
    stdVec & p0mVec
)
```



---
### **setP0_m**

```cpp
void setP0_m(
    armaMat & P0_m
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
### **reassign**

```cpp
void reassign(
    armaVec & oldVar,
    armaVec & newVar,
    data_t defaultVal =0
)
```



---
### **reassign**

```cpp
void reassign(
    armaVec & oldVar,
    stdVec & newVar,
    data_t defaultVal =0
)
```



---
### **reassign**

```cpp
void reassign(
    armaSubVec & oldVar,
    armaVec & newVar,
    data_t defaultVal =0
)
```



---
### **reassign**

```cpp
void reassign(
    armaSubVec & oldVar,
    stdVec & newVar,
    data_t defaultVal =0
)
```



---
### **reassign**

```cpp
void reassign(
    armaMat & oldVar,
    armaMat & newVar,
    data_t defaultVal =0
)
```



---
### **reassign**

```cpp
void reassign(
    armaMat & oldVar,
    stdVec & newVar,
    data_t defaultVal =0
)
```



---
### **reassign**

```cpp
void reassign(
    armaSubMat & oldVar,
    armaMat & newVar,
    data_t defaultVal =0
)
```



---
### **reassign**

```cpp
void reassign(
    armaSubMat & oldVar,
    stdVec & newVar,
    data_t defaultVal =0
)
```



---
### **limit**

```cpp
void limit(
    stdVec & x,
    data_t & lb,
    data_t & ub
)
```



---
### **limit**

```cpp
void limit(
    armaVec & x,
    data_t & lb,
    data_t & ub
)
```



---
### **limit**

```cpp
void limit(
    armaMat & x,
    data_t & lb,
    data_t & ub
)
```



---
### **defaultQ**

```cpp
void defaultQ()
```



---
### **checkP**

```cpp
void checkP()
```



---


## Public Attribute Details

### **adaptM**

```cpp
bool adaptM;
```



---


## Protected Attribute Details

### **u**

```cpp
armaVec u;
```



---
### **x**

```cpp
armaVec x;
```



---
### **P**

```cpp
armaMat P;
```



---
### **m**

```cpp
armaVec m;
```



---
### **P_m**

```cpp
armaMat P_m;
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
### **m0**

```cpp
armaVec m0;
```



---
### **P0_m**

```cpp
armaMat P0_m;
```



---
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
### **Q**

```cpp
armaMat Q;
```



---
### **Q_m**

```cpp
armaMat Q_m;
```



---
### **dt**

```cpp
data_t & dt;
```



---
### **q0**

```cpp
data_t & q0;
```



---
### **p0**

```cpp
data_t & p0;
```



---
### **nX**

```cpp
std::size_t nX;
```



---
### **nU**

```cpp
std::size_t nU;
```



---
### **szChanged**

```cpp
bool szChanged;
```



---
### **plim**

```cpp
const data_t plim = 1e2;
```



---


-------------------------------

Updated on  3 March 2021 at 23:06:11 CST
