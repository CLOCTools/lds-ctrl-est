---
title: lds
summary: Linear Dynamical Systems (LDS) namespace. 

---

# lds

Linear Dynamical Systems (LDS) namespace. <br>

## Namespaces

| Name           |
| -------------- |
| **[lds::gaussian](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/)** <br>Linear Dynamical Systems with Gaussian observations.  |
| **[lds::poisson](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/)** <br>Linear Dynamical Systems with Poisson observations.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::fit_t](/ldsctrlest/docs/api/classes/classlds_1_1fit__t/)** <br>LDS Fit Type.  |
| class | **[lds::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/)** <br>Linear Dynamical System Type.  |

## Types

|                | Name           |
| -------------- | -------------- |
| enum| **[ssidWt](/ldsctrlest/docs/api/namespaces/namespacelds/#enum-ssidwt)** { NONE, MOESP, CVA} |
| typedef double | **[data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t)**  |
| typedef std::vector< [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) > | **[stdVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-stdvec)** <br>provides fill types for constructing new armadillo vectors, matrices  |
| typedef arma::Col< [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) > | **[armaVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-armavec)**  |
| typedef arma::Mat< [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) > | **[armaMat](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-armamat)**  |
| typedef arma::Cube< [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) > | **[armaCube](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-armacube)**  |
| typedef arma::subview_col< [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) > | **[armaSubVec](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-armasubvec)**  |
| typedef arma::subview< [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) > | **[armaSubMat](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-armasubmat)**  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| const std::size_t | **[CONTROL_TYPE_U](/ldsctrlest/docs/api/modules/group__control__masks/#variable-control_type_u)**  |
| const std::size_t | **[CONTROL_TYPE_INTY](/ldsctrlest/docs/api/modules/group__control__masks/#variable-control_type_inty)**  |
| const std::size_t | **[CONTROL_TYPE_ADAPT_M](/ldsctrlest/docs/api/modules/group__control__masks/#variable-control_type_adapt_m)** <br>Control setpoint adapted with (re-estimated) process disturbance `m`.  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) | **[inf](/ldsctrlest/docs/api/namespaces/namespacelds/#variable-inf)** <br>Some useful numbers.  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) | **[neginf](/ldsctrlest/docs/api/namespaces/namespacelds/#variable-neginf)**  |
| const [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) | **[pi](/ldsctrlest/docs/api/namespaces/namespacelds/#variable-pi)**  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) | **[DEFAULT_P0](/ldsctrlest/docs/api/modules/group__defaults/#variable-default_p0)**  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) | **[DEFAULT_Q0](/ldsctrlest/docs/api/modules/group__defaults/#variable-default_q0)**  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) | **[DEFAULT_R0](/ldsctrlest/docs/api/modules/group__defaults/#variable-default_r0)**  |
| std::vector< [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) > | **[DEFAULT_T0](/ldsctrlest/docs/api/modules/group__defaults/#variable-default_t0)**  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) | **[DATA_T_ZERO](/ldsctrlest/docs/api/modules/group__defaults/#variable-data_t_zero)**  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) | **[DATA_T_ONE](/ldsctrlest/docs/api/modules/group__defaults/#variable-data_t_one)**  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t) | **[DEFAULT_SOFTSTART](/ldsctrlest/docs/api/modules/group__defaults/#variable-default_softstart)**  |
| bool | **[FALSE](/ldsctrlest/docs/api/modules/group__defaults/#variable-false)**  |
| bool | **[TRUE](/ldsctrlest/docs/api/modules/group__defaults/#variable-true)**  |

## Type Details

### ssidWt

| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
| NONE | |   |
| MOESP | |   |
| CVA | |   |





Weighting options for singular value decomposition performed during subspace identification (SSID): None, MOESP "robust method", CVA "Canonical Variate Analysis") 


### data_t

```cpp
typedef double lds::data_t;
```



Precision/type of all data in library. If need 32b, change `double` to `float`. 


### stdVec

```cpp
typedef std::vector<data_t> lds::stdVec;
```



### armaVec

```cpp
typedef arma::Col<data_t> lds::armaVec;
```



### armaMat

```cpp
typedef arma::Mat<data_t> lds::armaMat;
```



### armaCube

```cpp
typedef arma::Cube<data_t> lds::armaCube;
```



### armaSubVec

```cpp
typedef arma::subview_col<data_t> lds::armaSubVec;
```



### armaSubMat

```cpp
typedef arma::subview<data_t> lds::armaSubMat;
```





## Attribute Details

### CONTROL_TYPE_U

```cpp
static const std::size_t CONTROL_TYPE_U = 0x1;
```



Control was designed to penalize change in input (i.e., the state was augmented with input `u`) 


### CONTROL_TYPE_INTY

```cpp
static const std::size_t CONTROL_TYPE_INTY = CONTROL_TYPE_U << 1;
```



Control using integral action (i.e., the state was augmented with output `y` during design) 


### CONTROL_TYPE_ADAPT_M

```cpp
static const std::size_t CONTROL_TYPE_ADAPT_M = CONTROL_TYPE_U << 2;
```



### inf

```cpp
static data_t inf = std::numeric_limits<[data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t)>::infinity();
```



### neginf

```cpp
static data_t neginf = -inf;
```



### pi

```cpp
static const data_t pi = arma::datum::pi;
```



### DEFAULT_P0

```cpp
static data_t DEFAULT_P0 = 1e-6;
```



### DEFAULT_Q0

```cpp
static data_t DEFAULT_Q0 = 1e-6;
```



### DEFAULT_R0

```cpp
static data_t DEFAULT_R0 = 1e-2;
```



### DEFAULT_T0

```cpp
static std::vector< data_t > DEFAULT_T0 = std::vector<[data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t)>(1, 0.0);
```



### DATA_T_ZERO

```cpp
static data_t DATA_T_ZERO = ([data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t))0;
```



### DATA_T_ONE

```cpp
static data_t DATA_T_ONE = ([data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t))1;
```



### DEFAULT_SOFTSTART

```cpp
static data_t DEFAULT_SOFTSTART = ([data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#typedef-data_t))0;
```



### FALSE

```cpp
static bool FALSE = false;
```



### TRUE

```cpp
static bool TRUE = true;
```







-------------------------------

Updated on  3 March 2021 at 23:06:11 CST
