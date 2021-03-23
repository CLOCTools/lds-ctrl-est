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
| class | **[lds::gaussian::System](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1_system/)** <br>Gaussian LDS Type.  |

## Detailed Description



This file declares and partially defines the type for state estimation (filtering) as well as simulation of Gaussian-output linear dynamical systems (`[lds::gaussian::System](/ldsctrlest/docs/api/classes/classlds_1_1gaussian_1_1_system/)`). It inherits functionality from the underlying linear dynamical system (`[lds::System](/ldsctrlest/docs/api/classes/classlds_1_1_system/)`). 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_gaussian_sys.h - GLDS ------------------*- C++ -*-===//
//
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

#ifndef LDSCTRLEST_LDS_GAUSSIAN_SYS_H
#define LDSCTRLEST_LDS_GAUSSIAN_SYS_H

// namespace
#include "lds_gaussian.h"
// system
#include "lds_sys.h"

namespace lds {
namespace gaussian {
class System : public lds::System {
 public:
  System() = default;

  System(std::size_t n_u, std::size_t n_x, std::size_t n_y, data_t dt,
         data_t p0 = kDefaultP0, data_t q0 = kDefaultQ0,
         data_t r0 = kDefaultR0);

  const Vector& Simulate(const Vector& u_tm1) override;

  // get methods
  const Matrix& R() const { return R_; };

  // set methods
  void set_Q(const Matrix& Q) {
    lds::System::set_Q(Q);
    do_recurse_Ke_ = true;
  }
  void set_R(const Matrix& R) {
    Reassign(R_,R);
    do_recurse_Ke_ = true;
  };

  void set_Ke(const Matrix& Ke) {
    Reassign(Ke_,Ke);
    // if users have set Ke, they must not want to calculate it online.
    do_recurse_Ke_ = false;
  };
  void set_Ke_m(const Matrix& Ke_m) {
    Reassign(Ke_m_,Ke_m);
    // if users have set Ke, they must not want to calculate it online.
    do_recurse_Ke_ = false;
  };

  void Print();

 protected:
  void h() override {
    cx_ = C_ * x_;
    y_ = cx_ + d_;
  };

  void RecurseKe() override;

  // Gaussian-output-specific
  Matrix R_;           
  bool do_recurse_Ke_{};  
};                      // System
}  // namespace gaussian
}  // namespace lds

#endif
```


-------------------------------

Updated on 23 March 2021 at 09:14:15 CDT
