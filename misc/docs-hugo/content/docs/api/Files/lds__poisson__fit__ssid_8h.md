---
title: ldsCtrlEst_h/lds_poisson_fit_ssid.h
summary: PLDS SSID fit type. 

---

# ldsCtrlEst_h/lds_poisson_fit_ssid.h

PLDS SSID fit type.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/ldsctrlest/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |
| **[lds::poisson](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/)** <br>Linear Dynamical Systems with Poisson observations.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::poisson::ssidFit_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1ssid_fit__t/)** <br>GLDS SSID Fit Type.  |

## Detailed Description



This file declares and partially defines a type by which Poisson-output LDS models are fit by a subspace identification (SSID) algorithm (`[lds::gaussian::ssidFit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ssid_fit__t/)`). Currently, these models are fit assuming linear/Gaussian output, and then an exponential output is fit by maximum likelihood under the Poisson assumption. Future work should implement SSID after Poisson<->Gaussian moment conversions as outlined in [2].

References: [1] van Overschee P, de Moore B. (1996) Subspace Identification for Linear Systems. Boston: Springer. [2] Buesing L, Macke JH, Sahani M. (2012) Spectral learning of linear dynamics from generalised-linear observations with application to neural population data. NIPS 25. 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_poisson_fit_ssid.h - PLDS Fit (SSID) ---*- C++ -*-===//
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

#ifndef LDSCTRLEST_LDS_POISSON_FIT_SSID_H
#define LDSCTRLEST_LDS_POISSON_FIT_SSID_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace poisson {
class ssidFit_t : public fit_t {
 public:
  ssidFit_t() : fit_t(){};
  ssidFit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q,
            armaVec& x0, armaMat& P0, armaMat& C, armaVec& d, data_t dt,
            data_t t_startSSID, data_t t_stopSSID, armaVec& singVals,
            std::vector<data_t>& t0, std::vector<armaMat>& uTrain,
            std::vector<armaMat>& zTrain);
  std::vector<data_t> t0;
  data_t t_startSSID;
  data_t t_stopSSID;
  armaVec singVals;
};

std::tuple<ssidFit_t, lds::gaussian::ssidFit_t> ssidFit(
    std::vector<armaMat>& u, std::vector<armaMat>& z, data_t dt, size_t nX,
    size_t nH = 25, armaVec d0 = armaVec(1).fill(-inf), ssidWt wt = NONE,
    data_t wtG0 = 0.0, std::vector<data_t>& t0 = DEFAULT_T0,
    data_t t_startSSID = -std::numeric_limits<data_t>::infinity(),
    data_t t_stopSSID = std::numeric_limits<data_t>::infinity(),
    bool assumeGaussian = true);

void newtonSolveC_mle(armaMat& C, armaVec& d, std::vector<armaMat>& x,
                      std::vector<armaMat>& n, data_t& dt);

data_t newtonSolve_rescaleC_mle(armaMat& C, armaVec& d, std::vector<armaMat>& x,
                                std::vector<armaMat>& n, data_t& dt);

void calcD_mle(armaMat& C, armaVec& d, std::vector<armaMat>& x,
               std::vector<armaMat>& n, data_t& dt);

data_t calcD_newtonSolve_rescaleC_mle(armaMat& C, armaVec& d,
                                      std::vector<armaMat>& x,
                                      std::vector<armaMat>& n, data_t& dt);

void fitOutput_mle(armaMat& C, armaVec& d, std::vector<armaMat>& x,
                   std::vector<armaMat>& n, data_t& dt);
}  // namespace poisson
}  // namespace lds
#endif
```


-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
