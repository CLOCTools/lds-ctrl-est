---
title: ldsCtrlEst_h/lds_poisson_sys.h
summary: PLDS base type. 

---

# ldsCtrlEst_h/lds_poisson_sys.h

PLDS base type.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/lds-ctrl-est/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |
| **[lds::poisson](/lds-ctrl-est/docs/api/namespaces/namespacelds_1_1poisson/)** <br>Linear Dynamical Systems with Poisson observations.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::poisson::System](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_system/)** <br>Poisson [System]() type.  |

## Detailed Description



This file declares and partially defines the type for state estimation (filtering) as well as simulation of Poisson-output linear dynamical systems (`[lds::poisson::System](/lds-ctrl-est/docs/api/classes/classlds_1_1poisson_1_1_system/)`). It inherits functionality from the underlying linear dynamical system (`[lds::System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/)`). 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_poisson_sys.h - PLDS -------------------*- C++ -*-===//
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

#ifndef LDSCTRLEST_LDS_POISSON_SYS_H
#define LDSCTRLEST_LDS_POISSON_SYS_H

// namespace
#include "lds_poisson.h"
// system
#include "lds_sys.h"

// needed for Poisson random number generation
#include <random>

namespace lds {
namespace poisson {

class System : public lds::System {
 public:
  System() = default;

  System(std::size_t n_u, std::size_t n_x, std::size_t n_y, data_t dt,
         data_t p0 = kDefaultP0, data_t q0 = kDefaultQ0);

  const Vector& Simulate(const Vector& u_tm1) override;

 protected:
  void h() override {
    cx_ = C_ * x_;
    y_ = exp(cx_ + d_);
    diag_y_.diag() = y_;
  };

  Vector h_(Vector x) override { return exp(C_ * x + d_); };

  void RecurseKe() override;

 private:
  // Poisson-output-specific
  Matrix diag_y_;  
  std::poisson_distribution<size_t>
      pd_;  
};  // System
}  // namespace poisson
}  // namespace lds

#endif
```


-------------------------------

Updated on 31 March 2025 at 16:24:27 EDT
