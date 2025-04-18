---
title: lds::UniformVectorList

---

# lds::UniformVectorList



Inherits from std::vector< Vector >

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/#function-uniformvectorlist)**() =default<br>Constructs a new [UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/).  |
| | **[UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/#function-uniformvectorlist)**(const std::vector< Vector > & vecs, size_t dim =0)<br>Constructs a new [UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/) by copying existing vector of Vector if dimensions consistent.  |
| | **[UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/#function-uniformvectorlist)**(std::vector< Vector > && vecs, size_t dim =0)<br>Constructs a new [UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/) by moving existing vector of Vector if dimensions consistent.  |
| | **[UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/#function-uniformvectorlist)**(std::initializer_list< Vector > vecs, size_t dim =0)<br>Constructs a new [UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/) from initializer_list of Vector if dimensions consistent.  |
| | **[UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/#function-uniformvectorlist)**(const [UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/) & that)<br>Constructs a new [UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/) (copy)  |
| | **[UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/#function-uniformvectorlist)**([UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/) && that)<br>Constructs a new [UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/) (move)  |
| | **[~UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/#function-~uniformvectorlist)**() =default<br>Destroys the object.  |
| size_t | **[dim](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/#function-dim)**() const<br>gets dimensions of the uniformly sized matrices  |
| size_t | **[size](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/#function-size)**()<br>size of container  |
| const Vector & | **[at](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/#function-at)**(size_t n)<br>gets reference to n^th element  |
| void | **[Swap](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/#function-swap)**(Vector & that, size_t n)<br>swaps input matrix with n^th vector of list  |
| [UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/) & | **[operator=](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/#function-operator=)**(const [UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/) & that)<br>assigns the contents (copy)  |
| [UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/) & | **[operator=](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/#function-operator=)**([UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/) && that)<br>assigns the contents (move)  |

---
---
## Public Function Details

### **UniformVectorList**

```cpp
UniformVectorList() =default
```



---
### **UniformVectorList**

```cpp
explicit UniformVectorList(
    const std::vector< Vector > & vecs,
    size_t dim =0
)
```



**Parameters**:

  * **vecs** input vectors 
  * **dims** dimension 


---
### **UniformVectorList**

```cpp
explicit UniformVectorList(
    std::vector< Vector > && vecs,
    size_t dim =0
)
```



**Parameters**:

  * **vecs** input vectors 
  * **dim** dimension 


---
### **UniformVectorList**

```cpp
UniformVectorList(
    std::initializer_list< Vector > vecs,
    size_t dim =0
)
```



**Parameters**:

  * **vecs** input vectors 
  * **dim** dimension 


---
### **UniformVectorList**

```cpp
UniformVectorList(
    const UniformVectorList & that
)
```



**Parameters**:

  * **that** another [UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/)


---
### **UniformVectorList**

```cpp
UniformVectorList(
    UniformVectorList && that
)
```



**Parameters**:

  * **that** another [UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/)


---
### **~UniformVectorList**

```cpp
~UniformVectorList() =default
```



---
### **dim**

```cpp
inline size_t dim() const
```



---
### **size**

```cpp
inline size_t size()
```



---
### **at**

```cpp
inline const Vector & at(
    size_t n
)
```



---
### **Swap**

```cpp
inline void Swap(
    Vector & that,
    size_t n
)
```



**Parameters**:

  * **that** input vector 
  * **n** index where the vector is moved 


---
### **operator=**

```cpp
inline UniformVectorList & operator=(
    const UniformVectorList & that
)
```



**Parameters**:

  * **that** another [UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/)


**Return**: reference to object 

---
### **operator=**

```cpp
inline UniformVectorList & operator=(
    UniformVectorList && that
)
```



**Parameters**:

  * **that** another [UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_vector_list/)


**Return**: reference to object 

---


-------------------------------

Updated on  3 April 2025 at 13:48:29 EDT
