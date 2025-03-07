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
| | **[SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-ssid)**(size_t n_x, size_t n_h, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) dt, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > && u_train, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > && z_train, const Vector & d =Vector(1).fill(-kInf))<br>Constructs a new [SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/)[Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/) type.  |
| std::tuple< [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/), Vector > | **[Run](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-run)**([SSIDWt](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enum-ssidwt) ssid_wt)<br>Runs fitting by subspace identification ([SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/))  |
| std::tuple< [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) >, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > > | **[ReturnData](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-returndata)**()<br>Returns the I/O data to caller.  |

**Protected Functions inherited from [lds::SSID< Fit >](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/)**

|                | Name           |
| -------------- | -------------- |
| void | **[CalcD](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-calcd)**([data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) t_silence =0.1, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) thresh_silence =0.001)<br>Using periods of silence in inputs (u), calculates the output \ bias (d)  |
| void | **[CreateHankelDataMat](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-createhankeldatamat)**()<br>Creates the block-hankel I/O data matrix.  |
| void | **[CalcSVD](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-calcsvd)**([SSIDWt](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enum-ssidwt) wt)<br>performs the singular value decomposition (SVD)  |
| void | **[Solve](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-solve)**([data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) wt_dc)<br>solves for LDS parameters  |
| void | **[RecomputeExtObs](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-recomputeextobs)**()<br>recompute extended observability matrix from estimates of A, C  |

**Protected Attributes inherited from [lds::SSID< Fit >](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/)**

|                | Name           |
| -------------- | -------------- |
| [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > | **[u_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-u-)** <br>input training data  |
| [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > | **[z_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-z-)** <br>measurement training data  |
| Matrix | **[D_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-d-)** <br>block-Hankel I/O data matrix  |
| [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/) | **[fit_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-fit-)** <br>fit  |
| Matrix | **[g_dc_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-g-dc-)** <br>I/O gain @ DC.  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[dt_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-dt-)** <br>sample period  |
| size_t | **[n_u_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n-u-)** <br>number of inputs  |
| size_t | **[n_x_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n-x-)** <br>number of states  |
| size_t | **[n_y_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n-y-)** <br>number of outputs  |
| size_t | **[n_h_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n-h-)**  |
| size_t | **[n_trials_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n-trials-)** <br>number of input/output data sequences  |
| std::vector< size_t > | **[n_t_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n-t-)** <br>number of time steps  |
| size_t | **[n_t_tot_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-n-t-tot-)** <br>total number of time steps across trials  |
| Matrix | **[L_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-l-)** <br>lower triangle decomp of covariance matrix  |
| Vector | **[s_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-s-)** <br>singular values  |
| Matrix | **[ext_obs_t_](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#variable-ext-obs-t-)** <br>extended observability matrix  |


---
---
-------------------------------

Updated on  5 March 2025 at 21:41:27 EST
