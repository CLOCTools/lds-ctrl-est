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
| class | **[lds::gaussian::FitSSID](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1_fit_s_s_i_d/)** <br>Subspace Identification ([SSID](/ldsctrlest/docs/api/classes/classlds_1_1_s_s_i_d/)) for GLDS.  |

## Detailed Description



This file declares and partially defines a type by which Gaussian-output LDS models are fit by a subspace identification (SSID) algorithm (`lds::gaussian::ssidFit_t`).

References: [1] van Overschee P, de Moore B. (1996) Subspace Identification for Linear Systems. Boston: Springer. 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_gaussian_fit_ssid.h - GLDS Fit (SSID) --*- C++ -*-===//
//
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

#ifndef LDSCTRLEST_LDS_GAUSSIAN_FIT_SSID_H
#define LDSCTRLEST_LDS_GAUSSIAN_FIT_SSID_H

#include "lds_fit_ssid.h"
#include "lds_gaussian_fit.h"

namespace lds {
namespace gaussian {
class FitSSID : public SSID<Fit> {
 public:
  using SSID<Fit>::SSID;
  using SSID<Fit>::Run;

 private:
  using SSID<Fit>::CreateHankelDataMat;
  using SSID<Fit>::CalcSVD;
  using SSID<Fit>::Solve;

  void DecomposeData() override;

  void SolveVanOverschee();
};
}  // namespace gaussian
}  // namespace lds
#endif
```


-------------------------------

Updated on 23 March 2021 at 09:14:15 CDT
