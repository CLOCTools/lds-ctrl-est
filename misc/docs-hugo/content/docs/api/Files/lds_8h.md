---
title: ldsCtrlEst_h/lds.h
summary: lds namespace 

---

# ldsCtrlEst_h/lds.h

`lds` namespace  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/ldsctrlest/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |

## Detailed Description



This file defines the `lds` namespace, which will be an umbrella for linear dynamical systems with Gaussian (`[lds::gaussian](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/)`) or Poisson (`[lds::poisson](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/)`) observations. 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds.h - Linear Dynmical System Namespace ---*- C++ -*-===//
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

#ifndef LDSCTRLEST_LDS_H
#define LDSCTRLEST_LDS_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
typedef double data_t;

namespace fill = arma::fill;

typedef std::vector<data_t> stdVec;
typedef arma::Col<data_t> armaVec;
typedef arma::Mat<data_t> armaMat;
typedef arma::Cube<data_t> armaCube;
typedef arma::subview_col<data_t> armaSubVec;
typedef arma::subview<data_t> armaSubMat;


static const std::size_t CONTROL_TYPE_U = 0x1;

static const std::size_t CONTROL_TYPE_INTY = CONTROL_TYPE_U << 1;

static const std::size_t CONTROL_TYPE_ADAPT_M = CONTROL_TYPE_U << 2;

static data_t inf = std::numeric_limits<data_t>::infinity();
static data_t neginf = -inf;
static const data_t pi = arma::datum::pi;

static data_t DEFAULT_P0 = 1e-6;
static data_t DEFAULT_Q0 = 1e-6;
static data_t DEFAULT_R0 = 1e-2;
static std::vector<data_t> DEFAULT_T0 = std::vector<data_t>(1, 0.0);

// Ideally these should be const, but trying to pass reference which means its
// val implicitly must be allowed to change. hopefully no one will try to
// reassign their vals...
static data_t DATA_T_ZERO = (data_t)0;
static data_t DATA_T_ONE = (data_t)1;
static data_t DEFAULT_SOFTSTART = (data_t)0;
static bool FALSE = false;
static bool TRUE = true;

enum ssidWt { NONE, MOESP, CVA };
}  // namespace lds

// fit and sys with dynamics only
#include "lds_sys.h"
#ifdef LDSCTRLEST_BUILD_FIT
#include "lds_fit.h"
#endif

// Pull in output equation variants
#include "lds_gaussian.h"
#include "lds_poisson.h"

#endif
```


-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
