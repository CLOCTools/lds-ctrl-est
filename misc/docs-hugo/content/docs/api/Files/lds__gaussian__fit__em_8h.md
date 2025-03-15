---
title: ldsCtrlEst_h/lds_gaussian_fit_em.h
summary: GLDS E-M fit type. 

---

# ldsCtrlEst_h/lds_gaussian_fit_em.h

GLDS E-M fit type.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/lds-ctrl-est/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |
| **[lds::gaussian](/lds-ctrl-est/docs/api/namespaces/namespacelds_1_1gaussian/)** <br>Linear Dynamical Systems with Gaussian observations.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::gaussian::FitEM](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_fit_e_m/)** <br>GLDS E-M [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1_fit/) Type.  |

## Detailed Description



This file declares and partially defines a type by which Gaussian-output LDS models are fit by the expectation-maximization (EM) algorithm (`lds::gaussian::emFit_t`).

References: [1] Shumway RH, Stoffer DS. (1982) An Approach to Time Series Smoothing and Forecasting Using the EM Algorithm.Journal of Time Series Analysis 3(2).

[2] Ghahramani Z, Hinton GE. (1996) Parameter Estimation for Linear Dynamical Systems. Technical Report CRG-TR-96-2. 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_gaussian_fit_em.h - GLDS Fit (EM) ------*- C++ -*-===//
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

#ifndef LDSCTRLEST_LDS_GAUSSIAN_FIT_EM_H
#define LDSCTRLEST_LDS_GAUSSIAN_FIT_EM_H

#include "lds_fit_em.h"
#include "lds_gaussian_fit.h"

namespace lds {
namespace gaussian {
class FitEM : public EM<Fit> {
 public:
  using EM<Fit>::EM;

 private:
  void MaximizeOutput() override;

  void MaximizeMeasurement() override;

  void RecurseKe(Matrix& Ke, Cube& P_pre, Cube& P_post, size_t t) override;
};

}  // namespace gaussian
}  // namespace lds

#endif
```


-------------------------------

Updated on  5 March 2025 at 21:41:27 EST
