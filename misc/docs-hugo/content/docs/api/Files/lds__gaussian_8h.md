---
title: ldsCtrlEst_h/lds_gaussian.h
summary: glds namespace 

---

# ldsCtrlEst_h/lds_gaussian.h

`glds` namespace  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/ldsctrlest/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |
| **[lds::gaussian](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/)** <br>Linear Dynamical Systems with Gaussian observations.  |

## Detailed Description



This file declares and partially defines the namespace for linear dynamical systems with Gaussian observations (`[lds::gaussian](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/)`). 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_gaussian.h - LDS with Gaussian Output --*- C++ -*-===//
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

#ifndef LDSCTRLEST_LDS_GAUSSIAN_H
#define LDSCTRLEST_LDS_GAUSSIAN_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace gaussian {
typedef lds::data_t data_t;
typedef lds::stdVec stdVec;
typedef lds::armaVec armaVec;
typedef lds::armaMat armaMat;
typedef lds::armaSubVec armaSubVec;
typedef lds::armaSubMat armaSubMat;

namespace fill = lds::fill;

// CONTROL BIT MASKS
static const std::size_t CONTROL_TYPE_U = lds::CONTROL_TYPE_U;
static const std::size_t CONTROL_TYPE_INTY = lds::CONTROL_TYPE_INTY;
static const std::size_t CONTROL_TYPE_ADAPT_M = lds::CONTROL_TYPE_ADAPT_M;

// default values
static data_t DATA_T_ZERO = lds::DATA_T_ZERO;
static data_t DATA_T_ONE = lds::DATA_T_ZERO;
static data_t DEFAULT_SOFTSTART = lds::DEFAULT_SOFTSTART;
static bool FALSE = lds::FALSE;
static bool TRUE = lds::TRUE;

// static const data_t inf = lds::inf;
static const data_t pi = lds::pi;
static data_t inf = lds::inf;
static data_t neginf = lds::neginf;
}  // namespace gaussian
}  // namespace lds

#include "lds_gaussian_sys.h"
#ifdef LDSCTRLEST_BUILD_FIT
#include "lds_gaussian_fit.h"
#endif

#endif
```


-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
