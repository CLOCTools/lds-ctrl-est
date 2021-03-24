---
title: ldsCtrlEst_h/mex_c_util.h
summary: arma <-> mex interoperability utilities (Matlab C API) 

---

# ldsCtrlEst_h/mex_c_util.h

arma <-> mex interoperability utilities (Matlab C API)  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[armamexc](/lds-ctrl-est/docs/api/namespaces/namespacearmamexc/)** <br>arma/mex interface using Matlab C API  |

## Detailed Description



This file defines utility functions for interoperability between armadillo and Matlab/Octave's C mex API. 





## Source code

```cpp
//===-- ldsCtrlEst_h/mex_c_util.h - Mex C API Utilities ---------*- C++ -*-===//
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

#ifndef LDSCTRLEST_MEXC_UTIL_H
#define LDSCTRLEST_MEXC_UTIL_H

#include <ldsCtrlEst>

#include "mex.h"

// // If Matlab_FOUND, include matrix.h.
// // (Octave does not need/have it.)
// #ifdef Matlab_FOUND
// #include "matrix.h"
// #endif

namespace armamexc {
template <class T>
inline auto m2T_scalar(const mxArray *matlab_scalar) -> T {
  if (mxGetData(matlab_scalar)) {
    return static_cast<T>(mxGetScalar(matlab_scalar));
  }
  mexErrMsgTxt("No data available.");
  return 0;
}

template <class T>
inline auto m2a_mat(const mxArray *matlab_mat, bool copy_aux_mem = false,
                    bool strict = true) -> arma::Mat<T> {
  if (mxGetData(matlab_mat)) {
    const mwSize n_dim = mxGetNumberOfDimensions(matlab_mat);
    if (n_dim == 2) {
      return arma::Mat<T>(static_cast<T *>(mxGetData(matlab_mat)),
                          mxGetM(matlab_mat), mxGetN(matlab_mat), copy_aux_mem,
                          strict);
    }
    mexErrMsgTxt("Number of dimensions must be 2.");
    return arma::Mat<T>();
  }
  mexErrMsgTxt("No data available.");
  return arma::Mat<T>();
}

// TODO(mfbolus): make these templated.

template <typename T>
inline auto a2m_mat(arma::Mat<T> const &arma_mat) -> mxArray * {
  mxArray *matlab_mat = mxCreateNumericMatrix(arma_mat.n_rows, arma_mat.n_cols,
                                              mxDOUBLE_CLASS, mxREAL);
  if (matlab_mat) {
    auto *dst_pointer = static_cast<T *>(mxGetData(matlab_mat));
    const auto *src_pointer = const_cast<T *>(arma_mat.memptr());
    // TODO(mfbolus): I just want to MOVE the data, not copy.
    std::memcpy(dst_pointer, src_pointer, sizeof(T) * arma_mat.n_elem);
    return matlab_mat;
  }
  mexErrMsgTxt("Failed to create matlab mat from arma::Mat.");
  return nullptr;
}

template <typename T>
inline auto a2m_vec(arma::Col<T> const &arma_vec) -> mxArray * {
  mxArray *matlab_mat =
      mxCreateNumericMatrix(arma_vec.n_elem, 1, mxDOUBLE_CLASS, mxREAL);
  if (matlab_mat) {
    auto *dst_pointer = static_cast<T *>(mxGetData(matlab_mat));
    const auto *src_pointer = const_cast<T *>(arma_vec.memptr());
    // TODO(mfbolus): I just want to MOVE the data, not copy.
    std::memcpy(dst_pointer, src_pointer, sizeof(T) * arma_vec.n_elem);
    return matlab_mat;
  }
  mexErrMsgTxt("Failed to create matlab mat from arma::Col.");
  return nullptr;
}

}  // namespace armamexc

#endif
```


-------------------------------

Updated on 24 March 2021 at 13:26:05 EDT
