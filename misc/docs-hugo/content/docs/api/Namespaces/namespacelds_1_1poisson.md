---
title: lds::poisson
summary: Linear Dynamical Systems with Poisson observations. 

---

# lds::poisson

Linear Dynamical Systems with Poisson observations. <br>

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::poisson::Controller](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_controller/)** <br>PLDS [Controller]() Type.  |
| class | **[lds::poisson::Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_fit/)** <br>PLDS [Fit]() Type.  |
| class | **[lds::poisson::FitEM](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_fit_e_m/)** <br>PLDS E-M [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_fit/) Type.  |
| class | **[lds::poisson::FitSSID](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_fit_s_s_i_d/)** <br>Subspace Identification ([SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/)) for PLDS.  |
| class | **[lds::poisson::SwitchedController](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_switched_controller/)** <br>Poisson-observation [SwitchedController]() Type.  |
| class | **[lds::poisson::System](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_system/)** <br>Poisson [System]() type.  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| std::random_device | **[rd](/lds-ctrl-est/docs/api/namespaces/namespacelds_1_1poisson/#variable-rd)** <br>random device for simulating poisson data  |
| std::mt19937 | **[rng](/lds-ctrl-est/docs/api/namespaces/namespacelds_1_1poisson/#variable-rng)** <br>random number generator for simulating poisson data  |



## Attribute Details

### rd

```cpp
static std::random_device rd;
```



### rng

```cpp
static std::mt19937 rng = std::mt19937(
    rd());
```







-------------------------------

Updated on 31 March 2025 at 16:04:30 EDT
