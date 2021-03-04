---
title: ldsCtrlEst_h/lds_gaussian_fit.h
summary: GLDS fit type. 

---

# ldsCtrlEst_h/lds_gaussian_fit.h

GLDS fit type.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/ldsctrlest/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |
| **[lds::gaussian](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/)** <br>Linear Dynamical Systems with Gaussian observations.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::gaussian::fit_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1fit__t/)** <br>GLDS Fit Type.  |

## Detailed Description



This file declares and partially defines the base fit type for a Gaussian-output linear dynamical system. Models are fit by either subspace identification (SSID) or expectation-maximization (EM). 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_gaussian_fit.h - Fit Type for GLDS -----*- C++ -*-===//
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

#ifndef LDSCTRLEST_LDS_GAUSSIAN_FIT_H
#define LDSCTRLEST_LDS_GAUSSIAN_FIT_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace gaussian {
class fit_t : public lds::fit_t {
 public:
  fit_t(){};
  fit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q, armaVec& x0,
        armaMat& P0, armaMat& C, armaMat& D, armaVec& d, armaMat& R, data_t dt,
        std::vector<armaMat>& uTrain, std::vector<armaMat>& zTrain);

  fit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q, armaVec& x0,
        armaMat& P0, armaMat& C, armaVec& d, armaMat& R, data_t dt,
        std::vector<armaMat>& uTrain, std::vector<armaMat>& zTrain);

  // Output
  armaMat C;  
  armaMat D;  
  armaVec d;  
  armaMat R;  

  // input/output training data
  std::vector<armaMat> uTrain;  
  std::vector<armaMat> zTrain;  
};

};  // namespace gaussian
}  // namespace lds

// subspace identification
#include "lds_gaussian_fit_ssid.h"

// expectation maximization
#include "lds_gaussian_fit_em.h"

#endif
```


-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
