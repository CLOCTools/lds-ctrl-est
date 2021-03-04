---
title: lds::gaussian
summary: Linear Dynamical Systems with Gaussian observations. 

---

# lds::gaussian

Linear Dynamical Systems with Gaussian observations. <br>

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::gaussian::ctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ctrl__t/)** <br>GLDS Controller Type.  |
| class | **[lds::gaussian::fit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1fit__t/)** <br>GLDS Fit Type.  |
| class | **[lds::gaussian::emFit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/)** <br>GLDS E-M Fit Type.  |
| class | **[lds::gaussian::ssidFit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ssid_fit__t/)** <br>GLDS SSID Fit Type.  |
| class | **[lds::gaussian::sctrl_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sctrl__t/)** <br>Switched GLDS Controller Type.  |
| class | **[lds::gaussian::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/)** <br>Gaussian LDS Type.  |

## Types

|                | Name           |
| -------------- | -------------- |
| typedef [lds::data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) | **[data_t](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#typedef-data_t)**  |
| typedef [lds::stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec) | **[stdVec](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#typedef-stdvec)**  |
| typedef lds::armaVec | **[armaVec](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#typedef-armavec)**  |
| typedef lds::armaMat | **[armaMat](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#typedef-armamat)**  |
| typedef lds::armaSubVec | **[armaSubVec](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#typedef-armasubvec)**  |
| typedef lds::armaSubMat | **[armaSubMat](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#typedef-armasubmat)**  |

## Functions

|                | Name           |
| -------------- | -------------- |
| [emFit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/) | **[emFit_x_equals_y](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#function-emfit_x_equals_y)**(std::vector< armaMat > & uTrain, std::vector< armaMat > & zTrain, data_t dt, std::size_t maxIter =100, data_t tol =1e-3, data_t q0 =1e-6, bool calcAB =true, bool calcQ =true, bool calcR =true)<br>Perform EM for special system where y = x + noise.  |
| [ssidFit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ssid_fit__t/) | **[ssidFit](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#function-ssidfit)**(std::vector< armaMat > & u, std::vector< armaMat > & z, data_t dt, size_t nX, size_t nH =25, armaVec d0 =armaVec(1).fill(-inf), bool force_unitNormC =false, [ssidWt](/ldsctrlest/docs/api/namespaces/namespacelds/#enum-ssidwt) wt =NONE, data_t wtG0 =0.0, std::vector< data_t > & t0 =DEFAULT_T0, data_t t_startSSID =-std::numeric_limits< data_t >::infinity(), data_t t_stopSSID =std::numeric_limits< data_t >::infinity())<br>Fit a GLDS model using subspace identification (SSID)  |
| armaVec | **[calcD_silence](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#function-calcd_silence)**(std::vector< armaMat > & n, std::vector< armaMat > & u, data_t dt, data_t tSilence =0.1, data_t threshSilence =0.001)<br>Using periods of silence in inputs (u), calculates the output \ bias.  |
| void | **[createHankelMat_n4sid](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#function-createhankelmat_n4sid)**(armaMat & u, armaMat & z, std::size_t & nH, armaMat & D)<br>Create block-hankel data matrix for N4SID method.  |
| void | **[ssid_n4sid](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#function-ssid_n4sid)**(std::size_t & nX, std::size_t & nU, std::size_t & nY, std::size_t & nH, armaMat & uSSID, armaMat & zSSID, armaMat & A, armaMat & B, armaMat & C, armaMat & D, armaMat & Q, armaMat & R, armaVec & m, armaVec & d, armaVec & s, bool force_unitNormC, [ssidWt](/ldsctrlest/docs/api/namespaces/namespacelds/#enum-ssidwt) wt, data_t wtG0)<br>N4SID method of SSID.  |
| void | **[ssid_n4sid_vanOverschee](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#function-ssid_n4sid_vanoverschee)**(std::size_t & nX, std::size_t & nU, std::size_t & nY, std::size_t & nH, armaMat & uSSID, armaMat & zSSID, armaMat & A, armaMat & B, armaMat & C, armaMat & D, armaMat & Q, armaMat & R, armaVec & s, [ssidWt](/ldsctrlest/docs/api/namespaces/namespacelds/#enum-ssidwt) wt)<br>N4SID method of SSID.  |
| void | **[recompute_extObs](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#function-recompute_extobs)**(armaMat & extObs, armaMat & A, armaMat & C, std::size_t & nY, std::size_t & nH)<br>Recompute extended observability matrix, given A, C.  |
| void | **[lq](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#function-lq)**(armaMat & L, armaMat & Qt, armaMat & X)<br>LQ decomposition.  |
| armaMat | **[calcCov](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#function-calccov)**(armaMat & A, armaMat & B)<br>Calculate covariance matrix.  |
| armaMat | **[sqrtmat_svd](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#function-sqrtmat_svd)**(armaMat & X) |

## Attributes

|                | Name           |
| -------------- | -------------- |
| const std::size_t | **[CONTROL_TYPE_U](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#variable-control_type_u)**  |
| const std::size_t | **[CONTROL_TYPE_INTY](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#variable-control_type_inty)**  |
| const std::size_t | **[CONTROL_TYPE_ADAPT_M](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#variable-control_type_adapt_m)**  |
| data_t | **[DATA_T_ZERO](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#variable-data_t_zero)**  |
| data_t | **[DATA_T_ONE](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#variable-data_t_one)**  |
| data_t | **[DEFAULT_SOFTSTART](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#variable-default_softstart)**  |
| bool | **[FALSE](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#variable-false)**  |
| bool | **[TRUE](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#variable-true)**  |
| const data_t | **[pi](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#variable-pi)**  |
| data_t | **[inf](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#variable-inf)**  |
| data_t | **[neginf](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/#variable-neginf)**  |

## Type Details

### data_t

```cpp
typedef lds::data_t lds::gaussian::data_t;
```



### stdVec

```cpp
typedef lds::stdVec lds::gaussian::stdVec;
```



### armaVec

```cpp
typedef lds::armaVec lds::gaussian::armaVec;
```



### armaMat

```cpp
typedef lds::armaMat lds::gaussian::armaMat;
```



### armaSubVec

```cpp
typedef lds::armaSubVec lds::gaussian::armaSubVec;
```



### armaSubMat

```cpp
typedef lds::armaSubMat lds::gaussian::armaSubMat;
```




## Function Details

### emFit_x_equals_y

```cpp
emFit_t emFit_x_equals_y(
    std::vector< armaMat > & uTrain,
    std::vector< armaMat > & zTrain,
    data_t dt,
    std::size_t maxIter =100,
    data_t tol =1e-3,
    data_t q0 =1e-6,
    bool calcAB =true,
    bool calcQ =true,
    bool calcR =true
)
```



**Parameters**:

  * **uTrain** training input data 
  * **zTrain** training measurement data 
  * **dt** sample period 
  * **maxIter** [optional] maximum number of iterations 
  * **tol** [optional] convergence tolerance 
  * **q0** [optional] initial diagonal elements of process noise covariance 
  * **calcAB** [optional] whether to calculate dynamics (A, B) 
  * **calcQ** [optional] whether to calculate process noise covariance 
  * **calcR** [optional] whether to calculate output noise covariance


**Return**: EM fit 

### ssidFit

```cpp
ssidFit_t ssidFit(
    std::vector< armaMat > & u,
    std::vector< armaMat > & z,
    data_t dt,
    size_t nX,
    size_t nH =25,
    armaVec d0 =armaVec(1).fill(-inf),
    bool force_unitNormC =false,
    ssidWt wt =NONE,
    data_t wtG0 =0.0,
    std::vector< data_t > & t0 =DEFAULT_T0,
    data_t t_startSSID =-std::numeric_limits< data_t >::infinity(),
    data_t t_stopSSID =std::numeric_limits< data_t >::infinity()
)
```



**Parameters**:

  * **u** inputs 
  * **z** measurements 
  * **dt** sample period 
  * **nX** number of states 
  * **nH** block-Hankel data matrix size 
  * **d0** previous estimate of output bias (d) 
  * **force_unitNormC** force fitting where C has unit norm 
  * **wt** chosen weighting for SSID singular value \ decomposition (NONE, MOESP, CVA) 
  * **wtG0** chosen weighting for minimizing model error at \ DC 
  * **t0** start times for each trial of input/output data 
  * **t_startSSID** start time for the data used in SSID 
  * **t_stopSSID** stop time for the data used in SSID


**Return**: the SSID-fit GLDS model. 

Fit a GLDS model using subspace identification (SSID)

References:

van Overschee P, de Moor B. (1996) Subspace Identification for Linear Systems.


### calcD_silence

```cpp
armaVec calcD_silence(
    std::vector< armaMat > & n,
    std::vector< armaMat > & u,
    data_t dt,
    data_t tSilence =0.1,
    data_t threshSilence =0.001
)
```



**Parameters**:

  * **n** measurements 
  * **u** inputs 
  * **dt** sample period 
  * **tSilence** threshold on period of time that qualifies as "silence" 
  * **threshSilence** threshold on input amplitude u that qualifies as "silence"


**Return**: d estimated output bias. 

### createHankelMat_n4sid

```cpp
void createHankelMat_n4sid(
    armaMat & u,
    armaMat & z,
    std::size_t & nH,
    armaMat & D
)
```



**Parameters**:

  * **u** input data 
  * **z** measurement data 
  * **nH** block-Hankel matrix size 
  * **D** block-Hankel data matrix 


### ssid_n4sid

```cpp
void ssid_n4sid(
    std::size_t & nX,
    std::size_t & nU,
    std::size_t & nY,
    std::size_t & nH,
    armaMat & uSSID,
    armaMat & zSSID,
    armaMat & A,
    armaMat & B,
    armaMat & C,
    armaMat & D,
    armaMat & Q,
    armaMat & R,
    armaVec & m,
    armaVec & d,
    armaVec & s,
    bool force_unitNormC,
    ssidWt wt,
    data_t wtG0
)
```



**Parameters**:

  * **nX** number of states 
  * **nU** number of inputs 
  * **nY** number of outputs 
  * **nH** block-Hankel matrix size 
  * **uSSID** input data 
  * **zSSID** measurement data 
  * **A** state matrix 
  * **B** input matrix 
  * **C** output matrix 
  * **D** feedthrough matrix 
  * **Q** process noise covariance 
  * **R** output noise covariance 
  * **m** process disturbance 
  * **d** output bias 
  * **s** singular values 
  * **force_unitNormC** whether to force C to be unit-norm 
  * **wt** weighting method for SVD (`NONE`, `MOESP`, `CVA`) 
  * **wtG0** weight put on minimizing error at DC 


### ssid_n4sid_vanOverschee

```cpp
void ssid_n4sid_vanOverschee(
    std::size_t & nX,
    std::size_t & nU,
    std::size_t & nY,
    std::size_t & nH,
    armaMat & uSSID,
    armaMat & zSSID,
    armaMat & A,
    armaMat & B,
    armaMat & C,
    armaMat & D,
    armaMat & Q,
    armaMat & R,
    armaVec & s,
    ssidWt wt
)
```



**Parameters**:

  * **nX** number of state 
  * **nU** number of inputs 
  * **nY** number of outputs 
  * **nH** block-Hankel data matrix size 
  * **uSSID** input data 
  * **zSSID** measurement data 
  * **A** state matrix 
  * **B** input matrix 
  * **C** output matrix 
  * **D** feedthrough matrix 
  * **Q** process noise covariance 
  * **R** output noise covariance 
  * **s** singular values 
  * **wt** weighting method for SVD (`NONE`, `MOESP`, `CVA`) 


N4SID method of SSID

This is a more direct implementation of van Overschee, de Moor (1996)


### recompute_extObs

```cpp
void recompute_extObs(
    armaMat & extObs,
    armaMat & A,
    armaMat & C,
    std::size_t & nY,
    std::size_t & nH
)
```



**Parameters**:

  * **extObs** Extended observability matrix 
  * **A** state matrix 
  * **C** output matrix 
  * **nY** number of outputs 
  * **nH** block-Hankel data matrix size 


### lq

```cpp
void lq(
    armaMat & L,
    armaMat & Qt,
    armaMat & X
)
```



**Parameters**:

  * **L** lower triangle matrix 
  * **Qt** orthonormal matrix (transposed cf QR decomp) 
  * **X** matrix being decomposed 


### calcCov

```cpp
armaMat calcCov(
    armaMat & A,
    armaMat & B
)
```



**Parameters**:

  * **A** some matrix 
  * **B** some other matrix


**Return**: covariance 

### sqrtmat_svd

```cpp
armaMat sqrtmat_svd(
    armaMat & X
)
```



**Parameters**:

  * **X** some matrix


**Return**: approximate sqrtmat(X) 

Approximate sqrtmat by singular value decomposition.

svd(X) --> U * sqrt(s) * V' ~= sqrtmat(X)



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

Updated on  3 March 2021 at 23:06:11 CST
