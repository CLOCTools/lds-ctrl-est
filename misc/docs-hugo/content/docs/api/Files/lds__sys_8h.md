---
title: ldsCtrlEst_h/lds_sys.h
summary: LDS base type. 

---

# ldsCtrlEst_h/lds_sys.h

LDS base type.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/lds-ctrl-est/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/)** <br>Linear Dynamical [System]() Type.  |

## Detailed Description



This file declares and partially defines the base type for linear dynamical systems (`[lds::System](/lds-ctrl-est/docs/api/classes/classlds_1_1_system/)`). Note that this class defines the underlying linear dynamics, but does not have output functions.Gaussian- and Poisson-output variants will be built upon this class. 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_sys.h - LDS ----------------------------*- C++ -*-===//
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
// Limitations under the License.
//
//===----------------------------------------------------------------------===//
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_SYS_H
#define LDSCTRLEST_LDS_SYS_H

#include "lds.h"

namespace lds {
class System {
 public:
  System() = default;

  System(size_t n_u, size_t n_x, size_t n_y, data_t dt, data_t p0 = kDefaultP0,
         data_t q0 = kDefaultQ0);

  void Filter(const Vector& u_tm1, const Vector& z);

  virtual const Vector& Simulate(const Vector& u_tm1) = 0;

  void f(const Vector& u, bool do_add_noise = false) {
    x_ = A_ * x_ + B_ * (g_ % u) + m_;
    if (do_add_noise) {
      x_ += arma::mvnrnd(Vector(n_x_).fill(0), Q_);
    }
  };

  virtual void h() = 0;

  size_t n_u() const { return n_u_; };
  size_t n_x() const { return n_x_; };
  size_t n_y() const { return n_y_; };
  data_t dt() const { return dt_; };

  const Vector& x() const { return x_; };
  const Matrix& P() const { return P_; };
  const Vector& m() const { return m_; };
  const Matrix& P_m() const { return P_m_; };
  const Vector& cx() const { return cx_; };
  const Vector& y() const { return y_; };

  const Vector& x0() const { return x0_; };
  const Vector& m0() const { return m0_; };

  const Matrix& A() const { return A_; };
  const Matrix& B() const { return B_; };
  const Vector& g() const { return g_; };
  const Matrix& C() const { return C_; };
  const Vector& d() const { return d_; };
  const Matrix& Ke() const { return Ke_; };
  const Matrix& Ke_m() const { return Ke_m_; };
  void set_A(const Matrix& A) { Reassign(A_, A); };
  void set_B(const Matrix& B) { Reassign(B_, B); };
  void set_m(const Vector& m) {
    Reassign(m0_, m);
    if (!do_adapt_m) {
      Reassign(m_, m);
    }
  };
  void set_g(const Vector& g) { Reassign(g_, g); };
  void set_Q(const Matrix& Q) { Reassign(Q_, Q); };
  void set_Q_m(const Matrix& Q_m) { Reassign(Q_m_, Q_m); };
  void set_x0(const Vector& x0) { Reassign(x0_, x0); };
  void set_P0(const Matrix& P0) { Reassign(P0_, P0); };
  void set_P0_m(const Matrix& P0_m) { Reassign(P0_m_, P0_m); };
  void set_C(const Matrix& C) { Reassign(C_, C); };
  void set_d(const Vector& d) { Reassign(d_, d); };

  void Reset();

  void Print();

  // safe to leave this public and non-const
  bool do_adapt_m{};  

 protected:
  virtual void RecurseKe() = 0;
  void InitVars(data_t p0 = kDefaultP0, data_t q0 = kDefaultQ0);

  std::size_t n_x_{};  
  std::size_t n_u_{};  
  std::size_t n_y_{};  
  data_t dt_{};        

  // Signals:
  Vector x_;    
  Matrix P_;    
  Vector m_;    
  Matrix P_m_;  
  Vector cx_;   
  Vector y_;    
  Vector z_;    

  // Parameters:
  Vector x0_;    
  Matrix P0_;    
  Vector m0_;    
  Matrix P0_m_;  
  Matrix A_;     
  Matrix B_;     
  Vector g_;     
  Matrix Q_;     
  Matrix Q_m_;   
  Matrix C_;     
  Vector d_;     

  Matrix Ke_;    
  Matrix Ke_m_;  
};               // System

}  // namespace lds

#endif
```


-------------------------------

Updated on 24 March 2021 at 13:26:05 EDT
