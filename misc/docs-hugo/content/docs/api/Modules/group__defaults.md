---
title: Defaults

---

# Defaults

<br> <br>[More...](#detailed-description)
<br>


## Attributes

|                | Name           |
| -------------- | -------------- |
| data_t | **[DEFAULT_P0](/ldsctrlest/docs/api/modules/group__defaults/#variable-default_p0)**  |
| data_t | **[DEFAULT_Q0](/ldsctrlest/docs/api/modules/group__defaults/#variable-default_q0)**  |
| data_t | **[DEFAULT_R0](/ldsctrlest/docs/api/modules/group__defaults/#variable-default_r0)**  |
| std::vector< data_t > | **[DEFAULT_T0](/ldsctrlest/docs/api/modules/group__defaults/#variable-default_t0)**  |
| data_t | **[DATA_T_ZERO](/ldsctrlest/docs/api/modules/group__defaults/#variable-data_t_zero)**  |
| data_t | **[DATA_T_ONE](/ldsctrlest/docs/api/modules/group__defaults/#variable-data_t_one)**  |
| data_t | **[DEFAULT_SOFTSTART](/ldsctrlest/docs/api/modules/group__defaults/#variable-default_softstart)**  |
| bool | **[FALSE](/ldsctrlest/docs/api/modules/group__defaults/#variable-false)**  |
| bool | **[TRUE](/ldsctrlest/docs/api/modules/group__defaults/#variable-true)**  |

## Detailed Description



Default values for common variables (e.g., default diagonal elements of covariances in case there is need to resolve dimension mismatch) 



## Attribute Details

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
static std::vector< data_t > DEFAULT_T0 = std::vector<data_t>(1, 0.0);
```



### DATA_T_ZERO

```cpp
static data_t DATA_T_ZERO = (data_t)0;
```



### DATA_T_ONE

```cpp
static data_t DATA_T_ONE = (data_t)1;
```



### DEFAULT_SOFTSTART

```cpp
static data_t DEFAULT_SOFTSTART = (data_t)0;
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

Updated on  3 March 2021 at 23:06:12 CST
