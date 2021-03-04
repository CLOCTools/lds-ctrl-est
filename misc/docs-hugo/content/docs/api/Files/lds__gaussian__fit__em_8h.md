---
title: ldsCtrlEst_h/lds_gaussian_fit_em.h
summary: GLDS E-M fit type. 

---

# ldsCtrlEst_h/lds_gaussian_fit_em.h

GLDS E-M fit type.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/ldsctrlest/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |
| **[lds::gaussian](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/)** <br>Linear Dynamical Systems with Gaussian observations.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::gaussian::emFit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/)** <br>GLDS E-M Fit Type.  |

## Detailed Description



This file declares and partially defines a type by which Gaussian-output LDS models are fit by the expectation-maximization (EM) algorithm (`[lds::gaussian::emFit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1em_fit__t/)`).

References: [1] Shumway RH, Stoffer DS. (1982) An Approach to Time Series Smoothing and Forecasting Using the EM Algorithm.Journal of Time Series Analysis 3(2).

[2] Ghahramani Z, Hinton GE. (1996) Parameter Estimation for Linear Dynamical Systems. Technical Report CRG-TR-96-2. 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_gaussian_fit_em.h - GLDS Fit (EM) ------*- C++ -*-===//
//
// Copyright 2021 [name of copyright owner]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//===----------------------------------------------------------------------===//
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_GAUSSIAN_FIT_EM_H
#define LDSCTRLEST_LDS_GAUSSIAN_FIT_EM_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace gaussian {
class emFit_t : public fit_t {
 public:
  emFit_t() : fit_t(){};
  emFit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q,
          armaVec& x0, armaMat& P0, armaMat& C, armaVec& d, armaMat& R,
          data_t dt, std::vector<armaMat>& uTrain,
          std::vector<armaMat>& zTrain);
  emFit_t(data_t dt, std::vector<armaMat>& uTrain,
          std::vector<armaMat>& zTrain);
  emFit_t(ssidFit_t& fit0);

  void reset(void);

  void runEM(bool calcAB = true, bool calcQ = true, bool calcInitial = true,
             bool calcC = true, bool calcd = true, bool calcR = true);

  data_t tol = 1e-3;     
  size_t maxIter = 100;  

  // get methods
  std::vector<armaMat> getX() { return xHat; };
  std::vector<armaMat> getY() { return yHat; };

  // these are covariances that can be useful to the end-user
  armaMat get_sum_E_xu_tm1_xu_tm1() { return sum_E_xu_tm1_xu_tm1; };
  armaMat get_sum_E_xu_t_xu_tm1() { return sum_E_xu_t_xu_tm1; };
  armaMat get_sum_E_xu_t_xu_t() { return sum_E_xu_t_xu_t; };
  size_t getT() { return T; };

  void setY(std::vector<armaMat> yHat) { this->yHat = yHat; };

 protected:
  armaVec getTheta();

  void kalmanFilt();

  void kalmanSmooth(bool forceCommonInitial);

  void Estep(bool forceCommonInitial = false);

  void Mstep(bool calcAB = true, bool calcQ = true, bool calcInitial = false,
             bool calcC = false, bool calcd = false, bool calcR = true);

  std::vector<armaMat> xHat;                
  std::vector<arma::Cube<data_t>> P;        
  std::vector<armaMat> yHat;                
  std::vector<arma::Cube<data_t>> P_t_tm1;  

  armaMat sum_E_xu_t_xu_t;      
  armaMat sum_E_xu_tm1_xu_tm1;  
  armaMat sum_E_xu_t_xu_tm1;    
  size_t T;
};

emFit_t emFit_x_equals_y(std::vector<armaMat>& uTrain,
                         std::vector<armaMat>& zTrain, data_t dt,
                         std::size_t maxIter = 100, data_t tol = 1e-3,
                         data_t q0 = 1e-6, bool calcAB = true,
                         bool calcQ = true, bool calcR = true);
};  // namespace gaussian
}  // namespace lds

#endif
```


-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
