---
title: src/lds_uniform_vecs.cpp
summary: Uniformly sized vectors. 

---

# src/lds_uniform_vecs.cpp

Uniformly sized vectors.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/lds-ctrl-est/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |

## Detailed Description



This file provides a container for uniformly sized vectors. 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_uniform_vecs.cpp - Uniform Matrices --------------===//
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

#include <ldsCtrlEst_h/lds_uniform_vecs.h>

namespace lds {

UniformVectorList::UniformVectorList(const std::vector<Vector>& vecs,
                                     size_t dim)
    : vector(vecs) {
  CheckDimensions(dim);
}

UniformVectorList::UniformVectorList(std::vector<Vector>&& vecs, size_t dim)
    : vector(std::move(vecs)) {
  CheckDimensions(dim);
};

UniformVectorList::UniformVectorList(std::initializer_list<Vector> vecs,
                                     size_t dim)
    : vector(vecs) {
  CheckDimensions(dim);
};

UniformVectorList::UniformVectorList(const UniformVectorList& that)
    : vector(that) {
  (*this) = that;
}

UniformVectorList::UniformVectorList(UniformVectorList&& that) noexcept
    : vector(std::move(that)) {
  this->dim_ = this->at(0).n_elem;
}

void UniformVectorList::CheckDimensions(size_t dim) {
  if (dim) {
    dim_ = dim;
  } else {
    dim_ = this->at(0).n_elem;
  }

  // make sure dimensiolaties are all uniform
  bool does_match(true);
  for (const Vector& vec : *this) {
    does_match = does_match && (vec.n_elem == dim_);
    if (!does_match) {
      throw std::runtime_error(
          "Dimensionality of one or more input matrices are not uniform.");
    }
  }
}

}  // namespace lds
```


-------------------------------

Updated on 19 May 2022 at 17:16:05 Eastern Daylight Time
