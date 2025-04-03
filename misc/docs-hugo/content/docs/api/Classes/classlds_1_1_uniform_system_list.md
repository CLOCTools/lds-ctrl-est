---
title: lds::UniformSystemList

---

# lds::UniformSystemList



 [More...](#detailed-description)

Inherits from std::vector< System >

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/#function-uniformsystemlist)**() =default<br>Constructs a new [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/).  |
| | **[UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/#function-uniformsystemlist)**(const std::vector< [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) > & systems, std::array< size_t, 3 > dim ={0, 0, 0})<br>Constructs a new [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/) by copying existing vector of [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) if dimensions consistent.  |
| | **[UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/#function-uniformsystemlist)**(std::vector< [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) > && systems, std::array< size_t, 3 > dim ={0, 0, 0})<br>Constructs a new [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/) by moving existing vector of [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) if dimensions consistent.  |
| | **[UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/#function-uniformsystemlist)**(std::initializer_list< [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) > systems, std::array< size_t, 3 > dim ={0, 0, 0})<br>Constructs a new [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/) from initializer_list of [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) if dimensions consistent.  |
| | **[UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/#function-uniformsystemlist)**(const [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/) & that)<br>Constructs a new [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/) (copy).  |
| | **[UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/#function-uniformsystemlist)**([UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/) && that)<br>Constructs a new [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/) (move).  |
| | **[~UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/#function-~uniformsystemlist)**() =default<br>Destroys the object.  |
| const std::array< size_t, 3 > & | **[dim](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/#function-dim)**() const<br>gets dimensions of the uniformly sized systems  |
| size_t | **[size](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/#function-size)**()<br>size of container  |
| const [System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) & | **[at](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/#function-at)**(size_t n)<br>gets reference to n^th element  |
| void | **[Swap](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/#function-swap)**([System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/) & that, size_t n)<br>swaps input system with n^th system of list  |
| [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/) & | **[operator=](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/#function-operator=)**(const [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/) & that)<br>assigns the contents (copy)  |
| [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/) & | **[operator=](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/#function-operator=)**([UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/) && that)<br>assigns the contents (move)  |

## Detailed Description

```cpp
template <typename System >
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

  * **that** another [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/)


---
### **UniformSystemList**

```cpp
UniformSystemList(
    UniformSystemList && that
)
```



**Parameters**:

  * **that** another [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/)


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

  * **that** another [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/)


**Return**: reference to object 

---
### **operator=**

```cpp
inline UniformSystemList & operator=(
    UniformSystemList && that
)
```



**Parameters**:

  * **that** another [UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/)


**Return**: reference to object 

---


-------------------------------

Updated on  3 April 2025 at 13:48:29 EDT
