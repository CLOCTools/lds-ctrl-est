---
title: ldsCtrlEst_h/lds_poisson_fit_ssid.h
summary: PLDS SSID fit type. 

---

# ldsCtrlEst_h/lds_poisson_fit_ssid.h

PLDS SSID fit type.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/lds-ctrl-est/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |
| **[lds::poisson](/lds-ctrl-est/docs/api/namespaces/namespacelds_1_1poisson/)** <br>Linear Dynamical Systems with Poisson observations.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::poisson::FitSSID](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1fitssid/)** <br>Subspace Identification ([SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1ssid/)) for PLDS.  |

## Detailed Description



This file declares and partially defines a type by which Poisson-output LDS models are fit by a subspace identification (SSID) algorithm (`[lds::gaussian::FitSSID](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1fitssid/)`).

References: [1] van Overschee P, de Moore B. (1996) Subspace Identification for Linear Systems. Boston: Springer. [2] Buesing L, Macke JH, Sahani M. (2012) Spectral learning of linear dynamics from generalised-linear observations with application to neural population data. NIPS 25. 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_poisson_fit_ssid.h - PLDS Fit (SSID) ---*- C++ -*-===//
//
// Copyright 2021 Michael Bolus
// Copyright 2021 Georgia Institute of Technology
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

#include "lds_fit_ssid.h"
#include "lds_poisson_fit.h"

namespace lds {
namespace poisson {
class FitSSID : public SSID<Fit> {
 public:
  using SSID<Fit>::SSID;

 private:
  void DecomposeData() override;

  void CalcCov();

  void PoissonToGaussianMoments();
  Matrix cov_;
};

}  // namespace poisson
}  // namespace lds
#endif
```


-------------------------------

Updated on  4 May 2022 at 16:59:51 Eastern Daylight Time
