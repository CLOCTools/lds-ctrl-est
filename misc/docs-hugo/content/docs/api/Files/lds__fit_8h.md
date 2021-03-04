---
title: ldsCtrlEst_h/lds_fit.h
summary: LDS base fit type. 

---

# ldsCtrlEst_h/lds_fit.h

LDS base fit type.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/ldsctrlest/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::fit_t](/ldsctrlest/docs/api/classes/classlds_1_1fit__t/)** <br>LDS Fit Type.  |

## Detailed Description



This file declares and partially defines the base fit type for a linear dynamical system. It is expounded upon by variants with Gaussian and Poisson observation assumptions for fitting. 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_fit.h - Fit Type for LDS ---------------*- C++ -*-===//
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

#ifndef LDS_FIT_HPP
#define LDS_FIT_HPP

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
class fit_t {
 public:
  fit_t(){};
  fit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q, armaVec& x0,
        armaMat& P0, data_t& dt);

  // Dynamics
  armaMat A;   
  armaMat B;   
  armaVec g;   
  armaVec m;   
  armaMat Q;   
  armaVec x0;  
  armaMat P0;  

  data_t dt;  
};

}  // namespace lds
#endif
```


-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
