---
title: armamexc
summary: arma/mex interface using Matlab C API 

---

# armamexc

arma/mex interface using Matlab C API <br> <br>[More...](#detailed-description)
<br>


## Functions

|                | Name           |
| -------------- | -------------- |
| template <class T \> <br>auto | **[m2T_scalar](/lds-ctrl-est/docs/api/namespaces/namespacearmamexc/#function-m2t-scalar)**(const mxArray * matlab_scalar)<br>Convert Matlab mxArray to scalar of type T.  |
| template <class T \> <br>auto | **[m2a_mat](/lds-ctrl-est/docs/api/namespaces/namespacearmamexc/#function-m2a-mat)**(const mxArray * matlab_mat, bool copy_aux_mem =false, bool strict =true)<br>Convert matlab matrix to armadillo.  |
| template <typename T \> <br>auto | **[a2m_mat](/lds-ctrl-est/docs/api/namespaces/namespacearmamexc/#function-a2m-mat)**(arma::Mat< T > const & arma_mat)<br>Convert armadillo to matlab matrix.  |
| template <typename T \> <br>auto | **[a2m_vec](/lds-ctrl-est/docs/api/namespaces/namespacearmamexc/#function-a2m-vec)**(arma::Col< T > const & arma_vec)<br>Convert armadillo to matlab vector.  |

## Detailed Description



Utilities for arma/mex interface _using Matlab C API_


## Function Details

### m2T_scalar

```cpp
template <class T >
inline auto m2T_scalar(
    const mxArray * matlab_scalar
)
```



**Parameters**:

  * **matlab_scalar** matlab scalar


**Template Parameters**:

  * **T** type


**Return**: scalar of type T 

### m2a_mat

```cpp
template <class T >
inline auto m2a_mat(
    const mxArray * matlab_mat,
    bool copy_aux_mem =false,
    bool strict =true
)
```



**Parameters**:

  * **matlab_mat** matlab matrix 
  * **copy_aux_mem** [optional] whether to copy auxiliary memory 
  * **strict** [optional] strictly enforce the above


**Template Parameters**:

  * **T** type


**Return**: armadillo matrix of type T 

### a2m_mat

```cpp
template <typename T >
inline auto a2m_mat(
    arma::Mat< T > const & arma_mat
)
```



**Parameters**:

  * **arma_mat** armadillo matrix


**Return**: matlab matrix 

### a2m_vec

```cpp
template <typename T >
inline auto a2m_vec(
    arma::Col< T > const & arma_vec
)
```



**Parameters**:

  * **arma_vec** armadillo vector


**Return**: matlab vector 






-------------------------------

Updated on  4 May 2022 at 15:48:57 Eastern Daylight Time
