---
title: ldsCtrlEst_h/lds_uniform_vecs.h
summary: List of uniformly sized vectors. 

---

# ldsCtrlEst_h/lds_uniform_vecs.h

List of uniformly sized vectors.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/lds-ctrl-est/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::UniformVectorList](/lds-ctrl-est/docs/api/classes/classlds_1_1uniformvectorlist/)**  |

## Detailed Description



This file provides a container for uniformly sized vectors. 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_uniform_vecs.h - Uniform Vectors -------*- C++ -*-===//
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
// Limitations under the License.
//
//===----------------------------------------------------------------------===//
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_UNIFORM_VECS_H
#define LDSCTRLEST_LDS_UNIFORM_VECS_H

#include <array>   // std::array
#include <vector>  // std::vector

#include "lds.h"

namespace lds {

class UniformVectorList : public std::vector<Vector> {
 private:
  // TODO(mfbolus): would rather *uncomment* the below for sake of conversion
  // using std::vector<Vector>::vector;
  using std::vector<Vector>::operator=;
  using std::vector<Vector>::operator[];
  using std::vector<Vector>::at;
  using std::vector<Vector>::begin;
  using std::vector<Vector>::end;
  using std::vector<Vector>::size;

 public:
  UniformVectorList() = default;

  explicit UniformVectorList(const std::vector<Vector>& vecs, size_t dim = 0);

  explicit UniformVectorList(std::vector<Vector>&& vecs, size_t dim = 0);

  UniformVectorList(std::initializer_list<Vector> vecs, size_t dim = 0);

  UniformVectorList(const UniformVectorList& that);
  UniformVectorList(UniformVectorList&& that) noexcept;
  ~UniformVectorList() = default;

  size_t dim() const { return dim_; }

  size_t size() { return std::vector<Vector>::size(); };

  const Vector& at(size_t n) { return std::vector<Vector>::at(n); };

  void Swap(Vector& that, size_t n);

  UniformVectorList& operator=(const UniformVectorList& that);
  UniformVectorList& operator=(UniformVectorList&& that) noexcept;

 private:
  void CheckDimensions(size_t dim);
  size_t dim_{};
};

inline void UniformVectorList::Swap(Vector& that, size_t n) {
  // make sure request in range
  if (n >= this->size()) {
    std::cerr
        << "Requested UniformMatrixList element out of bounds. Skipping.\n";
    return;
  }
  // check dim
  bool does_match = dim_ == that.n_elem;
  if (!does_match) {
    std::cerr << "Cannot swap a UniformMatrixList element for an element of "
                 "different size. Skipping.\n";
    return;
  }
  // if checks pass, perform swap
  Vector tmp = std::move((*this)[n]);
  (*this)[n] = std::move(that);
  that = std::move(tmp);
}

inline UniformVectorList& UniformVectorList::operator=(
    const UniformVectorList& that) {
  // check dimensions
  if (!this->empty()) {
    if (this->size() != that.size()) {
      std::ostringstream ss;
      ss << "cannot reassign " << this->size() << " vectors with "
         << that.size() << " vectors";
      throw std::runtime_error(ss.str());
    }

    if (dim_) {
      size_t other_dim(that.dim());
      if (dim_ != other_dim) {
        std::ostringstream ss;
        ss << "cannot reassign vectors of size " << dim_
           << " with vectors of size " << other_dim;
        throw std::runtime_error(ss.str());
      }
    }
  }

  for (size_t k = 0; k < this->size(); k++) {
    (*this)[k] = that[k];
  }

  return (*this);
}

inline UniformVectorList& UniformVectorList::operator=(
    UniformVectorList&& that) noexcept {
  // // check dimensions
  // if (!this->empty()) {
  //   if (this->size() != that.size()) {
  //     std::cerr << "Cannot reassign " << this->size() << " vectors with "
  //               << that.size() << " vectors. Skipping.\n";
  //     return (*this);
  //   }
  //
  //   if (dim_) {
  //     size_t other_dim(that.dim());
  //     if (dim_ != other_dim) {
  //       std::cerr << "Cannot reassign vectors of size " << dim_
  //                 << " with matrices of size " << other_dim << ".
  //                 Skipping.\n";
  //       return (*this);
  //     }
  //   }
  // }

  dim_ = that.dim_;
  std::vector<Vector>::operator=(std::move(that));

  return (*this);
}

}  // namespace lds

#endif
```


-------------------------------

Updated on  4 May 2022 at 16:34:53 Eastern Daylight Time
