---
title: ldsCtrlEst_h/lds.h
summary: lds namespace 

---

# ldsCtrlEst_h/lds.h

`lds` namespace  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/lds-ctrl-est/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |

## Detailed Description



This file defines the `lds` namespace, which will be an umbrella for linear dynamical systems with Gaussian (`[lds::gaussian](/lds-ctrl-est/docs/api/namespaces/namespacelds_1_1gaussian/)`) or Poisson (`[lds::poisson](/lds-ctrl-est/docs/api/namespaces/namespacelds_1_1poisson/)`) observations. 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds.h - Linear Dynmical System Namespace ---*- C++ -*-===//
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

#ifndef LDSCTRLEST_LDS_H
#define LDSCTRLEST_LDS_H

// #ifndef LDSCTRLEST
// #include <ldsCtrlEst>
// #endif

#include <armadillo>

namespace lds {
using data_t = double;  // may change to float (but breaks mex functions)
using Vector = arma::Col<data_t>;
using Matrix = arma::Mat<data_t>;
using Cube = arma::Cube<data_t>;
using View = arma::subview<data_t>;

namespace fill = arma::fill;


static const std::size_t kControlTypeDeltaU = 0x1;

static const std::size_t kControlTypeIntY = kControlTypeDeltaU << 1;

static const std::size_t kControlTypeAdaptM = kControlTypeDeltaU << 2;

static const data_t kInf = std::numeric_limits<data_t>::infinity();
static const data_t kPi = arma::datum::pi;

static const data_t kDefaultP0 = 1e-6;  
static const data_t kDefaultQ0 = 1e-6;  
static const data_t kDefaultR0 = 1e-2;  

enum SSIDWt {
  kSSIDNone,   
  kSSIDMOESP,  
  kSSIDCVA     
};

enum MatrixListFreeDim {
  kMatFreeDimNone,  
  kMatFreeDim1,     
  kMatFreeDim2      
};

// TODO(mfbolus): for SwitchedController, may want systems to have differing
// numbers of states. Use this enum as template parameter?
// enum SystemListFreeDim {
//   kSysFreeDimNone,
//   kSysFreeDimX  ///< allow state dim (x) of systems in list to be hetero
// };

// place hard limits on contents of vecors/mats
void Limit(std::vector<data_t>& x, data_t lb, data_t ub);
void Limit(Vector& x, data_t lb, data_t ub);
void Limit(Matrix& x, data_t lb, data_t ub);

// in-place assign that errs if there are dimension mismatches:
void Reassign(Vector& some, const Vector& other,
              const std::string& parenthetical = "Reassign");
void Reassign(Matrix& some, const Matrix& other,
              const std::string& parenthetical = "Reassign");

// TODO(mfbolus): this is a fudge, but for some reason, cov mats often going
// numerically asymm.

void ForceSymPD(Matrix& X);

void ForceSymMinEig(Matrix& X, data_t eig_min = 0);

void lq(Matrix& L, Matrix& Qt, const Matrix& X);

Matrix calcCov(const Matrix& A, const Matrix& B);

inline void Limit(std::vector<data_t>& x, data_t lb, data_t ub) {
  for (data_t& el : x) {
    el = el < lb ? lb : el;
    el = el > ub ? ub : el;
  }
}
inline void Limit(Vector& x, data_t lb, data_t ub) {
  for (data_t& el : x) {
    el = el < lb ? lb : el;
    el = el > ub ? ub : el;
  }
}
inline void Limit(Matrix& x, data_t lb, data_t ub) {
  for (data_t& el : x) {
    el = el < lb ? lb : el;
    el = el > ub ? ub : el;
  }
}

inline void Reassign(Vector& some, const Vector& other,
                     const std::string& parenthetical) {
  // check dimensions
  if (other.n_elem != some.n_elem) {
    std::ostringstream ss;
    ss << "cannot reassign vector of size " << some.n_elem
       << " with vector of size " << other.n_elem << "(" << parenthetical
       << ")";
    throw std::runtime_error(ss.str());
  }

  for (size_t k = 0; k < some.n_elem; k++) {
    some[k] = other[k];
  }
}

inline void Reassign(Matrix& some, const Matrix& other,
                     const std::string& parenthetical) {
  // check dimensions
  if ((other.n_rows != some.n_rows) || (other.n_cols != some.n_cols)) {
    std::ostringstream ss;
    ss << "cannot reassign matrix of size " << some.n_rows << "x" << some.n_cols
       << " with matrix of size " << other.n_rows << "x" << other.n_cols << "("
       << parenthetical << ")";
    throw std::runtime_error(ss.str());
  }

  for (size_t k = 0; k < some.n_elem; k++) {
    some[k] = other[k];
  }
}

}  // namespace lds

#endif
```


-------------------------------

Updated on  4 May 2022 at 16:34:52 Eastern Daylight Time
