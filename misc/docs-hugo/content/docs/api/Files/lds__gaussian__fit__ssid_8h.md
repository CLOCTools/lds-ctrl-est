---
title: ldsCtrlEst_h/lds_gaussian_fit_ssid.h
summary: GLDS SSID fit type. 

---

# ldsCtrlEst_h/lds_gaussian_fit_ssid.h

GLDS SSID fit type.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/ldsctrlest/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |
| **[lds::gaussian](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/)** <br>Linear Dynamical Systems with Gaussian observations.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::gaussian::ssidFit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ssid_fit__t/)** <br>GLDS SSID Fit Type.  |

## Detailed Description



This file declares and partially defines a type by which Gaussian-output LDS models are fit by a subspace identification (SSID) algorithm (`[lds::gaussian::ssidFit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1ssid_fit__t/)`).

References: [1] van Overschee P, de Moore B. (1996) Subspace Identification for Linear Systems. Boston: Springer. 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_gaussian_fit_ssid.h - GLDS Fit (SSID) --*- C++ -*-===//
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

#ifndef LDSCTRLEST_LDS_GAUSSIAN_FIT_SSID_H
#define LDSCTRLEST_LDS_GAUSSIAN_FIT_SSID_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace gaussian {
class ssidFit_t : public fit_t {
 public:
  ssidFit_t() : fit_t(){};
  ssidFit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q,
            armaVec& x0, armaMat& P0, armaMat& C, armaMat& D, armaVec& d,
            armaMat& R, data_t dt, data_t t_startSSID, data_t t_stopSSID,
            armaVec& singVals, std::vector<data_t>& t0,
            std::vector<armaMat>& uTrain, std::vector<armaMat>& zTrain);
  std::vector<data_t> t0;
  data_t t_startSSID;
  data_t t_stopSSID;
  armaVec singVals;
};

ssidFit_t ssidFit(std::vector<armaMat>& u, std::vector<armaMat>& z, data_t dt,
                  size_t nX, size_t nH = 25, armaVec d0 = armaVec(1).fill(-inf),
                  bool force_unitNormC = false, ssidWt wt = NONE,
                  data_t wtG0 = 0.0, std::vector<data_t>& t0 = DEFAULT_T0,
                  data_t t_startSSID = -std::numeric_limits<data_t>::infinity(),
                  data_t t_stopSSID = std::numeric_limits<data_t>::infinity());

armaVec calcD_silence(std::vector<armaMat>& n, std::vector<armaMat>& u,
                      data_t dt, data_t tSilence = 0.1,
                      data_t threshSilence = 0.001);

void createHankelMat_n4sid(armaMat& u, armaMat& z, std::size_t& nH, armaMat& D);

void ssid_n4sid(std::size_t& nX, std::size_t& nU, std::size_t& nY,
                std::size_t& nH, armaMat& uSSID, armaMat& zSSID, armaMat& A,
                armaMat& B, armaMat& C, armaMat& D, armaMat& Q, armaMat& R,
                armaVec& m, armaVec& d, armaVec& s, bool force_unitNormC,
                ssidWt wt, data_t wtG0);

void ssid_n4sid_vanOverschee(std::size_t& nX, std::size_t& nU, std::size_t& nY,
                             std::size_t& nH, armaMat& uSSID, armaMat& zSSID,
                             armaMat& A, armaMat& B, armaMat& C, armaMat& D,
                             armaMat& Q, armaMat& R, armaVec& s, ssidWt wt);

void recompute_extObs(armaMat& extObs, armaMat& A, armaMat& C, std::size_t& nY,
                      std::size_t& nH);

void lq(armaMat& L, armaMat& Qt, armaMat& X);

armaMat calcCov(armaMat& A, armaMat& B);

armaMat sqrtmat_svd(armaMat& X);
}  // namespace gaussian
}  // namespace lds
#endif
```


-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
