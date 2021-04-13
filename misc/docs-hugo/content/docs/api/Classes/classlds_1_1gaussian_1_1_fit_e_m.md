---
title: lds::gaussian::FitEM
summary: GLDS E-M Fit Type. 

---

# lds::gaussian::FitEM



GLDS E-M [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_fit/) Type.  [More...](#detailed-description)


<br /> `#include <lds_gaussian_fit_em.h>`

Inherits from [lds::EM< Fit >](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/)

## Additional inherited members

**Public Functions inherited from [lds::EM< Fit >](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/)**

|                | Name           |
| -------------- | -------------- |
| | **[EM](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-em)**() =default<br>Constructs a new [EM](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/)[Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/) type.  |
| | **[EM](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-em)**(size_t n_x, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) dt, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > && u_train, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > && z_train)<br>Constructs a new [EM](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/)[Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/) type.  |
| | **[EM](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-em)**(const [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/) & fit0, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > && u_train, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > && z_train)<br>Constructs a new [EM](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/)[Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/) type.  |
| const [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/) & | **[Run](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-run)**(bool calc_dynamics =true, bool calc_Q =true, bool calc_init =true, bool calc_output =true, bool calc_measurement =true, size_t max_iter =100, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) tol =1e-2)<br>Runs fitting by Expectation(E)-Maximization(M)  |
| std::tuple< [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) >, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > > | **[ReturnData](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-returndata)**()<br>Returns the input/output data to caller.  |
| const std::vector< Matrix > & | **[x](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-x)**() const<br>gets estimated state (over time)  |
| const std::vector< Matrix > & | **[y](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-y)**() const<br>gets estimated output (over time)  |
| const Matrix & | **[sum_E_x_t_x_t](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-sum_e_x_t_x_t)**() const<br>gets state-input covariance  |
| const Matrix & | **[sum_E_xu_tm1_xu_tm1](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-sum_e_xu_tm1_xu_tm1)**() const<br>gets state-input covariance (t-minus-1)  |
| const Matrix & | **[sum_E_xu_t_xu_tm1](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-sum_e_xu_t_xu_tm1)**() const<br>gets single lag state-input covariance  |
| size_t | **[n_t_tot](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-n_t_tot)**()<br>total number of time samples  |
| const Vector & | **[theta](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-theta)**() const<br>gets parameters updated in M step  |

**Protected Functions inherited from [lds::EM< Fit >](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/)**

|                | Name           |
| -------------- | -------------- |
| void | **[Expectation](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-expectation)**(bool force_common_initial =false)<br>Expectation step.  |
| void | **[Maximization](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-maximization)**(bool calc_dynamics =true, bool calc_Q =true, bool calc_init =false, bool calc_output =false, bool calc_measurement =false)<br>Maximization step.  |
| void | **[MaximizeDynamics](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-maximizedynamics)**() |
| void | **[MaximizeQ](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-maximizeq)**() |
| void | **[MaximizeInitial](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-maximizeinitial)**() |
| void | **[Smooth](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-smooth)**(bool force_common_initial)<br>get smoothed estimates  |
| void | **[Reset](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-reset)**()<br>reset to initial conditions  |
| void | **[InitVars](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-initvars)**()<br>Initializes the variables.  |
| Vector | **[UpdateTheta](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#function-updatetheta)**()<br>updates parameter list, theta  |

**Protected Attributes inherited from [lds::EM< Fit >](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/)**

|                | Name           |
| -------------- | -------------- |
| [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > | **[u_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-u_)** <br>input training data  |
| [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > | **[z_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-z_)** <br>measurement training data  |
| std::vector< Matrix > | **[x_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-x_)** <br>state estimate  |
| std::vector< Cube > | **[P_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-p_)** <br>state estimate cov  |
| std::vector< Cube > | **[P_t_tm1_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-p_t_tm1_)** <br>single-lag state covariance  |
| std::vector< Matrix > | **[y_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-y_)** <br>output estimate  |
| Matrix | **[diag_y_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-diag_y_)**  |
| Matrix | **[sum_E_x_t_x_t_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-sum_e_x_t_x_t_)** <br>state covariance (current time)  |
| Matrix | **[sum_E_xu_tm1_xu_tm1_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-sum_e_xu_tm1_xu_tm1_)** <br>state-input covariance (t-minus-1)  |
| Matrix | **[sum_E_xu_t_xu_tm1_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-sum_e_xu_t_xu_tm1_)** <br>single lag state-input covariance  |
| [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/) | **[fit_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-fit_)**  |
| Vector | **[theta_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-theta_)**  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) | **[dt_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-dt_)** <br>sample period  |
| size_t | **[n_u_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-n_u_)** <br>number of inputs  |
| size_t | **[n_x_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-n_x_)** <br>number of states  |
| size_t | **[n_y_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-n_y_)** <br>number of outputs  |
| size_t | **[n_trials_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-n_trials_)** <br>number of input/output data sequences  |
| std::vector< size_t > | **[n_t_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-n_t_)** <br>number of time steps  |
| size_t | **[n_t_tot_](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/#variable-n_t_tot_)** <br>total number of time steps across trials  |


## Detailed Description

```cpp
class lds::gaussian::FitEM;
```





```
        This type is used in the process of fitting GLDS models by
        expectation-maximization (EM).
```

---
---
-------------------------------

Updated on 30 March 2021 at 15:49:43 CDT
