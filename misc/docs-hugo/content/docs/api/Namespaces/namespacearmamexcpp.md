---
title: armamexcpp
summary: arma/mex interface using Matlab C++ API 

---

# armamexcpp

arma/mex interface using Matlab C++ API <br> <br>[More...](#detailed-description)
<br>


## Functions

|                | Name           |
| -------------- | -------------- |
| template <class T \> <br>std::vector< arma::Mat< T > > | **[m2a_cellmat](/lds-ctrl-est/docs/api/namespaces/namespacearmamexcpp/#function-m2a-cellmat)**(matlab::data::CellArray & matlab_cell)<br>Convert matlab cell array to vector of armadillo matrices.  |
| template <class T \> <br>std::vector< T > | **[m2s_vec](/lds-ctrl-est/docs/api/namespaces/namespacearmamexcpp/#function-m2s-vec)**(matlab::data::TypedArray< T > & matlab_array)<br>Convert matlab matrix to a vector of scalars.  |
| template <class T \> <br>arma::Col< T > | **[m2a_vec](/lds-ctrl-est/docs/api/namespaces/namespacearmamexcpp/#function-m2a-vec)**(matlab::data::TypedArray< T > matlab_array)<br>Convert matlab to armadillo vector.  |
| template <class T \> <br>arma::Mat< T > | **[m2a_mat](/lds-ctrl-est/docs/api/namespaces/namespacearmamexcpp/#function-m2a-mat)**(matlab::data::TypedArray< T > matlab_array)<br>Convert matlab to armadillo matrix.  |
| template <class T \> <br>matlab::data::TypedArray< T > | **[a2m_mat](/lds-ctrl-est/docs/api/namespaces/namespacearmamexcpp/#function-a2m-mat)**(const arma::Mat< T > & arma_mat, matlab::data::ArrayFactory & factory)<br>Convert armadillo to matlab matrix.  |
| template <class T \> <br>matlab::data::TypedArray< T > | **[a2m_vec](/lds-ctrl-est/docs/api/namespaces/namespacearmamexcpp/#function-a2m-vec)**(const arma::Col< T > & arma_vec, matlab::data::ArrayFactory & factory)<br>Convert armadillo to matlab vector.  |
| template <class T \> <br>matlab::data::TypedArray< T > | **[s2m_vec](/lds-ctrl-est/docs/api/namespaces/namespacearmamexcpp/#function-s2m-vec)**(const std::vector< T > & std_vec, matlab::data::ArrayFactory & factory)<br>Convert vector of scalar T to matlab matrix.  |

## Detailed Description



utilities for arma/mex interface _using Matlab C++ API_


## Function Details

### m2a_cellmat

```cpp
template <class T >
std::vector< arma::Mat< T > > m2a_cellmat(
    matlab::data::CellArray & matlab_cell
)
```



**Parameters**:

  * **matlab_cell** matlab cell


**Template Parameters**:

  * **T** type


**Return**: vector of armadillo matrices of type T 

### m2s_vec

```cpp
template <class T >
std::vector< T > m2s_vec(
    matlab::data::TypedArray< T > & matlab_array
)
```



**Parameters**:

  * **matlab_array** matlab array


**Template Parameters**:

  * **T** type


**Return**: vector of type T 

### m2a_vec

```cpp
template <class T >
arma::Col< T > m2a_vec(
    matlab::data::TypedArray< T > matlab_array
)
```



**Parameters**:

  * **matlab_array** matlab array


**Template Parameters**:

  * **T** type


**Return**: armadillo vector of type T 

### m2a_mat

```cpp
template <class T >
arma::Mat< T > m2a_mat(
    matlab::data::TypedArray< T > matlab_array
)
```



**Parameters**:

  * **matlab_array** matlab matrix


**Template Parameters**:

  * **T** type


**Return**: armadillo matrix of type T 

### a2m_mat

```cpp
template <class T >
matlab::data::TypedArray< T > a2m_mat(
    const arma::Mat< T > & arma_mat,
    matlab::data::ArrayFactory & factory
)
```



**Parameters**:

  * **arma_mat** arma matrix 
  * **factory** matlab "array factory"


**Template Parameters**:

  * **T** type


**Return**: matlab matrix 

### a2m_vec

```cpp
template <class T >
matlab::data::TypedArray< T > a2m_vec(
    const arma::Col< T > & arma_vec,
    matlab::data::ArrayFactory & factory
)
```



**Parameters**:

  * **arma_vec** armadillo vector 
  * **factory** matlab "array factory"


**Template Parameters**:

  * **T** type


**Return**: matlab matrix 

### s2m_vec

```cpp
template <class T >
matlab::data::TypedArray< T > s2m_vec(
    const std::vector< T > & std_vec,
    matlab::data::ArrayFactory & factory
)
```



**Parameters**:

  * **std_vec** standard vector 
  * **factory** matlab "array factory"


**Template Parameters**:

  * **T** type


**Return**: matlab matrix 






-------------------------------

Updated on 31 March 2025 at 16:31:00 EDT
