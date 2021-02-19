#ifndef LDSCTRLEST_MEXC_UTIL_HPP
#define LDSCTRLEST_MEXC_UTIL_HPP

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

#include "mex.h"

// If Matlab_FOUND, include matrix.h. Octave doesn't need it.
#ifdef Matlab_FOUND
#include "matrix.h"
#endif

/// utilities for arma/mex interface
/// *using Matlab C API*
namespace armamexc {
/// From Matlab mxArray to scalar of type T.
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
      return arma::Mat<T>(static_cast<double *>(mxGetData(matlab_mat)),
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
inline auto a2m_mat(arma::Mat<double> const &arma_mat) -> mxArray * {
  mxArray *matlab_mat = mxCreateNumericMatrix(arma_mat.n_rows, arma_mat.n_cols,
                                              mxDOUBLE_CLASS, mxREAL);
  if (matlab_mat) {
    auto *dst_pointer = static_cast<double *>(mxGetData(matlab_mat));
    const auto *src_pointer = const_cast<double *>(arma_mat.memptr());
    // TODO(mfbolus): I just want to MOVE the data, not copy.
    std::memcpy(dst_pointer, src_pointer, sizeof(double) * arma_mat.n_elem);
    return matlab_mat;
  }
  mexErrMsgTxt("Failed to create matlab mat from arma::Mat.");
  return nullptr;
}

inline auto a2m_vec(arma::Col<double> const &arma_vec) -> mxArray * {
  mxArray *matlab_mat =
      mxCreateNumericMatrix(arma_vec.n_elem, 1, mxDOUBLE_CLASS, mxREAL);
  if (matlab_mat) {
    auto *dst_pointer = static_cast<double *>(mxGetData(matlab_mat));
    const auto *src_pointer = const_cast<double *>(arma_vec.memptr());
    // TODO(mfbolus): I just want to MOVE the data, not copy.
    std::memcpy(dst_pointer, src_pointer, sizeof(double) * arma_vec.n_elem);
    return matlab_mat;
  }
  mexErrMsgTxt("Failed to create matlab mat from arma::Col.");
  return nullptr;
}

}  // namespace armamexc

#endif
