---
title: Control Mode Bit Masks
summary: provides fill types for constructing new armadillo vectors, matrices 

---

# Control Mode Bit Masks

provides fill types for constructing new armadillo vectors, matrices <br> <br>[More...](#detailed-description)
<br>


## Attributes

|                | Name           |
| -------------- | -------------- |
| const std::size_t | **[kControlTypeDeltaU](/ldsctrlest/docs/api/modules/group__control__masks/#variable-kcontroltypedeltau)** <br>control designed to penalize change in input  |
| const std::size_t | **[kControlTypeIntY](/ldsctrlest/docs/api/modules/group__control__masks/#variable-kcontroltypeinty)** <br>control using integral action  |
| const std::size_t | **[kControlTypeAdaptM](/ldsctrlest/docs/api/modules/group__control__masks/#variable-kcontroltypeadaptm)** <br>adapt control setpoint with re-estimated disturbance `m` |

## Detailed Description



Control mode bit masks. These can be bit-wise OR'd to use in combination. 



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






-------------------------------

Updated on 23 March 2021 at 09:14:15 CDT
