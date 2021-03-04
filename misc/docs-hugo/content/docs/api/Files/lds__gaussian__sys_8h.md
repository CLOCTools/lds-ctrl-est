---
title: ldsCtrlEst_h/lds_gaussian_sys.h
summary: GLDS base type. 

---

# ldsCtrlEst_h/lds_gaussian_sys.h

GLDS base type.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/ldsctrlest/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |
| **[lds::gaussian](/ldsctrlest/docs/api/namespaces/namespacelds_1_1gaussian/)** <br>Linear Dynamical Systems with Gaussian observations.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::gaussian::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/)** <br>Gaussian LDS Type.  |

## Detailed Description



This file declares and partially defines the type for state estimation (filtering) as well as simulation of Gaussian-output linear dynamical systems (`[lds::gaussian::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1sys__t/)`). It inherits functionality from the underlying linear dynamical system (`[lds::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/)`). 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_gaussian_sys.h - GLDS ------------------*- C++ -*-===//
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

#ifndef LDSCTRLEST_LDS_GAUSSIAN_SYS_H
#define LDSCTRLEST_LDS_GAUSSIAN_SYS_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace gaussian {
class sys_t : public lds::sys_t {
 public:
  void filter(armaVec& u_tm1, armaVec& z_t, bool doRecurse_Ke = true);

  void filter(armaVec& z, bool doRecurse_Ke = true);

  void simMeasurement(armaVec& z);

  sys_t(std::size_t nU, std::size_t nX, std::size_t nY, data_t& dt,
        data_t& p0 = DEFAULT_P0, data_t& q0 = DEFAULT_Q0,
        data_t& r0 = DEFAULT_R0);
  sys_t& operator=(const sys_t& sys);

  // get methods
  size_t getNy() const { return nY; };

  armaMat getC() const { return C; };

  armaVec getD() const { return d; };

  armaVec getR() const { return R; };

  armaVec getY() const { return y; };

  armaVec getZ() const { return z; };

  armaMat getKe() const { return Ke; };
  armaMat getKe_m() const { return Ke_m; };

  // set methods
  void setDims(std::size_t& nU, std::size_t& nX, std::size_t& nY);

  void setC(stdVec& cVec);
  void setC(armaMat& C);

  void setD(stdVec& dVec);
  void setD(armaVec& d);

  void setR(stdVec& rVec);
  void setR(armaMat& R);

  void setZ(stdVec& zVec);
  void setZ(armaVec& z);

  void setKe(stdVec& keVec);
  void setKe(armaMat& Ke);

  void setKe_m(stdVec& kemVec);
  void setKe_m(armaMat& Ke_m);

  void reset();

  void printSys();

 protected:
  void recurse_Ke();

  void predict();

  void h();

  void defaultR();

  // output-specific stuff
  std::size_t nY;  
  armaMat C;       
  armaVec d;       
  armaMat R;       
  data_t& r0;      
  armaVec y;       
  armaVec z;       

  armaMat Ke;    
  armaMat Ke_m;  
};               // sys_t
}  // namespace gaussian
}  // namespace lds

#include "lds_gaussian_ctrl.h"

#endif
```


-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
