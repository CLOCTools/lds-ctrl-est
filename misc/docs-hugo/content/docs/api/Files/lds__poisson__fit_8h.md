---
title: ldsCtrlEst_h/lds_poisson_fit.h
summary: PLDS base fit type. 

---

# ldsCtrlEst_h/lds_poisson_fit.h

PLDS base fit type.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/lds-ctrl-est/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |
| **[lds::poisson](/lds-ctrl-est/docs/api/namespaces/namespacelds_1_1poisson/)** <br>Linear Dynamical Systems with Poisson observations.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::poisson::Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_fit/)** <br>PLDS [Fit]() Type.  |

## Detailed Description



This file declares and partially defines the base fit type for a Gaussian-output linear dynamical system. Models are fit by either subspace identification (SSID) or expectation-maximization (EM). 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_poisson_fit.h - Fit Type for PLDS ------*- C++ -*-===//
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

#ifndef LDSCTRLEST_LDS_POISSON_FIT_H
#define LDSCTRLEST_LDS_POISSON_FIT_H

// namespace
#include "lds_poisson.h"
// fit
#include "lds_fit.h"

namespace lds {
namespace poisson {
class Fit : public lds::Fit {
 public:
  Fit() = default;

  Fit(size_t n_u, size_t n_x, size_t n_y, data_t dt)
      : lds::Fit(n_u, n_x, n_y, dt){};

  View h(Matrix& y, const Matrix& x, size_t t) override {
    y.col(t) = exp(C_ * x.col(t) + d_);
    return y.col(t);
  };

  void set_R(const Matrix& R) override {
    std::cerr
        << "WARNING: Cannot set R (R[0] = " << R.at(0)
        << "). No Gaussian measurement noise in Poisson observation model.\n";
  };

  const Matrix& R() const override {
    return R_;
  };

};

};  // namespace poisson
}  // namespace lds

#endif
```


-------------------------------

Updated on 24 March 2021 at 13:26:05 EDT
