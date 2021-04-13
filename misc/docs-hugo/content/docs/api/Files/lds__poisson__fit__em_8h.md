---
title: ldsCtrlEst_h/lds_poisson_fit_em.h
summary: PLDS E-M fit type. 

---

# ldsCtrlEst_h/lds_poisson_fit_em.h

PLDS E-M fit type.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/lds-ctrl-est/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |
| **[lds::poisson](/lds-ctrl-est/docs/api/namespaces/namespacelds_1_1poisson/)** <br>Linear Dynamical Systems with Poisson observations.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::poisson::FitEM](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_fit_e_m/)** <br>PLDS E-M [Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_fit/) Type.  |

## Detailed Description



This file declares and partially defines a type by which Gaussian-output LDS models are fit by the expectation-maximization (EM) algorithm (`lds::gaussian::emFit_t`).

References: [1] Shumway RH, Stoffer DS. (1982) An Approach to Time Series Smoothing and Forecasting Using the EM Algorithm.Journal of Time Series Analysis 3(2).

[2] Ghahramani Z, Hinton GE. (1996) Parameter Estimation for Linear Dynamical Systems. Technical Report CRG-TR-96-2.

[3] Smith A, Brown E. (2003) Estimating a State-Space Model from Point Process Observations. Neural Computation. 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_poisson_fit_em.h - PLDS Fit (EM) -------*- C++ -*-===//
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

#ifndef LDSCTRLEST_LDS_POISSON_FIT_EM_H
#define LDSCTRLEST_LDS_POISSON_FIT_EM_H

#include "lds_fit_em.h"
#include "lds_poisson_fit.h"

namespace lds {
namespace poisson {
class FitEM : public EM<Fit> {
 public:
  using EM<Fit>::EM;

 private:
  void MaximizeOutput() override;

  void MaximizeMeasurement() override{};

  void RecurseKe(Matrix& Ke, Cube& P_pre, Cube& P_post, size_t t) override;

  data_t NewtonSolveC();

  void AnalyticalSolveD();
};

}  // namespace poisson
}  // namespace lds

#endif
```


-------------------------------

Updated on 30 March 2021 at 15:49:43 CDT
