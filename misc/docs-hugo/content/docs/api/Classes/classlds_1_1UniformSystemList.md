---
title: lds::UniformSystemList

---

# lds::UniformSystemList



 [More...](#detailed-description)

Inherits from std::vector< System >

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/#function-uniformsystemlist)**() =default<br>Constructs a new [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/).  |
| | **[UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/#function-uniformsystemlist)**(const std::vector< [System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/) > & systems, std::array< size_t, 3 > dim ={0, 0, 0})<br>Constructs a new [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/) by copying existing vector of [System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/) if dimensions consistent.  |
| | **[UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/#function-uniformsystemlist)**(std::vector< [System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/) > && systems, std::array< size_t, 3 > dim ={0, 0, 0})<br>Constructs a new [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/) by moving existing vector of [System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/) if dimensions consistent.  |
| | **[UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/#function-uniformsystemlist)**(std::initializer_list< [System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/) > systems, std::array< size_t, 3 > dim ={0, 0, 0})<br>Constructs a new [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/) from initializer_list of [System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/) if dimensions consistent.  |
| | **[UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/#function-uniformsystemlist)**(const [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/) & that)<br>Constructs a new [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/) (copy).  |
| | **[UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/#function-uniformsystemlist)**([UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/) && that)<br>Constructs a new [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/) (move).  |
| | **[~UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/#function-~uniformsystemlist)**() =default<br>Destroys the object.  |
| const std::array< size_t, 3 > & | **[dim](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/#function-dim)**() const<br>gets dimensions of the uniformly sized systems  |
| size_t | **[size](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/#function-size)**()<br>size of container  |
| const [System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/) & | **[at](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/#function-at)**(size_t n)<br>gets reference to n^th element  |
| void | **[Swap](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/#function-swap)**([System](/lds-ctrl-est/docs/api/classes/classlds_1_1system/) & that, size_t n)<br>swaps input system with n^th system of list  |
| [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/) & | **[operator=](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/#function-operator=)**(const [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/) & that)<br>assigns the contents (copy)  |
| [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/) & | **[operator=](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/#function-operator=)**([UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/) && that)<br>assigns the contents (move)  |

## Detailed Description

```cpp
template <typename System>
class lds::UniformSystemList;
```


---
---
## Public Function Details

### **UniformSystemList**

```cpp
UniformSystemList() =default
```



---
### **UniformSystemList**

```cpp
explicit UniformSystemList(
    const std::vector< System > & systems,
    std::array< size_t, 3 > dim ={0, 0, 0}
)
```



**Parameters**:

  * **systems** input systems 
  * **dim** dimensions (n_u, n_x, n_y) 


---
### **UniformSystemList**

```cpp
explicit UniformSystemList(
    std::vector< System > && systems,
    std::array< size_t, 3 > dim ={0, 0, 0}
)
```



**Parameters**:

  * **systems** input systems 
  * **dim** dimensions (n_u, n_x, n_y) 


---
### **UniformSystemList**

```cpp
UniformSystemList(
    std::initializer_list< System > systems,
    std::array< size_t, 3 > dim ={0, 0, 0}
)
```



**Parameters**:

  * **systems** input systems 
  * **dim** dimensions (n_u, n_x, n_y) 


---
### **UniformSystemList**

```cpp
UniformSystemList(
    const UniformSystemList & that
)
```



**Parameters**:

  * **that** another [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/)


---
### **UniformSystemList**

```cpp
UniformSystemList(
    UniformSystemList && that
)
```



**Parameters**:

  * **that** another [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/)


---
### **~UniformSystemList**

```cpp
~UniformSystemList() =default
```



---
### **dim**

```cpp
inline const std::array< size_t, 3 > & dim() const
```



---
### **size**

```cpp
inline size_t size()
```



---
### **at**

```cpp
inline const System & at(
    size_t n
)
```



---
### **Swap**

```cpp
inline void Swap(
    System & that,
    size_t n
)
```



**Parameters**:

  * **that** input system 
  * **n** index where the system is moved 


---
### **operator=**

```cpp
inline UniformSystemList & operator=(
    const UniformSystemList & that
)
```



**Parameters**:

  * **that** another [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/)


**Return**: reference to object 

---
### **operator=**

```cpp
inline UniformSystemList & operator=(
    UniformSystemList && that
)
```



**Parameters**:

  * **that** another [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformsystemlist/)


**Return**: reference to object 

---


-------------------------------

Updated on  4 May 2022 at 16:59:50 Eastern Daylight Time
