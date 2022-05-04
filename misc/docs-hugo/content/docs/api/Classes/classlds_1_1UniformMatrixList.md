---
title: lds::UniformMatrixList

---

# lds::UniformMatrixList



 [More...](#detailed-description)

Inherits from std::vector< Matrix >

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/#function-uniformmatrixlist)**() =default<br>Constructs a new [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/).  |
| | **[UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/#function-uniformmatrixlist)**(const std::vector< Matrix > & mats, std::array< size_t, 2 > dim ={0, 0})<br>Constructs a new [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/) by copying existing vector of Matrix if dimensions consistent.  |
| | **[UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/#function-uniformmatrixlist)**(std::vector< Matrix > && mats, std::array< size_t, 2 > dim ={0, 0})<br>Constructs a new [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/) by moving existing vector of Matrix if dimensions consistent.  |
| | **[UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/#function-uniformmatrixlist)**(std::initializer_list< Matrix > mats, std::array< size_t, 2 > dim ={0, 0})<br>Constructs a new [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/) from initializer_list of Matrix if dimensions consistent.  |
| | **[UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/#function-uniformmatrixlist)**(const [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)< D > & that)<br>Constructs a new [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/) (copy).  |
| | **[UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/#function-uniformmatrixlist)**([UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)< D > && that)<br>Constructs a new [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/) (move).  |
| | **[~UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/#function-~uniformmatrixlist)**() =default<br>Destroys the object.  |
| const std::array< size_t, 2 > & | **[dim](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/#function-dim)**(size_t n =0) const<br>gets dimensions of uniformly sized matrices  |
| size_t | **[size](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/#function-size)**()<br>size of container  |
| const Matrix & | **[at](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/#function-at)**(size_t n)<br>gets reference to n^th element  |
| void | **[Swap](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/#function-swap)**(Matrix & that, size_t n)<br>swaps input matrix with n^th matrix of list  |
| [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)< D > & | **[operator=](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/#function-operator=)**(const [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)< D > & that)<br>assigns the contents (copy)  |
| [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)< D > & | **[operator=](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/#function-operator=)**([UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)< D > && that)<br>assigns the contents (move)  |

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

  * **that** another [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)


---
### **UniformMatrixList**

```cpp
UniformMatrixList(
    UniformMatrixList< D > && that
)
```



**Parameters**:

  * **that** another [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)


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

  * **that** another [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)


**Return**: reference to object 

---
### **operator=**

```cpp
inline UniformMatrixList< D > & operator=(
    UniformMatrixList< D > && that
)
```



**Parameters**:

  * **that** another [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)


**Return**: reference to object 

---


-------------------------------

Updated on  4 May 2022 at 16:34:51 Eastern Daylight Time
