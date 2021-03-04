---
title: Control Mode Bit Masks

---

# Control Mode Bit Masks

<br> <br>[More...](#detailed-description)
<br>


## Attributes

|                | Name           |
| -------------- | -------------- |
| const std::size_t | **[CONTROL_TYPE_U](/ldsctrlest/docs/api/modules/group__control__masks/#variable-control_type_u)**  |
| const std::size_t | **[CONTROL_TYPE_INTY](/ldsctrlest/docs/api/modules/group__control__masks/#variable-control_type_inty)**  |
| const std::size_t | **[CONTROL_TYPE_ADAPT_M](/ldsctrlest/docs/api/modules/group__control__masks/#variable-control_type_adapt_m)** <br>Control setpoint adapted with (re-estimated) process disturbance `m`.  |

## Detailed Description



Control mode bit masks. These can be bit-wise OR'd to use in combination. 



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







-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
