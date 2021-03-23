---
title: lds::UniformMatrixList

---

# lds::UniformMatrixList



 [More...](#detailed-description)

Inherits from std::vector< Matrix >

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/#function-uniformmatrixlist)**() =default<br>Constructs a new [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/).  |
| | **[UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/#function-uniformmatrixlist)**(const std::vector< Matrix > & mats, std::array< size_t, 2 > dim ={0, 0})<br>Constructs a new [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/) by copying existing vector of Matrix if dimensions consistent.  |
| | **[UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/#function-uniformmatrixlist)**(std::vector< Matrix > && mats, std::array< size_t, 2 > dim ={0, 0})<br>Constructs a new [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/) by moving existing vector of Matrix if dimensions consistent.  |
| | **[UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/#function-uniformmatrixlist)**(std::initializer_list< Matrix > mats, std::array< size_t, 2 > dim ={0, 0})<br>Constructs a new [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/) from initializer_list of Matrix if dimensions consistent.  |
| | **[UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/#function-uniformmatrixlist)**(const [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)< D > & that)<br>Constructs a new [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/) (copy).  |
| | **[UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/#function-uniformmatrixlist)**([UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)< D > && that)<br>Constructs a new [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/) (move).  |
| | **[~UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/#function-~uniformmatrixlist)**() =default<br>Destroys the object.  |
| const std::array< size_t, 2 > & | **[dim](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/#function-dim)**(size_t n =0) const<br>gets dimensions of uniformly sized matrices  |
| size_t | **[size](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/#function-size)**()<br>size of container  |
| const Matrix & | **[at](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/#function-at)**(size_t n)<br>gets reference to n^th element  |
| void | **[Swap](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/#function-swap)**(Matrix & that, size_t n)<br>swaps input matrix with n^th matrix of list  |
| [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)< D > & | **[operator=](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/#function-operator=)**(const [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)< D > & that)<br>assigns the contents (copy)  |
| [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)< D > & | **[operator=](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/#function-operator=)**([UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)< D > && that)<br>assigns the contents (move)  |

## Detailed Description

```cpp
template <MatrixListFreeDim D =kMatFreeDimNone>
class lds::UniformMatrixList;
```


---
---
## Public Function Details

### **UniformMatrixList**

```cpp
UniformMatrixList() =default
```



---
### **UniformMatrixList**

```cpp
explicit UniformMatrixList(
    const std::vector< Matrix > & mats,
    std::array< size_t, 2 > dim ={0, 0}
)
```



**Parameters**:

  * **mats** input matrices 
  * **dim** dimensions 


---
### **UniformMatrixList**

```cpp
explicit UniformMatrixList(
    std::vector< Matrix > && mats,
    std::array< size_t, 2 > dim ={0, 0}
)
```



**Parameters**:

  * **mats** input matrices 
  * **dim** dimensions 


---
### **UniformMatrixList**

```cpp
UniformMatrixList(
    std::initializer_list< Matrix > mats,
    std::array< size_t, 2 > dim ={0, 0}
)
```



**Parameters**:

  * **mats** input matrices 
  * **dim** dimensions 


---
### **UniformMatrixList**

```cpp
UniformMatrixList(
    const UniformMatrixList< D > & that
)
```



**Parameters**:

  * **that** another [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)


---
### **UniformMatrixList**

```cpp
UniformMatrixList(
    UniformMatrixList< D > && that
)
```



**Parameters**:

  * **that** another [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)


---
### **~UniformMatrixList**

```cpp
~UniformMatrixList() =default
```



---
### **dim**

```cpp
inline const std::array< size_t, 2 > & dim(
    size_t n =0
) const
```



**Parameters**:

  * **n** [optional] index in list of matrices


**Return**: dimensions 

---
### **size**

```cpp
inline size_t size()
```



---
### **at**

```cpp
inline const Matrix & at(
    size_t n
)
```



---
### **Swap**

```cpp
inline void Swap(
    Matrix & that,
    size_t n
)
```



**Parameters**:

  * **that** input matrix 
  * **n** index where the matrix is moved 


---
### **operator=**

```cpp
inline UniformMatrixList< D > & operator=(
    const UniformMatrixList< D > & that
)
```



**Parameters**:

  * **that** another [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)


**Return**: reference to object 

---
### **operator=**

```cpp
inline UniformMatrixList< D > & operator=(
    UniformMatrixList< D > && that
)
```



**Parameters**:

  * **that** another [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)


**Return**: reference to object 

---


-------------------------------

Updated on 23 March 2021 at 09:14:14 CDT
