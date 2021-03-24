---
title: src/lds_sys.cpp
summary: LDS base type. 

---

# src/lds_sys.cpp

LDS base type.  [More...](#detailed-description)



## Detailed Description



This file implements the base type for linear dynamical systems ([lds::System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/)). Note that this class defines the underlying linear dynamics, but does not have output functions.Gaussian- and Poisson-output variants will be built upon this class. 





## Source code

```cpp
//===-- lds_sys.cpp - LDS -------------------------------------------------===//
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

#include <ldsCtrlEst>

lds::System::System(size_t n_u, size_t n_x, size_t n_y, data_t dt, data_t p0,
                    data_t q0)
    : n_u_(n_u), n_x_(n_x), n_y_(n_y), dt_(dt) {
  InitVars(p0, q0);
}

void lds::System::InitVars(data_t p0, data_t q0) {
  // initial conditions.
  x0_ = Vector(n_x_, fill::zeros);  // includes bias (nY) and g (nU)
  P0_ = p0 * Matrix(n_x_, n_x_, fill::eye);

  m0_ = x0_;
  P0_m_ = P0_;

  // signals
  x_ = x0_;
  P_ = P0_;
  m_ = m0_;
  P_m_ = P0_m_;
  y_ = Vector(n_y_, fill::zeros);
  cx_ = Vector(n_y_, fill::zeros);
  z_ = Vector(n_y_, fill::zeros);

  // By default, random walk where each state is independent
  // In this way, provides independent estimates of rate per channel of output.
  A_ = Matrix(n_x_, n_x_, fill::eye);
  B_ = Matrix(n_x_, n_u_, fill::zeros);
  g_ = Vector(n_u_, fill::ones);
  Q_ = q0 * Matrix(n_x_, n_x_, fill::eye);
  Q_m_ = Q_;

  C_ = Matrix(n_y_, n_x_, fill::eye);  // each state will map to an output by
  d_ = Vector(n_y_, fill::zeros);

  Ke_ = Matrix(n_x_, n_y_, fill::zeros);    // estimator gain.
  Ke_m_ = Matrix(n_x_, n_y_, fill::zeros);  // estimator gain for m adaptation.

  do_adapt_m = false;
}

// Filter: Given measurement (`z`) and previous input (`u_tm1`), predict state
// and update estimate of the state, covar, output using Kalman filter
void lds::System::Filter(const Vector& u_tm1, const Vector& z_t) {
  // predict mean
  f(u_tm1);  // dynamics

  h();  // output

  // recursively calculate esimator gains (or just keep existing values)
  // (also predicts+updates estimate covariance)
  RecurseKe();

  // update
  x_ += Ke_ * (z_t - y_);
  if (do_adapt_m) {
    m_ += Ke_m_ * (z_t - y_);  // adaptively estimating disturbance
  }

  // With new state, estimate output.
  h();  // --> posterior
}

void lds::System::Reset() {
  // reset to initial conditions
  x_ = x0_;      // mean
  P_ = P0_;      // cov of state estimate
  m_ = m0_;      // process disturbance
  P_m_ = P0_m_;  // cov of disturbance estimate
  h();
}

void lds::System::Print() {
  std::cout << "\n ********** SYSTEM ********** \n";
  std::cout << "x: \n" << x_ << "\n";
  std::cout << "P: \n" << P_ << "\n";
  std::cout << "A: \n" << A_ << "\n";
  std::cout << "B: \n" << B_ << "\n";
  std::cout << "g: \n" << g_ << "\n";
  std::cout << "m: \n" << m_ << "\n";
  std::cout << "Q: \n" << Q_ << "\n";
  std::cout << "Q_m: \n" << Q_m_ << "\n";
  std::cout << "d: \n" << d_ << "\n";
  std::cout << "C: \n" << C_ << "\n";
  std::cout << "y: \n" << y_ << "\n";
}

//******************* SYS_T *******************
```


-------------------------------

Updated on 24 March 2021 at 13:11:19 EDT
