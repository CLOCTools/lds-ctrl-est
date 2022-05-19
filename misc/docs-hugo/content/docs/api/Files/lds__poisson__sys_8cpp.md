---
title: src/lds_poisson_sys.cpp
summary: PLDS base type. 

---

# src/lds_poisson_sys.cpp

PLDS base type.  [More...](#detailed-description)



## Detailed Description



This file implements the type for state estimation (filtering) as well as simulation of Poisson-output linear dynamical systems (`lds::poisson::sys_t`). It inherits functionality from the underlying linear dynamical system (`lds::sys_t`). 





## Source code

```cpp
//===-- lds_poisson_sys.cpp - PLDS ----------------------------------------===//
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

#include <ldsCtrlEst_h/lds_poisson_sys.h>

lds::poisson::System::System(size_t n_u, size_t n_x, size_t n_y, data_t dt,
                             data_t p0, data_t q0)
    : lds::System(n_u, n_x, n_y, dt, p0, q0) {
  diag_y_ = diagmat(y_);
  pd_ = std::poisson_distribution<size_t>(0);
};

// Correct: Given measurement (z) and current input (u), update estimate of the
// state, covar, output.
//
// see Eden et al. 2004
void lds::poisson::System::RecurseKe() {
  // predict covariance
  P_ = A_ * P_ * A_.t() + Q_;

  // update cov
  P_ = pinv(pinv(P_) + C_.t() * diag_y_ * C_);
  Ke_ = P_ * C_.t();
  if (do_adapt_m) {
    P_m_ += Q_m_;                                     // predict (A_m = I)
    P_m_ = pinv(pinv(P_m_) + C_.t() * diag_y_ * C_);  // update
    Ke_m_ = P_m_ * C_.t();
  }
}

// Simulate Measurement: z ~ Poisson(y)
const lds::Vector& lds::poisson::System::Simulate(const Vector& u_tm1) {
  f(u_tm1, true);  // simulate dynamics with noise added
  h();             // output

  z_.zeros();
  for (std::size_t k = 0; k < n_y_; k++) {
    // construct a Poisson distribution object with mean y[k]
    pd_ = std::poisson_distribution<size_t>(y_[k]);
    // pull random sample from this distribution
    z_[k] = pd_(rng);
  }

  return z_;
}
// ******************* SYS_T *******************
```


-------------------------------

Updated on 19 May 2022 at 17:16:05 Eastern Daylight Time
