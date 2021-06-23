---
title: lds::gaussian::FitSSID
summary: Subspace Identification (SSID) for GLDS. 

---

# lds::gaussian::FitSSID



Subspace Identification ([SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/)) for GLDS. 
<br /> `#include <lds_gaussian_fit_ssid.h>`

Inherits from [lds::SSID< Fit >](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/)

## Additional inherited members

**Public Functions inherited from [lds::SSID< Fit >](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/)**

|                | Name           |
| -------------- | -------------- |
| | **[SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-ssid)**() =default<br>Constructs a new [SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/)[Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/) type.  |
| | **[SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-ssid)**(size_t n_x, size_t n_h, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) dt, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > && u_train, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > && z_train, const Vector & d =Vector(1).fill(-kInf))<br>Constructs a new [SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/)[Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/) type.  |
| std::tuple< [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/), Vector > | **[Run](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-run)**([SSIDWt](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enum-ssidwt) ssid_wt)<br>Runs fitting by subspace identification ([SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/))  |
| std::tuple< [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) >, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > > | **[ReturnData](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-returndata)**()<br>Returns the I/O data to caller.  |

**Protected Functions inherited from [lds::SSID< Fit >](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/)**

|                | Name           |
| -------------- | -------------- |
| void | **[CalcD](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-calcd)**([data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) t_silence =0.1, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) thresh_silence =0.001)<br>Using periods of silence in inputs (u), calculates the output \ bias (d)  |
| void | **[CreateHankelDataMat](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-createhankeldatamat)**()<br>Creates the block-hankel I/O data matrix.  |
| void | **[CalcSVD](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-calcsvd)**([SSIDWt](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enum-ssidwt) wt)<br>performs the singular value decomposition (SVD)  |
| void | **[Solve](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-solve)**([data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) wt_dc)<br>solves for LDS parameters  |
| void | **[RecomputeExtObs](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-recomputeextobs)**()<br>recompute extended observability matrix from estimates of A, C  |

**Protected Attributes inherited from [lds::SSID< Fit >](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/)**

|                | Name           |
| -------------- | -------------- |
| [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > | **[u_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-u_)** <br>input training data  |
| [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > | **[z_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-z_)** <br>measurement training data  |
| Matrix | **[D_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-d_)** <br>block-Hankel I/O data matrix  |
| [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/) | **[fit_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-fit_)** <br>fit  |
| Matrix | **[g_dc_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-g_dc_)** <br>I/O gain @ DC.  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) | **[dt_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-dt_)** <br>sample period  |
| size_t | **[n_u_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n_u_)** <br>number of inputs  |
| size_t | **[n_x_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n_x_)** <br>number of states  |
| size_t | **[n_y_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n_y_)** <br>number of outputs  |
| size_t | **[n_h_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n_h_)**  |
| size_t | **[n_trials_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n_trials_)** <br>number of input/output data sequences  |
| std::vector< size_t > | **[n_t_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n_t_)** <br>number of time steps  |
| size_t | **[n_t_tot_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n_t_tot_)** <br>total number of time steps across trials  |
| Matrix | **[L_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-l_)** <br>lower triangle decomp of covariance matrix  |
| Vector | **[s_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-s_)** <br>singular values  |
| Matrix | **[ext_obs_t_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-ext_obs_t_)** <br>extended observability matrix  |


---
---
-------------------------------

Updated on 22 June 2021 at 23:08:17 CDT
