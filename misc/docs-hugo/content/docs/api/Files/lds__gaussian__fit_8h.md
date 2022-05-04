---
title: ldsCtrlEst_h/lds_gaussian_fit.h
summary: GLDS fit type. 

---

# ldsCtrlEst_h/lds_gaussian_fit.h

GLDS fit type.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/lds-ctrl-est/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |
| **[lds::gaussian](/lds-ctrl-est/docs/api/namespaces/namespacelds_1_1gaussian/)** <br>Linear Dynamical Systems with Gaussian observations.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::gaussian::Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1gaussian_1_1fit/)** <br>GLDS [Fit]() Type.  |

## Detailed Description



This file declares and partially defines the base fit type for a Gaussian-output linear dynamical system. Models are fit by either subspace identification (SSID) or expectation-maximization (EM). 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_gaussian_fit.h - Fit Type for GLDS -----*- C++ -*-===//
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

#ifndef LDSCTRLEST_LDS_GAUSSIAN_FIT_H
#define LDSCTRLEST_LDS_GAUSSIAN_FIT_H

// namespace
#include "lds_gaussian.h"
// fit type
#include "lds_fit.h"

namespace lds {
namespace gaussian {
class Fit : public lds::Fit {
 public:
  Fit() = default;

  Fit(size_t n_u, size_t n_x, size_t n_y, data_t dt);
  const Matrix& R() const override { return R_; };
  void set_R(const Matrix& R) override {
    Reassign(R_, R);
    ForceSymPD(R_);
  };

  View h(Matrix& y, const Matrix& x, size_t t) override {
    y.col(t) = C_ * x.col(t) + d_;
    return y.col(t);
  };
};

};  // namespace gaussian
}  // namespace lds
#endif
```


-------------------------------

Updated on  4 May 2022 at 16:59:51 Eastern Daylight Time
