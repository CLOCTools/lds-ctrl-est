---
title: lds::gaussian::FitSSID
summary: Subspace Identification (SSID) for GLDS. 

---

# lds::gaussian::FitSSID



Subspace Identification ([SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/)) for GLDS. 
<br /> `#include <lds_gaussian_fit_ssid.h>`

Inherits from [lds::SSID< Fit >](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/)

## Additional inherited members

**Public Functions inherited from [lds::SSID< Fit >](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/)**

|                | Name           |
| -------------- | -------------- |
| | **[SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#function-ssid)**() =default<br>Constructs a new [SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/)[Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/) type.  |
| | **[SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#function-ssid)**(size_t n_x, size_t n_h, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) dt, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > && u_train, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > && z_train, const Vector & d =Vector(1).fill(-kInf))<br>Constructs a new [SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/)[Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/) type.  |
| std::tuple< [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/), Vector > | **[Run](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#function-run)**([SSIDWt](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enum-ssidwt) ssid_wt)<br>Runs fitting by subspace identification ([SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/))  |
| std::tuple< [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) >, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > > | **[ReturnData](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#function-returndata)**()<br>Returns the I/O data to caller.  |

**Protected Functions inherited from [lds::SSID< Fit >](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/)**

|                | Name           |
| -------------- | -------------- |
| void | **[CalcD](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#function-calcd)**([data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) t_silence =0.1, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) thresh_silence =0.001)<br>Using periods of silence in inputs (u), calculates the output \ bias (d)  |
| void | **[CreateHankelDataMat](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#function-createhankeldatamat)**()<br>Creates the block-hankel I/O data matrix.  |
| void | **[CalcSVD](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#function-calcsvd)**([SSIDWt](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enum-ssidwt) wt)<br>performs the singular value decomposition (SVD)  |
| void | **[Solve](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#function-solve)**([data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) wt_dc)<br>solves for LDS parameters  |
| void | **[RecomputeExtObs](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#function-recomputeextobs)**()<br>recompute extended observability matrix from estimates of A, C  |

**Protected Attributes inherited from [lds::SSID< Fit >](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/)**

|                | Name           |
| -------------- | -------------- |
| [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > | **[u_](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#variable-u-)** <br>input training data  |
| [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformmatrixlist/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > | **[z_](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#variable-z-)** <br>measurement training data  |
| Matrix | **[D_](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#variable-d-)** <br>block-Hankel I/O data matrix  |
| [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1fit/) | **[fit_](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#variable-fit-)** <br>fit  |
| Matrix | **[g_dc_](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#variable-g-dc-)** <br>I/O gain @ DC.  |
| [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data-t) | **[dt_](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#variable-dt-)** <br>sample period  |
| size_t | **[n_u_](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#variable-n-u-)** <br>number of inputs  |
| size_t | **[n_x_](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#variable-n-x-)** <br>number of states  |
| size_t | **[n_y_](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#variable-n-y-)** <br>number of outputs  |
| size_t | **[n_h_](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#variable-n-h-)**  |
| size_t | **[n_trials_](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#variable-n-trials-)** <br>number of input/output data sequences  |
| std::vector< size_t > | **[n_t_](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#variable-n-t-)** <br>number of time steps  |
| size_t | **[n_t_tot_](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#variable-n-t-tot-)** <br>total number of time steps across trials  |
| Matrix | **[L_](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#variable-l-)** <br>lower triangle decomp of covariance matrix  |
| Vector | **[s_](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#variable-s-)** <br>singular values  |
| Matrix | **[ext_obs_t_](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/#variable-ext-obs-t-)** <br>extended observability matrix  |


---
---
-------------------------------

Updated on 19 May 2022 at 17:16:04 Eastern Daylight Time
