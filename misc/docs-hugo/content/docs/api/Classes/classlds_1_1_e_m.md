---
title: lds::EM

---

# lds::EM



 [More...](#detailed-description)

Inherited by [lds::gaussian::FitEM](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_fit_e_m/), [lds::poisson::FitEM](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_fit_e_m/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[EM](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-em)**() =default<br>Constructs a new [EM](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/)[Fit]() type.  |
| | **[EM](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-em)**(size_t n_x, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) dt, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > && u_train, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > && z_train)<br>Constructs a new [EM](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/)[Fit]() type.  |
| | **[EM](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-em)**(const [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/) & fit0, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > && u_train, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > && z_train)<br>Constructs a new [EM](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/)[Fit]() type.  |
| virtual | **[~EM](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-~em)**() =default |
| const [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/) & | **[Run](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-run)**(bool calc_dynamics =true, bool calc_Q =true, bool calc_init =true, bool calc_output =true, bool calc_measurement =true, size_t max_iter =100, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) tol =1e-2)<br>Runs fitting by Expectation(E)-Maximization(M)  |
| std::tuple< [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) >, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > > | **[ReturnData](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-returndata)**()<br>Returns the input/output data to caller.  |
| const std::vector< Matrix > & | **[x](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-x)**() const<br>gets estimated state (over time)  |
| const std::vector< Matrix > & | **[y](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-y)**() const<br>gets estimated output (over time)  |
| const Matrix & | **[sum_E_x_t_x_t](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-sum-e-x-t-x-t)**() const<br>gets state-input covariance  |
| const Matrix & | **[sum_E_xu_tm1_xu_tm1](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-sum-e-xu-tm1-xu-tm1)**() const<br>gets state-input covariance (t-minus-1)  |
| const Matrix & | **[sum_E_xu_t_xu_tm1](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-sum-e-xu-t-xu-tm1)**() const<br>gets single lag state-input covariance  |
| size_t | **[n_t_tot](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-n-t-tot)**()<br>total number of time samples  |
| const Vector & | **[theta](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-theta)**() const<br>gets parameters updated in M step  |

## Protected Functions

|                | Name           |
| -------------- | -------------- |
| void | **[Expectation](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-expectation)**(bool force_common_initial =false)<br>Expectation step.  |
| void | **[Maximization](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-maximization)**(bool calc_dynamics =true, bool calc_Q =true, bool calc_init =false, bool calc_output =false, bool calc_measurement =false)<br>Maximization step.  |
| void | **[MaximizeDynamics](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-maximizedynamics)**() |
| void | **[MaximizeQ](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-maximizeq)**() |
| void | **[MaximizeInitial](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-maximizeinitial)**() |
| virtual void | **[MaximizeOutput](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-maximizeoutput)**() =0 |
| virtual void | **[MaximizeMeasurement](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-maximizemeasurement)**() =0 |
| void | **[Smooth](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-smooth)**(bool force_common_initial)<br>get smoothed estimates  |
| virtual void | **[RecurseKe](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-recurseke)**(Matrix & Ke, Cube & P_pre, Cube & P_post, size_t t) =0<br>recursively update estimator gain Ke  |
| void | **[Reset](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-reset)**()<br>reset to initial conditions  |
| void | **[InitVars](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-initvars)**()<br>Initializes the variables.  |
| Vector | **[UpdateTheta](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-updatetheta)**()<br>updates parameter list, theta  |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > | **[u_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-u-)** <br>input training data  |
| [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > | **[z_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-z-)** <br>measurement training data  |
| std::vector< Matrix > | **[x_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-x-)** <br>state estimate  |
| std::vector< Cube > | **[P_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-p-)** <br>state estimate cov  |
| std::vector< Cube > | **[P_t_tm1_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-p-t-tm1-)** <br>single-lag state covariance  |
| std::vector< Matrix > | **[y_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-y-)** <br>output estimate  |
| Matrix | **[diag_y_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-diag-y-)**  |
| Matrix | **[sum_E_x_t_x_t_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-sum-e-x-t-x-t-)** <br>state covariance (current time)  |
| Matrix | **[sum_E_xu_tm1_xu_tm1_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-sum-e-xu-tm1-xu-tm1-)** <br>state-input covariance (t-minus-1)  |
| Matrix | **[sum_E_xu_t_xu_tm1_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-sum-e-xu-t-xu-tm1-)** <br>single lag state-input covariance  |
| [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/) | **[fit_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-fit-)**  |
| Vector | **[theta_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-theta-)**  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[dt_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-dt-)** <br>sample period  |
| size_t | **[n_u_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-n-u-)** <br>number of inputs  |
| size_t | **[n_x_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-n-x-)** <br>number of states  |
| size_t | **[n_y_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-n-y-)** <br>number of outputs  |
| size_t | **[n_trials_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-n-trials-)** <br>number of input/output data sequences  |
| std::vector< size_t > | **[n_t_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-n-t-)** <br>number of time steps  |
| size_t | **[n_t_tot_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-n-t-tot-)** <br>total number of time steps across trials  |

## Detailed Description

```cpp
template <typename Fit >
class lds::EM;
```


---
---
## Public Function Details

### **EM**

```cpp
EM() =default
```



---
### **EM**

```cpp
EM(
    size_t n_x,
    data_t dt,
    UniformMatrixList< kMatFreeDim2 > && u_train,
    UniformMatrixList< kMatFreeDim2 > && z_train
)
```



**Parameters**:

  * **n_x** number of states 
  * **dt** sample period 
  * **u_train** input training data 
  * **z_train** measurement training data 


---
### **EM**

```cpp
EM(
    const Fit & fit0,
    UniformMatrixList< kMatFreeDim2 > && u_train,
    UniformMatrixList< kMatFreeDim2 > && z_train
)
```



**Parameters**:

  * **fit0** initial fit 
  * **u_train** input training data 
  * **z_train** measurement training data 


---
### **~EM**

```cpp
virtual ~EM() =default
```



---
### **Run**

```cpp
const Fit & Run(
    bool calc_dynamics =true,
    bool calc_Q =true,
    bool calc_init =true,
    bool calc_output =true,
    bool calc_measurement =true,
    size_t max_iter =100,
    data_t tol =1e-2
)
```



**Parameters**:

  * **calc_dynamics** [optional] whether to calculate dynamics (A, B) 
  * **calc_Q** [optional] whether to calculate process noise covariance 
  * **calc_init** [optional] whether to calculate initial conditions 
  * **calc_output** [optional] whether to calculate output function 
  * **calc_measurement** [optional] whether to calculate parameters for measurement/observation law 
  * **max_iter** max number of iterations 
  * **tol** convergence tolerance (max fractional abs change)


**Return**: [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/)

---
### **ReturnData**

```cpp
inline std::tuple< UniformMatrixList< kMatFreeDim2 >, UniformMatrixList< kMatFreeDim2 > > ReturnData()
```



**Return**: tuple(input data, output data) 

---
### **x**

```cpp
inline const std::vector< Matrix > & x() const
```



---
### **y**

```cpp
inline const std::vector< Matrix > & y() const
```



---
### **sum_E_x_t_x_t**

```cpp
inline const Matrix & sum_E_x_t_x_t() const
```



---
### **sum_E_xu_tm1_xu_tm1**

```cpp
inline const Matrix & sum_E_xu_tm1_xu_tm1() const
```



---
### **sum_E_xu_t_xu_tm1**

```cpp
inline const Matrix & sum_E_xu_t_xu_tm1() const
```



---
### **n_t_tot**

```cpp
inline size_t n_t_tot()
```



---
### **theta**

```cpp
inline const Vector & theta() const
```



---


## Protected Function Details

### **Expectation**

```cpp
void Expectation(
    bool force_common_initial =false
)
```



**Parameters**:

  * **force_common_initial** whether to force common initial condition for all trials 


---
### **Maximization**

```cpp
void Maximization(
    bool calc_dynamics =true,
    bool calc_Q =true,
    bool calc_init =false,
    bool calc_output =false,
    bool calc_measurement =false
)
```



**Parameters**:

  * **calc_dynamics** [optional] whether to caclulate dynamics (A, B) 
  * **calc_Q** [optional] whether to calculate process noise covariance 
  * **calc_init** [optional] whether to calculate initial conditions 
  * **calc_output** [optional] whether to calculate output function 
  * **calc_measurement** [optional] whether to calculate parameters for measurement/observation law 


---
### **MaximizeDynamics**

```cpp
void MaximizeDynamics()
```



---
### **MaximizeQ**

```cpp
void MaximizeQ()
```



---
### **MaximizeInitial**

```cpp
void MaximizeInitial()
```



---
### **MaximizeOutput**

```cpp
virtual void MaximizeOutput() =0
```



**Reimplemented by**: [lds::gaussian::FitEM::MaximizeOutput](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_fit_e_m/#function-maximizeoutput), [lds::poisson::FitEM::MaximizeOutput](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_fit_e_m/#function-maximizeoutput)


---
### **MaximizeMeasurement**

```cpp
virtual void MaximizeMeasurement() =0
```



**Reimplemented by**: [lds::gaussian::FitEM::MaximizeMeasurement](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_fit_e_m/#function-maximizemeasurement), [lds::poisson::FitEM::MaximizeMeasurement](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_fit_e_m/#function-maximizemeasurement)


---
### **Smooth**

```cpp
void Smooth(
    bool force_common_initial
)
```



**Parameters**:

  * **force_common_initial** whether to force common initial conditions 


---
### **RecurseKe**

```cpp
virtual void RecurseKe(
    Matrix & Ke,
    Cube & P_pre,
    Cube & P_post,
    size_t t
) =0
```



**Parameters**:

  * **Ke** estimator gain 
  * **P_pre** cov of predicted state est. 
  * **P_post** cov of postior sate est. 
  * **t** time 


**Reimplemented by**: [lds::gaussian::FitEM::RecurseKe](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_fit_e_m/#function-recurseke), [lds::poisson::FitEM::RecurseKe](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_fit_e_m/#function-recurseke)


---
### **Reset**

```cpp
void Reset()
```



---
### **InitVars**

```cpp
void InitVars()
```



---
### **UpdateTheta**

```cpp
Vector UpdateTheta()
```



**Return**: parameter list 

---


## Protected Attribute Details

### **u_**

```cpp
UniformMatrixList< kMatFreeDim2 > u_;
```



---
### **z_**

```cpp
UniformMatrixList< kMatFreeDim2 > z_;
```



---
### **x_**

```cpp
std::vector< Matrix > x_;
```



---
### **P_**

```cpp
std::vector< Cube > P_;
```



---
### **P_t_tm1_**

```cpp
std::vector< Cube > P_t_tm1_;
```



---
### **y_**

```cpp
std::vector< Matrix > y_;
```



---
### **diag_y_**

```cpp
Matrix diag_y_;
```



---
### **sum_E_x_t_x_t_**

```cpp
Matrix sum_E_x_t_x_t_;
```



---
### **sum_E_xu_tm1_xu_tm1_**

```cpp
Matrix sum_E_xu_tm1_xu_tm1_;
```



---
### **sum_E_xu_t_xu_tm1_**

```cpp
Matrix sum_E_xu_t_xu_tm1_;
```



---
### **fit_**

```cpp
Fit fit_;
```



---
### **theta_**

```cpp
Vector theta_;
```



---
### **dt_**

```cpp
data_t dt_ {};
```



---
### **n_u_**

```cpp
size_t n_u_ {};
```



---
### **n_x_**

```cpp
size_t n_x_ {};
```



---
### **n_y_**

```cpp
size_t n_y_ {};
```



---
### **n_trials_**

```cpp
size_t n_trials_ {};
```



---
### **n_t_**

```cpp
std::vector< size_t > n_t_;
```



---
### **n_t_tot_**

```cpp
size_t n_t_tot_ {};
```



---


-------------------------------

Updated on 31 March 2025 at 16:04:30 EDT
