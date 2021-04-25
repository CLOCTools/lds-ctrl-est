---
title: lds
summary: Linear Dynamical Systems (LDS) namespace. 

---

# lds

Linear Dynamical Systems (LDS) namespace. <br>

## Namespaces

| Name           |
| -------------- |
| **[lds::gaussian](/lds-ctrl-est/docs/api/namespaces/namespacelds_1_1gaussian/)** <br>Linear Dynamical Systems with Gaussian observations.  |
| **[lds::poisson](/lds-ctrl-est/docs/api/namespaces/namespacelds_1_1poisson/)** <br>Linear Dynamical Systems with Poisson observations.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1_controller/)**  |
| class | **[lds::Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/)** <br>LDS [Fit]() Type.  |
| class | **[lds::EM](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/)**  |
| class | **[lds::SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/)**  |
| class | **[lds::SwitchedController](/lds-ctrl-est/docs/api/classes/classlds_1_1_switched_controller/)** <br>[SwitchedController]() Type.  |
| class | **[lds::System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/)** <br>Linear Dynamical [System]() Type.  |
| class | **[lds::UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)**  |
| class | **[lds::UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/)**  |
| class | **[lds::UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/)**  |

## Types

|                | Name           |
| -------------- | -------------- |
| enum| **[SSIDWt](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enum-ssidwt)** { kSSIDNone, kSSIDMOESP, kSSIDCVA}<br>weighting options for [SSID]() |
| enum| **[MatrixListFreeDim](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enum-matrixlistfreedim)** { kMatFreeDimNone, kMatFreeDim1, kMatFreeDim2} |
| using double | **[data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t)**  |
| using arma::Col< [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) > | **[Vector](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-vector)**  |
| using arma::Mat< [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) > | **[Matrix](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-matrix)**  |
| using arma::Cube< [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) > | **[Cube](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-cube)**  |
| using arma::subview< [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) > | **[View](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-view)**  |

## Functions

|                | Name           |
| -------------- | -------------- |
| void | **[Limit](/lds-ctrl-est/docs/api/namespaces/namespacelds/#function-limit)**(std::vector< [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) > & x, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) lb, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) ub) |
| void | **[Limit](/lds-ctrl-est/docs/api/namespaces/namespacelds/#function-limit)**(Vector & x, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) lb, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) ub) |
| void | **[Limit](/lds-ctrl-est/docs/api/namespaces/namespacelds/#function-limit)**(Matrix & x, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) lb, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) ub) |
| void | **[Reassign](/lds-ctrl-est/docs/api/namespaces/namespacelds/#function-reassign)**(Vector & some, const Vector & other, const std::string & parenthetical ="Reassign")<br>reassigns contents of some Vector in place  |
| void | **[Reassign](/lds-ctrl-est/docs/api/namespaces/namespacelds/#function-reassign)**(Matrix & some, const Matrix & other, const std::string & parenthetical ="Reassign")<br>reassigns contents of some Matrix in place  |
| void | **[ForceSymPD](/lds-ctrl-est/docs/api/namespaces/namespacelds/#function-forcesympd)**(Matrix & X)<br>forces matrix to be symmetric positive-definite  |
| void | **[ForceSymMinEig](/lds-ctrl-est/docs/api/namespaces/namespacelds/#function-forcesymmineig)**(Matrix & X, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) eig_min =0)<br>forces matrix to be symmetric and have a minimum eigenvalue  |
| void | **[lq](/lds-ctrl-est/docs/api/namespaces/namespacelds/#function-lq)**(Matrix & L, Matrix & Qt, const Matrix & X)<br>LQ decomposition.  |
| Matrix | **[calcCov](/lds-ctrl-est/docs/api/namespaces/namespacelds/#function-calccov)**(const Matrix & A, const Matrix & B)<br>Calculate covariance matrix.  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| const std::size_t | **[kControlTypeDeltaU](/lds-ctrl-est/docs/api/modules/group__control__masks/#variable-kcontroltypedeltau)** <br>control designed to penalize change in input  |
| const std::size_t | **[kControlTypeIntY](/lds-ctrl-est/docs/api/modules/group__control__masks/#variable-kcontroltypeinty)** <br>control using integral action  |
| const std::size_t | **[kControlTypeAdaptM](/lds-ctrl-est/docs/api/modules/group__control__masks/#variable-kcontroltypeadaptm)** <br>adapt control setpoint with re-estimated disturbance `m` |
| const [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) | **[kInf](/lds-ctrl-est/docs/api/namespaces/namespacelds/#variable-kinf)** <br>Some useful numbers.  |
| const [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) | **[kPi](/lds-ctrl-est/docs/api/namespaces/namespacelds/#variable-kpi)**  |
| const [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) | **[kDefaultP0](/lds-ctrl-est/docs/api/modules/group__defaults/#variable-kdefaultp0)** <br>default state estimate covar  |
| const [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) | **[kDefaultQ0](/lds-ctrl-est/docs/api/modules/group__defaults/#variable-kdefaultq0)** <br>default process noise covar  |
| const [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) | **[kDefaultR0](/lds-ctrl-est/docs/api/modules/group__defaults/#variable-kdefaultr0)** <br>default output noise covar  |

## Type Details

### SSIDWt

| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
| kSSIDNone | | None.   |
| kSSIDMOESP | | MOESP (AKA "robust method" in van Overschee 1996)   |
| kSSIDCVA | | CVA "Canonical Variate Analysis".   |





Weighting options for singular value decomposition performed during subspace identification ([SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/))

Reference:

van Overschee, de Moor. 1996. Subspace Identification for Linear Systems. 


### MatrixListFreeDim

| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
| kMatFreeDimNone | | neither dim free to be hetero in mat list   |
| kMatFreeDim1 | | allow 1st dim of mats in list to be hetero   |
| kMatFreeDim2 | | allow 2nd dim of mats in list to be hetero   |





### data_t

```cpp
using lds::data_t = typedef double;
```



Type of all data in library. If need 32b, change `double` to `float`. This could be potentially useful for large scale problems where there are memory constraints. 


### Vector

```cpp
using lds::Vector = typedef arma::Col<data_t>;
```



### Matrix

```cpp
using lds::Matrix = typedef arma::Mat<data_t>;
```



### Cube

```cpp
using lds::Cube = typedef arma::Cube<data_t>;
```



### View

```cpp
using lds::View = typedef arma::subview<data_t>;
```




## Function Details

### Limit

```cpp
inline void Limit(
    std::vector< data_t > & x,
    data_t lb,
    data_t ub
)
```



### Limit

```cpp
inline void Limit(
    Vector & x,
    data_t lb,
    data_t ub
)
```



### Limit

```cpp
inline void Limit(
    Matrix & x,
    data_t lb,
    data_t ub
)
```



### Reassign

```cpp
inline void Reassign(
    Vector & some,
    const Vector & other,
    const std::string & parenthetical ="Reassign"
)
```



**Parameters**:

  * **some** some Vector 
  * **other** other Vector 
  * **parenthetical** optional description provided by caller to ease debugging 


### Reassign

```cpp
inline void Reassign(
    Matrix & some,
    const Matrix & other,
    const std::string & parenthetical ="Reassign"
)
```



**Parameters**:

  * **some** some Matrix 
  * **other** other Matrix 
  * **parenthetical** optional description provided by caller to ease debugging 


### ForceSymPD

```cpp
void ForceSymPD(
    Matrix & X
)
```



**Parameters**:

  * **X** mutated matrix 


### ForceSymMinEig

```cpp
void ForceSymMinEig(
    Matrix & X,
    data_t eig_min =0
)
```



**Parameters**:

  * **X** mutated matrix 
  * **eig_min** [optional] minimum eigen value 


### lq

```cpp
void lq(
    Matrix & L,
    Matrix & Qt,
    const Matrix & X
)
```



**Parameters**:

  * **L** lower triangle matrix 
  * **Qt** orthonormal matrix (transposed cf QR decomp) 
  * **X** matrix being decomposed 


### calcCov

```cpp
Matrix calcCov(
    const Matrix & A,
    const Matrix & B
)
```



**Parameters**:

  * **A** some matrix 
  * **B** some other matrix


**Return**: covariance 


## Attribute Details

### kControlTypeDeltaU

```cpp
static const std::size_t kControlTypeDeltaU = 0x1;
```



Control was designed to penalize change in input (i.e., the state was augmented with input `u`) 


### kControlTypeIntY

```cpp
static const std::size_t kControlTypeIntY = kControlTypeDeltaU << 1;
```



Control using integral action (i.e., the state was augmented with output `y` during design) 


### kControlTypeAdaptM

```cpp
static const std::size_t kControlTypeAdaptM = kControlTypeDeltaU << 2;
```



Adapt control setpoint adapted with re-estimated process disturbance `m`. 


### kInf

```cpp
static const data_t kInf = std::numeric_limits<[data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t)>::infinity();
```



### kPi

```cpp
static const data_t kPi = arma::datum::pi;
```



### kDefaultP0

```cpp
static const data_t kDefaultP0 = 1e-6;
```



### kDefaultQ0

```cpp
static const data_t kDefaultQ0 = 1e-6;
```



### kDefaultR0

```cpp
static const data_t kDefaultR0 = 1e-2;
```







-------------------------------

Updated on 25 April 2021 at 11:04:30 EDT
