---
title: ldsCtrlEst_h/lds_poisson.h
summary: plds namespace 

---

# ldsCtrlEst_h/lds_poisson.h

`plds` namespace  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/lds-ctrl-est/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |
| **[lds::poisson](/lds-ctrl-est/docs/api/namespaces/namespacelds_1_1poisson/)** <br>Linear Dynamical Systems with Poisson observations.  |

## Detailed Description



This file declares and partially defines the namespace for linear dynamical systems with Poisson observations (`[lds::poisson](/lds-ctrl-est/docs/api/namespaces/namespacelds_1_1poisson/)`). 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_poisson.h - LDS with Poisson Output ----*- C++ -*-===//
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

#ifndef LDSCTRLEST_LDS_POISSON_H
#define LDSCTRLEST_LDS_POISSON_H

#include "lds.h"

namespace lds {
namespace poisson {
// TODO(mfbolus): Not sure if defining these as static here makes the most
// sense. Is there a downside to letting multiple poisson System objects share a
// common random number generator?
static std::random_device rd;  
static std::mt19937 rng = std::mt19937(
    rd());  
}  // namespace poisson
}  // namespace lds

#endif
```


-------------------------------

Updated on  4 May 2022 at 16:59:51 Eastern Daylight Time
