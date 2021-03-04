---
title: ldsCtrlEst_h/lds_poisson_fit.h
summary: PLDS base fit type. 

---

# ldsCtrlEst_h/lds_poisson_fit.h

PLDS base fit type.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/ldsctrlest/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |
| **[lds::poisson](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/)** <br>Linear Dynamical Systems with Poisson observations.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::poisson::fit_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1fit__t/)** <br>PLDS Fit Type.  |

## Detailed Description



This file declares and partially defines the base fit type for a Gaussian-output linear dynamical system. Models are fit by either subspace identification (SSID) or expectation-maximization (EM). 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_poisson_fit.h - Fit Type for PLDS ------*- C++ -*-===//
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

#ifndef LDSCTRLEST_LDS_POISSON_FIT_H
#define LDSCTRLEST_LDS_POISSON_FIT_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace poisson {
class fit_t : public lds::fit_t {
 public:
  fit_t(){};
  fit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q, armaVec& x0,
        armaMat& P0, armaMat& C, armaVec& d, data_t dt,
        std::vector<armaMat>& uTrain, std::vector<armaMat>& zTrain);

  // Output
  armaMat C;  
  armaVec d;  

  // input/output training data
  std::vector<armaMat> uTrain;  
  std::vector<armaMat> zTrain;  
};

};  // namespace poisson
}  // namespace lds

#include "lds_poisson_fit_ssid.h"

#endif
```


-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
