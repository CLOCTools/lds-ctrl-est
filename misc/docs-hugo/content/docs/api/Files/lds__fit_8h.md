---
title: ldsCtrlEst_h/lds_fit.h
summary: LDS base fit type. 

---

# ldsCtrlEst_h/lds_fit.h

LDS base fit type.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/lds-ctrl-est/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::Fit](/lds-ctrl-est/docs/api/classes/classlds_1_1_fit/)** <br>LDS [Fit]() Type.  |

## Detailed Description



This file declares and partially defines the base fit type for a linear dynamical system. It is expounded upon by variants with Gaussian and Poisson observation assumptions for fitting. 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_fit.h - Fit Type for LDS ---------------*- C++ -*-===//
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

#ifndef LDS_FIT_HPP
#define LDS_FIT_HPP

// namespace
#include "lds.h"
#include "lds_uniform_mats.h"

namespace lds {
class Fit {
 public:
  Fit() = default;
  Fit(size_t n_u, size_t n_x, size_t n_y, data_t dt);

  virtual ~Fit() = default;

  // get methods
  size_t n_u() const { return n_u_; };
  size_t n_x() const { return n_x_; };
  size_t n_y() const { return n_y_; };
  data_t dt() const { return dt_; };
  const Matrix& A() const { return A_; };
  const Matrix& B() const { return B_; };
  const Vector& g() const { return g_; };
  const Vector& m() const { return m_; };
  const Matrix& Q() const { return Q_; };
  const Vector& x0() const { return x0_; };
  const Matrix& P0() const { return P0_; };
  const Matrix& C() const { return C_; };
  const Vector& d() const { return d_; };
  // gets measurement noise
  virtual const Matrix& R() const = 0;

  // set methods (e.g., seeding initial fit values)
  void set_A(const Matrix& A) { Reassign(A_, A); };
  void set_B(const Matrix& B) { Reassign(B_, B); };
  void set_g(const Vector& g) { Reassign(g_, g); };
  void set_m(const Vector& m) { Reassign(m_, m); };
  void set_Q(const Matrix& Q) {
    Reassign(Q_, Q);
    ForceSymPD(Q_);
  };
  virtual void set_R(const Matrix& R) = 0;
  void set_x0(const Vector& x0) { Reassign(x0_, x0); };
  void set_P0(const Matrix& P0) {
    Reassign(P0_, P0);
    ForceSymPD(P0_);
  };
  void set_C(const Matrix& C) { Reassign(C_, C); };
  void set_d(const Vector& d) { Reassign(d_, d); };

  View f(Matrix& x, const Matrix& u, size_t t) {
    x.col(t) = A_ * x.col(t - 1) + B_ * (g_ % u.col(t - 1)) + m_;
    return x.col(t);
  };

  View f(Matrix& x_pre, const Matrix& x_post, const Matrix& u, size_t t) {
    x_pre.col(t) = A_ * x_post.col(t - 1) + B_ * (g_ % u.col(t - 1)) + m_;
    return x_pre.col(t);
  };

  virtual View h(Matrix& y, const Matrix& x, size_t t) = 0;

 protected:
  data_t dt_{};  

  // Dynamics
  Matrix A_;  
  Matrix B_;  
  Vector g_;  
  Vector m_;  
  Matrix Q_;  

  // Output
  Matrix C_;  
  Vector d_;  
  Matrix R_; 

  // initial conditions
  Vector x0_;  
  Matrix P0_;  

  size_t n_u_{};  
  size_t n_x_{};  
  size_t n_y_{};  
};

}  // namespace lds
#endif
```


-------------------------------

Updated on  5 March 2025 at 21:41:27 EST
