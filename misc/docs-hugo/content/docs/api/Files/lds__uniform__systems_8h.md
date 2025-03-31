---
title: ldsCtrlEst_h/lds_uniform_systems.h
summary: List of uniformly sized Systems. 

---

# ldsCtrlEst_h/lds_uniform_systems.h

List of uniformly sized Systems.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/lds-ctrl-est/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::UniformSystemList](/lds-ctrl-est/docs/api/classes/classlds_1_1_uniform_system_list/)**  |

## Detailed Description



This file provides a container for uniformly sized Systems. 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_uniform_systems.h - Uniform Systems ----*- C++ -*-===//
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

#ifndef LDSCTRLEST_LDS_UNIFORM_SYSTEMS_H
#define LDSCTRLEST_LDS_UNIFORM_SYSTEMS_H

#include <array>   // std::array
#include <vector>  // std::vector

// namespace
#include "lds.h"
// System type
#include "lds_sys.h"

namespace lds {
template <typename System>
class UniformSystemList : public std::vector<System> {
  static_assert(std::is_base_of<lds::System, System>::value,
                "System must be derived from lds::System type.");

 private:
  // TODO(mfbolus): would rather *uncomment* the below for sake of conversion
  // using std::vector<System>::vector;
  using std::vector<System>::operator=;
  using std::vector<System>::operator[];
  using std::vector<System>::at;
  using std::vector<System>::begin;
  using std::vector<System>::end;
  using std::vector<System>::size;

 public:
  UniformSystemList() = default;

  explicit UniformSystemList(const std::vector<System>& systems,
                             std::array<size_t, 3> dim = {0, 0, 0});

  explicit UniformSystemList(std::vector<System>&& systems,
                             std::array<size_t, 3> dim = {0, 0, 0});

  UniformSystemList(std::initializer_list<System> systems,
                    std::array<size_t, 3> dim = {0, 0, 0});

  UniformSystemList(const UniformSystemList& that);
  UniformSystemList(UniformSystemList&& that) noexcept;

  ~UniformSystemList() = default;

  const std::array<size_t, 3>& dim() const { return dim_; }

  size_t size() { return std::vector<System>::size(); };

  const System& at(size_t n) { return std::vector<System>::at(n); };

  void Swap(System& that, size_t n);

  UniformSystemList& operator=(const UniformSystemList& that);
  UniformSystemList& operator=(UniformSystemList&& that) noexcept;

 private:
  void CheckDimensions(std::array<size_t, 3> dim);
  std::array<size_t, 3> dim_{};
};

template <typename System>
inline void UniformSystemList<System>::Swap(System& that, size_t n) {
  // make sure request in range
  if (n >= this->size()) {
    std::cerr
        << "Requested UniformSystemList element out of bounds. Skipping.\n";
    return;
  }
  // check dim
  bool does_match = (dim_[0] == that.n_u()) && (dim_[1] == that.n_x()) &&
                    (dim_[2] == that.n_y());
  if (!does_match) {
    std::cerr << "Cannot swap a UniformSystemList element for an element of "
                 "different size. Skipping.\n";
    return;
  }
  // if checks pass, perform swap
  System tmp = std::move((*this)[n]);
  (*this)[n] = std::move(that);
  that = std::move(tmp);
}

template <typename System>
inline UniformSystemList<System>& UniformSystemList<System>::operator=(
    const UniformSystemList& that) {
  // check dimensions
  if (!this->empty()) {
    if (this->size() != that.size()) {
      std::ostringstream ss;
      ss << "cannot reassign " << this->size() << " systems with "
         << that.size() << " systems";
      throw std::runtime_error(ss.str());
    }

    if (dim_[0] + dim_[1] + dim_[2]) {
      std::array<size_t, 3> other_dim(that.dim());
      if (dim_ != other_dim) {
        std::ostringstream ss;
        ss << "cannot reassign systems of size " << dim_[0] << "x" << dim_[1]
           << "x" << dim_[2] << " with systems of size " << other_dim[0] << "x"
           << other_dim[1] << "x" << dim_[2];
        throw std::runtime_error(ss.str());
      }
    }
  }

  for (size_t k = 0; k < this->size(); k++) {
    (*this)[k] = that[k];
  }
  return (*this);
}

template <typename System>
inline UniformSystemList<System>& UniformSystemList<System>::operator=(
    UniformSystemList&& that) noexcept {
  // // check dimensions
  // // if empty, assume a default constructed object and safe to move
  // if (!this->empty()) {
  //   if (this->size() != that.size()) {
  //     std::cerr << "Cannot reassign " << this->size() << " systems with "
  //               << that.size() << " systems. Skipping.\n";
  //     return (*this);
  //   }
  //
  //   // if dimensions a not zero and do not match, skip move with error
  //   message. if (dim_[0] + dim_[1] + dim_[2]) {
  //     bool does_match = (dim_[0] == that.at(0).n_u()) &&
  //                       (dim_[1] == that.at(0).n_x()) &&
  //                       (dim_[2] == that.at(0).n_y());
  //     if (!does_match) {
  //       std::cerr
  //           << "Cannot move a UniformSystemList element for an element of "
  //              "different size. Skipping.\n";
  //       return (*this);
  //     }
  //   }
  // }

  dim_ = that.dim_;
  std::vector<System>::operator=(std::move(that));

  return (*this);
}

template <typename System>
UniformSystemList<System>::UniformSystemList(const std::vector<System>& systems,
                                             std::array<size_t, 3> dim)
    : std::vector<System>(systems) {
  CheckDimensions(dim);
}

template <typename System>
UniformSystemList<System>::UniformSystemList(std::vector<System>&& systems,
                                             std::array<size_t, 3> dim)
    : std::vector<System>(std::move(systems)) {
  CheckDimensions(dim);
};

template <typename System>
UniformSystemList<System>::UniformSystemList(
    std::initializer_list<System> systems, std::array<size_t, 3> dim)
    : std::vector<System>(systems) {
  CheckDimensions(dim);
};

template <typename System>
UniformSystemList<System>::UniformSystemList(const UniformSystemList& that)
    : std::vector<System>(that) {
  (*this) = that;
}

template <typename System>
UniformSystemList<System>::UniformSystemList(UniformSystemList&& that) noexcept
    : std::vector<System>(std::move(that)) {
  this->dim_[0] = this->at(0).n_u();
  this->dim_[1] = this->at(0).n_x();
  this->dim_[2] = this->at(0).n_y();
}

template <typename System>
void UniformSystemList<System>::CheckDimensions(std::array<size_t, 3> dim) {
  if (dim[0] + dim[1] + dim[2]) {
    dim_ = dim;
  } else {
    dim_[0] = this->at(0).n_u();
    dim_[1] = this->at(0).n_x();
    dim_[2] = this->at(0).n_y();
  }

  // make sure dimensiolaties are all uniform
  bool does_match(true);
  for (const System& sys : *this) {
    does_match = does_match && (sys.n_u() == dim_[0]);
    does_match = does_match && (sys.n_x() == dim_[1]);
    does_match = does_match && (sys.n_y() == dim_[2]);
    if (!does_match) {
      throw std::runtime_error(
          "Dimensionality of one or more input systems are not uniform.");
    }
  }
}

}  // namespace lds

#endif
```


-------------------------------

Updated on 31 March 2025 at 16:24:27 EDT
