---
title: ldsCtrlEst_h/mex_cpp_util.h
summary: arma <-> mex interoperability utilities (Matlab C++ API) 

---

# ldsCtrlEst_h/mex_cpp_util.h

arma <-> mex interoperability utilities (Matlab C++ API)  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[armamexcpp](/lds-ctrl-est/docs/api/namespaces/namespacearmamexcpp/)** <br>arma/mex interface using Matlab C++ API  |

## Detailed Description



This file defines utility functions for interoperability between armadillo and Matlab's C++ mex API. 





## Source code

```cpp
//===-- ldsCtrlEst_h/mex_cpp_util.h - Mex C++ API Utilities -----*- C++ -*-===//
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

#ifndef LDSCTRLEST_MEXCPP_UTIL_H
#define LDSCTRLEST_MEXCPP_UTIL_H

#include <ldsCtrlEst>

#include "mex.hpp"
#include "mexAdapter.hpp"

namespace armamexcpp {
template <class T>
std::vector<arma::Mat<T>> m2a_cellmat(matlab::data::CellArray& matlab_cell) {
  size_t n_cells = matlab_cell.getNumberOfElements();
  std::vector<arma::Mat<T>> arma_mat(n_cells,
                                     arma::Mat<T>(1, 1, arma::fill::zeros));
  for (size_t k = 0; k < n_cells; k++) {
    matlab::data::TypedArray<T> matlab_mat = matlab_cell[k];
    auto dims = matlab_mat.getDimensions();
    arma_mat[k] = arma::Mat<T>(matlab_mat.release().get(), dims[0], dims[1]);
  }
  return arma_mat;
};

template <class T>
std::vector<T> m2s_vec(matlab::data::TypedArray<T>& matlab_array) {
  size_t n_elem = matlab_array.getNumberOfElements();
  T* ptr = matlab_array.release().get();
  std::vector<T> vec(ptr, ptr + n_elem);
  return vec;
};

template <class T>
arma::Col<T> m2a_vec(matlab::data::TypedArray<T> matlab_array) {
  size_t n_elem = matlab_array.getNumberOfElements();
  // T* ptr = matlab_array.release().get();
  // arma::Col<T> vec(ptr, n_elem);  //, false);
  // TODO(mfbolus): for some reason, using the above pointer at times leads to
  // getting garbage values. matlab array values may be stored in non-contiguous
  // memory?
  arma::Col<T> vec(n_elem, arma::fill::zeros);
  for (size_t k = 0; k < n_elem; k++) {
    vec[k] = matlab_array[k];
  }
  return vec;
};

template <class T>
arma::Mat<T> m2a_mat(matlab::data::TypedArray<T> matlab_array) {
  // ArrayDimensions == std::vector<size_t>
  auto dims = matlab_array.getDimensions();
  // T* ptr = matlab_array.release().get();
  // // mat(ptr_aux_mem, n_rows, n_cols, copy_aux_mem = true, strict = false)
  // arma::Mat<T> mat(ptr, dims[0], dims[1]);  //, false);

  // TODO(mfbolus): for some reason, using the above pointer at times leads to
  // getting garbage values. matlab array values may be stored in non-contiguous
  // memory?
  //
  // armadillo and matlab both use column-major ordering, so this should work:
  size_t n_elem = dims[0] * dims[1];
  arma::Mat<T> mat(dims[0], dims[1], arma::fill::zeros);
  size_t k(0);
  for (auto m: matlab_array) {
    mat[k] = m;
    k++;
  }
  return mat;
};

template <class T>
matlab::data::TypedArray<T> a2m_mat(const arma::Mat<T>& arma_mat,
                                    matlab::data::ArrayFactory& factory) {
  const matlab::data::TypedArray<T> matlab_mat = factory.createArray<T>(
      {arma_mat.n_rows, arma_mat.n_cols}, arma_mat.memptr(),
      arma_mat.memptr() + arma_mat.n_elem);
  return matlab_mat;
};

template <class T>
matlab::data::TypedArray<T> a2m_vec(const arma::Col<T>& arma_vec,
                                    matlab::data::ArrayFactory& factory) {
  const matlab::data::TypedArray<T> matlab_mat =
      factory.createArray<T>({arma_vec.n_elem, 1}, arma_vec.memptr(),
                             arma_vec.memptr() + arma_vec.n_elem);
  return matlab_mat;
};

template <class T>
matlab::data::TypedArray<T> s2m_vec(const std::vector<T>& std_vec,
                                    matlab::data::ArrayFactory& factory) {
  const matlab::data::TypedArray<T> matlab_mat = factory.createArray<T>(
      {std_vec.size(), 1}, std_vec.data(), std_vec.data() + std_vec.size());
  return matlab_mat;
};
}  // namespace armamexcpp

#endif
```


-------------------------------

Updated on 25 April 2021 at 11:04:30 EDT
