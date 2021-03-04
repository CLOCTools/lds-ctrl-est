---
title: ldsCtrlEst_h/lds_poisson_sys.h
summary: PLDS base type. 

---

# ldsCtrlEst_h/lds_poisson_sys.h

PLDS base type.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/ldsctrlest/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |
| **[lds::poisson](/ldsctrlest/docs/api/namespaces/namespacelds_1_1poisson/)** <br>Linear Dynamical Systems with Poisson observations.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::poisson::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/)** <br>Poisson LDS Type.  |

## Detailed Description



This file declares and partially defines the type for state estimation (filtering) as well as simulation of Poisson-output linear dynamical systems (`[lds::poisson::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1poisson_1_1sys__t/)`). It inherits functionality from the underlying linear dynamical system (`[lds::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/)`). 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_poisson_sys.h - PLDS -------------------*- C++ -*-===//
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

#ifndef LDSCTRLEST_LDS_POISSON_SYS_H
#define LDSCTRLEST_LDS_POISSON_SYS_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace poisson {
class sys_t : public lds::sys_t {
 public:
  void filter(armaVec& u_tm1, armaVec& z_t);

  void filter(armaVec& z);

  void simMeasurement(armaVec& z);

  sys_t(std::size_t nU, std::size_t nX, std::size_t nY, data_t& dt,
        data_t& p0 = DEFAULT_P0, data_t& q0 = DEFAULT_Q0);
  sys_t& operator=(const sys_t& sys);

  // get methods
  size_t getNy() const { return nY; };

  armaMat getC() const { return C; };

  armaVec getD() const { return d; };

  armaVec getY() const { return y; };

  armaVec getZ() const { return z; };

  // set methods
  void setDims(std::size_t& nU, std::size_t& nX, std::size_t& nY);

  void setC(stdVec& cVec);
  void setC(armaMat& C);

  void setD(stdVec& dVec);
  void setD(armaVec& d);

  void setZ(stdVec& zVec);
  void setZ(armaVec& z);

  void reset();
  void printSys();

 protected:
  void predict();
  void h();  // output nonlinearity

  // output-specific stuff
  std::size_t nY;  
  armaMat C;       
  armaVec d;       
  armaVec y;       
  armaVec logy;    
  armaVec z;       

  armaMat diag_y;  
  armaVec chance;  
};                 // sys_t
}  // namespace poisson
}  // namespace lds

#include "lds_poisson_ctrl.h"

#endif
```


-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
