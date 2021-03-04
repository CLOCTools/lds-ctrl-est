---
title: lds::poisson
summary: Linear Dynamical Systems with Poisson observations. 

---

# lds::poisson

Linear Dynamical Systems with Poisson observations. <br>

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::poisson::ctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ctrl__t/)** <br>PLDS Controller Type.  |
| class | **[lds::poisson::fit_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1fit__t/)** <br>PLDS Fit Type.  |
| class | **[lds::poisson::ssidFit_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ssid_fit__t/)** <br>GLDS SSID Fit Type.  |
| class | **[lds::poisson::sctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sctrl__t/)** <br>Switched PLDS Controller Type.  |
| class | **[lds::poisson::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/)** <br>Poisson LDS Type.  |

## Types

|                | Name           |
| -------------- | -------------- |
| typedef [lds::data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) | **[data_t](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#typedef-data_t)**  |
| typedef [lds::stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) | **[stdVec](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#typedef-stdvec)**  |
| typedef lds::armaVec | **[armaVec](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#typedef-armavec)**  |
| typedef lds::armaMat | **[armaMat](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#typedef-armamat)**  |
| typedef lds::armaSubVec | **[armaSubVec](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#typedef-armasubvec)**  |
| typedef lds::armaSubMat | **[armaSubMat](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#typedef-armasubmat)**  |

## Functions

|                | Name           |
| -------------- | -------------- |
| std::tuple< [ssidFit_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ssid_fit__t/), [lds::gaussian::ssidFit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ssid_fit__t/) > | **[ssidFit](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#function-ssidfit)**(std::vector< armaMat > & u, std::vector< armaMat > & z, data_t dt, size_t nX, size_t nH =25, armaVec d0 =armaVec(1).fill(-inf), [ssidWt](/ldsctrlest/docs/api/namespaces/namespacelds/#enum-ssidwt) wt =NONE, data_t wtG0 =0.0, std::vector< data_t > & t0 =DEFAULT_T0, data_t t_startSSID =-std::numeric_limits< data_t >::infinity(), data_t t_stopSSID =std::numeric_limits< data_t >::infinity(), bool assumeGaussian =true)<br>Fit a PLDS model using subspace identification (SSID)  |
| void | **[newtonSolveC_mle](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#function-newtonsolvec_mle)**(armaMat & C, armaVec & d, std::vector< armaMat > & x, std::vector< armaMat > & n, data_t & dt)<br>Solve for output matrix by Poisson MLE.  |
| data_t | **[newtonSolve_rescaleC_mle](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#function-newtonsolve_rescalec_mle)**(armaMat & C, armaVec & d, std::vector< armaMat > & x, std::vector< armaMat > & n, data_t & dt)<br>Solve for rescaled output matrix by Poisson MLE.  |
| void | **[calcD_mle](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#function-calcd_mle)**(armaMat & C, armaVec & d, std::vector< armaMat > & x, std::vector< armaMat > & n, data_t & dt)<br>Calculates output bias by Poisson MLE.  |
| data_t | **[calcD_newtonSolve_rescaleC_mle](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#function-calcd_newtonsolve_rescalec_mle)**(armaMat & C, armaVec & d, std::vector< armaMat > & x, std::vector< armaMat > & n, data_t & dt)<br>Calculates output bias and rescaled output matrix by Poisson MLE.  |
| void | **[fitOutput_mle](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#function-fitoutput_mle)**(armaMat & C, armaVec & d, std::vector< armaMat > & x, std::vector< armaMat > & n, data_t & dt)<br>Calculates exponential output function by Poisson MLE.  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| const std::size_t | **[CONTROL_TYPE_U](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#variable-control_type_u)**  |
| const std::size_t | **[CONTROL_TYPE_INTY](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#variable-control_type_inty)**  |
| const std::size_t | **[CONTROL_TYPE_ADAPT_M](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#variable-control_type_adapt_m)**  |
| data_t | **[DATA_T_ZERO](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#variable-data_t_zero)**  |
| data_t | **[DATA_T_ONE](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#variable-data_t_one)**  |
| data_t | **[DEFAULT_SOFTSTART](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#variable-default_softstart)**  |
| bool | **[FALSE](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#variable-false)**  |
| bool | **[TRUE](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#variable-true)**  |
| const data_t | **[pi](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#variable-pi)**  |
| data_t | **[inf](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#variable-inf)**  |
| data_t | **[neginf](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/#variable-neginf)**  |

## Type Details

### data_t

```cpp
typedef lds::data_t lds::poisson::data_t;
```



### stdVec

```cpp
typedef lds::stdVec lds::poisson::stdVec;
```



### armaVec

```cpp
typedef lds::armaVec lds::poisson::armaVec;
```



### armaMat

```cpp
typedef lds::armaMat lds::poisson::armaMat;
```



### armaSubVec

```cpp
typedef lds::armaSubVec lds::poisson::armaSubVec;
```



### armaSubMat

```cpp
typedef lds::armaSubMat lds::poisson::armaSubMat;
```




## Function Details

### ssidFit

```cpp
std::tuple< ssidFit_t, lds::gaussian::ssidFit_t > ssidFit(
    std::vector< armaMat > & u,
    std::vector< armaMat > & z,
    data_t dt,
    size_t nX,
    size_t nH =25,
    armaVec d0 =armaVec(1).fill(-inf),
    ssidWt wt =NONE,
    data_t wtG0 =0.0,
    std::vector< data_t > & t0 =DEFAULT_T0,
    data_t t_startSSID =-std::numeric_limits< data_t >::infinity(),
    data_t t_stopSSID =std::numeric_limits< data_t >::infinity(),
    bool assumeGaussian =true
)
```



**Parameters**:

  * **u** inputs 
  * **n** { parameter_description } 
  * **dt** sample period 
  * **nX** number of states 
  * **nH** block-Hankel data matrix size 
  * **d0** previous estimate of output bias (d) 
  * **wt** chosen weighting for SSID singular value \ decomposition (NONE, MOESP, CVA) 
  * **wtG0** chosen weighting for minimizing model error at \ DC 
  * **t0** start times for each trial of input/output data 
  * **t_startSSID** start time for the data used in SSID 
  * **t_stopSSID** stop time for the data used in SSID 
  * **assumeGaussian** [NONFUNCTIONAL] whether to assume Gaussian observation model and fit by linear SSID first, then fit output nonlinearity by Poisson MLE 
  * **z** measurements


**Return**: tuple<PLDS fit, GLDS fit> 

Fit PLDS model by subspace identification (SSID). System matrices (A,B,C) are fit by SSID assuming gaussian observations. Iteratively refits output function parameters (C, d) by Poisson MLE using Newton's method.


### newtonSolveC_mle

```cpp
void newtonSolveC_mle(
    armaMat & C,
    armaVec & d,
    std::vector< armaMat > & x,
    std::vector< armaMat > & n,
    data_t & dt
)
```



**Parameters**:

  * **C** output matrix 
  * **d** output bias 
  * **x** state data 
  * **n** measured count data 
  * **dt** sample period 


Given dynamics and initial guess of output, solves for output matrix (C) by maximizing Poisson maximum likelihood using Newton's method.


### newtonSolve_rescaleC_mle

```cpp
data_t newtonSolve_rescaleC_mle(
    armaMat & C,
    armaVec & d,
    std::vector< armaMat > & x,
    std::vector< armaMat > & n,
    data_t & dt
)
```



**Parameters**:

  * **C** output matrix 
  * **d** output bias 
  * **x** state data 
  * **n** measured count data 
  * **dt** sample period


**Return**: negative log-likelihood 

Given dynamics and initial guess of output, solves for a scaling factor to apply to output matrix (C) by maximizing Poisson maximum likelihood using Newton's method.

n.b., see Bussgang theorem (1952)


### calcD_mle

```cpp
void calcD_mle(
    armaMat & C,
    armaVec & d,
    std::vector< armaMat > & x,
    std::vector< armaMat > & n,
    data_t & dt
)
```



**Parameters**:

  * **C** output matrix 
  * **d** output bias 
  * **x** state data 
  * **n** measured count data 
  * **dt** sample period 


### calcD_newtonSolve_rescaleC_mle

```cpp
data_t calcD_newtonSolve_rescaleC_mle(
    armaMat & C,
    armaVec & d,
    std::vector< armaMat > & x,
    std::vector< armaMat > & n,
    data_t & dt
)
```



**Parameters**:

  * **C** output matrix 
  * **d** output bias 
  * **x** state data 
  * **n** measured count data 
  * **dt** sample period


**Return**: negative log-likelihood 

### fitOutput_mle

```cpp
void fitOutput_mle(
    armaMat & C,
    armaVec & d,
    std::vector< armaMat > & x,
    std::vector< armaMat > & n,
    data_t & dt
)
```



**Parameters**:

  * **C** output matrix 
  * **d** output bias 
  * **x** state data 
  * **n** measured count data 
  * **dt** sample period 



## Attribute Details

### CONTROL_TYPE_U

```cpp
static const std::size_t CONTROL_TYPE_U = lds::CONTROL_TYPE_U;
```



### CONTROL_TYPE_INTY

```cpp
static const std::size_t CONTROL_TYPE_INTY = lds::CONTROL_TYPE_INTY;
```



### CONTROL_TYPE_ADAPT_M

```cpp
static const std::size_t CONTROL_TYPE_ADAPT_M = lds::CONTROL_TYPE_ADAPT_M;
```



### DATA_T_ZERO

```cpp
static data_t DATA_T_ZERO = lds::DATA_T_ZERO;
```



### DATA_T_ONE

```cpp
static data_t DATA_T_ONE = lds::DATA_T_ZERO;
```



### DEFAULT_SOFTSTART

```cpp
static data_t DEFAULT_SOFTSTART = lds::DEFAULT_SOFTSTART;
```



### FALSE

```cpp
static bool FALSE = lds::FALSE;
```



### TRUE

```cpp
static bool TRUE = lds::TRUE;
```



### pi

```cpp
static const data_t pi = lds::pi;
```



### inf

```cpp
static data_t inf = lds::inf;
```



### neginf

```cpp
static data_t neginf = lds::neginf;
```







-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
