---
title: lds::gaussian::FitSSID
summary: Subspace Identification (SSID) for GLDS. 

---

# lds::gaussian::FitSSID



Subspace Identification ([SSID](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/)) for GLDS. 
<br /> `#include <lds_gaussian_fit_ssid.h>`

Inherits from [lds::SSID< Fit >](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/)

## Additional inherited members

**Public Functions inherited from [lds::SSID< Fit >](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/)**

|                | Name           |
| -------------- | -------------- |
| | **[SSID](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#function-ssid)**() =default<br>Constructs a new [SSID](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/)[Fit](/ldsctrlest/docs/api/classes/classlds_1_1_fit/) type.  |
| | **[SSID](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#function-ssid)**(size_t n_x, size_t n_h, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) dt, [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/ldsctrlest/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > && u_train, [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/ldsctrlest/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > && z_train, const Vector & d =Vector(1).fill(-kInf))<br>Constructs a new [SSID](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/)[Fit](/ldsctrlest/docs/api/classes/classlds_1_1_fit/) type.  |
| std::tuple< [Fit](/ldsctrlest/docs/api/classes/classlds_1_1_fit/), Vector > | **[Run](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#function-run)**([SSIDWt](/ldsctrlest/docs/api/namespaces/namespacelds/#enum-ssidwt) ssid_wt)<br>Runs fitting by subspace identification ([SSID](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/))  |
| std::tuple< [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/ldsctrlest/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) >, [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/ldsctrlest/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > > | **[ReturnData](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#function-returndata)**()<br>Returns the I/O data to caller.  |

**Protected Functions inherited from [lds::SSID< Fit >](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/)**

|                | Name           |
| -------------- | -------------- |
| void | **[CalcD](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#function-calcd)**([data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) t_silence =0.1, [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) thresh_silence =0.001)<br>Using periods of silence in inputs (u), calculates the output \ bias (d)  |
| void | **[CreateHankelDataMat](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#function-createhankeldatamat)**()<br>Creates the block-hankel I/O data matrix.  |
| void | **[CalcSVD](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#function-calcsvd)**([SSIDWt](/ldsctrlest/docs/api/namespaces/namespacelds/#enum-ssidwt) wt)<br>performs the singular value decomposition (SVD)  |
| void | **[Solve](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#function-solve)**([data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) wt_dc)<br>solves for LDS parameters  |
| void | **[RecomputeExtObs](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#function-recomputeextobs)**()<br>recompute extended observability matrix from estimates of A, C  |

**Protected Attributes inherited from [lds::SSID< Fit >](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/)**

|                | Name           |
| -------------- | -------------- |
| [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/ldsctrlest/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > | **[u_](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#variable-u_)** <br>input training data  |
| [UniformMatrixList](/ldsctrlest/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/ldsctrlest/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > | **[z_](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#variable-z_)** <br>measurement training data  |
| Matrix | **[D_](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#variable-d_)** <br>block-Hankel I/O data matrix  |
| [Fit](/ldsctrlest/docs/api/classes/classlds_1_1_fit/) | **[fit_](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#variable-fit_)** <br>fit  |
| Matrix | **[g_dc_](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#variable-g_dc_)** <br>I/O gain @ DC.  |
| [data_t](/ldsctrlest/docs/api/namespaces/namespacelds/#using-data_t) | **[dt_](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#variable-dt_)** <br>sample period  |
| size_t | **[n_u_](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n_u_)** <br>number of inputs  |
| size_t | **[n_x_](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n_x_)** <br>number of states  |
| size_t | **[n_y_](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n_y_)** <br>number of outputs  |
| size_t | **[n_h_](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n_h_)**  |
| size_t | **[n_trials_](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n_trials_)** <br>number of input/output data sequences  |
| std::vector< size_t > | **[n_t_](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n_t_)** <br>number of time steps  |
| size_t | **[n_t_tot_](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n_t_tot_)** <br>total number of time steps across trials  |
| Matrix | **[L_](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#variable-l_)** <br>lower triangle decomp of covariance matrix  |
| Vector | **[s_](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#variable-s_)** <br>singular values  |
| Matrix | **[ext_obs_t_](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/#variable-ext_obs_t_)** <br>extended observability matrix  |


---
---
-------------------------------

Updated on 23 March 2021 at 09:14:14 CDT
