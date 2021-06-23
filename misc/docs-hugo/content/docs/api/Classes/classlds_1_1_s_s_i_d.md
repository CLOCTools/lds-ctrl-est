---
title: lds::SSID

---

# lds::SSID



 [More...](#detailed-description)

Inherited by [lds::gaussian::FitSSID](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_fit_s_s_i_d/), [lds::poisson::FitSSID](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_fit_s_s_i_d/)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-ssid)**() =default<br>Constructs a new [SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/)[Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/) type.  |
| | **[SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-ssid)**(size_t n_x, size_t n_h, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) dt, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > && u_train, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > && z_train, const Vector & d =Vector(1).fill(-kInf))<br>Constructs a new [SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/)[Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/) type.  |
| std::tuple< [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/), Vector > | **[Run](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-run)**([SSIDWt](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enum-ssidwt) ssid_wt)<br>Runs fitting by subspace identification ([SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/))  |
| std::tuple< [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) >, [UniformMatrixList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_matrix_list/)< [kMatFreeDim2](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enumvalue-kmatfreedim2) > > | **[ReturnData](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-returndata)**()<br>Returns the I/O data to caller.  |

## Protected Functions

|                | Name           |
| -------------- | -------------- |
| void | **[CalcD](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-calcd)**([data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) t_silence =0.1, [data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) thresh_silence =0.001)<br>Using periods of silence in inputs (u), calculates the output \ bias (d)  |
| void | **[CreateHankelDataMat](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-createhankeldatamat)**()<br>Creates the block-hankel I/O data matrix.  |
| virtual void | **[DecomposeData](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-decomposedata)**() =0<br>Decompose data to lower-triangular matrix (used in Solve)  |
| void | **[CalcSVD](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-calcsvd)**([SSIDWt](/lds-ctrl-est/docs/api/namespaces/namespacelds/#enum-ssidwt) wt)<br>performs the singular value decomposition (SVD)  |
| void | **[Solve](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-solve)**([data_t](/lds-ctrl-est/docs/api/namespaces/namespacelds/#using-data_t) wt_dc)<br>solves for LDS parameters  |
| void | **[RecomputeExtObs](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/#function-recomputeextobs)**()<br>recompute extended observability matrix from estimates of A, C  |

## Protected Attributes

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

## Detailed Description

```cpp
template <typename Fit >
class lds::SSID;
```


---
---
## Public Function Details

### **SSID**

```cpp
SSID() =default
```



---
### **SSID**

```cpp
SSID(
    size_t n_x,
    size_t n_h,
    data_t dt,
    UniformMatrixList< kMatFreeDim2 > && u_train,
    UniformMatrixList< kMatFreeDim2 > && z_train,
    const Vector & d =Vector(1).fill(-kInf)
)
```



**Parameters**:

  * **n_x** number of states 
  * **n_h** size of block-hankel data matrix 
  * **dt** sample period 
  * **u_train** input training data 
  * **z_train** measurement training data 
  * **d** output bias 


---
### **Run**

```cpp
std::tuple< Fit, Vector > Run(
    SSIDWt ssid_wt
)
```



**Parameters**:

  * **ssid_wt** weight for singular value decomp


**Return**: tuple ([Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/), singular values) 

---
### **ReturnData**

```cpp
inline std::tuple< UniformMatrixList< kMatFreeDim2 >, UniformMatrixList< kMatFreeDim2 > > ReturnData()
```



**Return**: tuple(input data, output data) 

---


## Protected Function Details

### **CalcD**

```cpp
void CalcD(
    data_t t_silence =0.1,
    data_t thresh_silence =0.001
)
```



**Parameters**:

  * **t_silence** threshold on period of time that qualifies as "silence" 
  * **thresh_silence** threshold on input amplitude u that qualifies as "silence" 


---
### **CreateHankelDataMat**

```cpp
void CreateHankelDataMat()
```



Creates the block-hankel I/O data matrix. Also calculates I/O gain @ DC. 


---
### **DecomposeData**

```cpp
virtual void DecomposeData() =0
```



**Reimplemented by**: [lds::gaussian::FitSSID::DecomposeData](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_fit_s_s_i_d/#function-decomposedata), [lds::poisson::FitSSID::DecomposeData](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_fit_s_s_i_d/#function-decomposedata)


---
### **CalcSVD**

```cpp
void CalcSVD(
    SSIDWt wt
)
```



**Parameters**:

  * **ssid_wt** weight for SVD 


---
### **Solve**

```cpp
void Solve(
    data_t wt_dc
)
```



**Parameters**:

  * **wt_dc** weight placed on getting correct DC I/O gain 


---
### **RecomputeExtObs**

```cpp
void RecomputeExtObs()
```



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
### **D_**

```cpp
Matrix D_;
```



---
### **fit_**

```cpp
Fit fit_;
```



---
### **g_dc_**

```cpp
Matrix g_dc_;
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
### **n_h_**

```cpp
size_t n_h_ {};
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
### **L_**

```cpp
Matrix L_;
```



---
### **s_**

```cpp
Vector s_;
```



---
### **ext_obs_t_**

```cpp
Matrix ext_obs_t_;
```



---


-------------------------------

Updated on 22 June 2021 at 23:08:17 CDT
