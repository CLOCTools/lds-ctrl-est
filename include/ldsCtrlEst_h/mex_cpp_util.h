//===-- ldsCtrlEst_h/mex_cpp_util.h - Mex C++ API Utilities -----*- C++ -*-===//
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
///
/// \file
/// This file defines utility functions for interoperability between armadillo
/// and Matlab's C++ mex API.
///
/// \brief arma <-> mex interoperability utilities (Matlab C++ API)
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_MEXCPP_UTIL_H
#define LDSCTRLEST_MEXCPP_UTIL_H

#include <ldsCtrlEst>

#include "mex.hpp"
#include "mexAdapter.hpp"

/// utilities for arma/mex interface
/// *using Matlab C++ API*
/// \brief arma/mex interface using Matlab C++ API
namespace armamexcpp {
/**
 * @brief      Convert matlab cell array to vector of armadillo matrices
 *
 * @param      matlab_cell  matlab cell
 *
 * @tparam     T            type
 *
 * @return     vector of armadillo matrices of type T
 */
template <class T>
auto m2a_cellmat(matlab::data::CellArray& matlab_cell)
    -> std::vector<arma::Mat<T>> {
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

/**
 * @brief      Convert matlab matrix to a vector of scalars
 *
 * @param      matlab_array  matlab array
 *
 * @tparam     T             type
 *
 * @return     vector of type T
 */
template <class T>
auto m2s_vec(matlab::data::TypedArray<T>& matlab_array) -> std::vector<T> {
  size_t n_elem = matlab_array.getNumberOfElements();
  T* ptr = matlab_array.release().get();
  std::vector<T> vec(ptr, ptr + n_elem);
  return vec;
};

/**
 * @brief      Convert matlab to armadillo vector
 *
 * @param      matlab_array  matlab array
 *
 * @tparam     T             type
 *
 * @return     armadillo vector of type T
 */
template <class T>
auto m2a_vec(matlab::data::TypedArray<T> matlab_array) -> arma::Col<T> {
  size_t n_elem = matlab_array.getNumberOfElements();
  T* ptr = matlab_array.release().get();
  arma::Col<T> vec(ptr, n_elem);  //, false);
  return vec;
};

/**
 * @brief      Convert matlab to armadillo matrix
 *
 * @param      matlab_array  matlab matrix
 *
 * @tparam     T             type
 *
 * @return     armadillo matrix of type T
 */
template <class T>
auto m2a_mat(matlab::data::TypedArray<T> matlab_array) -> arma::Mat<T> {
  // ArrayDimensions == std::vector<size_t>
  auto dims = matlab_array.getDimensions();
  T* ptr = matlab_array.release().get();

  // mat(ptr_aux_mem, n_rows, n_cols, copy_aux_mem = true, strict = false)
  arma::Mat<T> mat(ptr, dims[0], dims[1]);  //, false);
  return mat;
};

/**
 * @brief      Convert armadillo to matlab matrix
 *
 * @param      arma_mat  arma matrix
 * @param      factory   matlab "array factory"
 *
 * @tparam     T         type
 *
 * @return     matlab matrix
 */
template <class T>
auto a2m_mat(const arma::Mat<T>& arma_mat, matlab::data::ArrayFactory& factory)
    -> matlab::data::TypedArray<T> {
  const matlab::data::TypedArray<T> matlab_mat = factory.createArray<T>(
      {arma_mat.n_rows, arma_mat.n_cols}, arma_mat.memptr(),
      arma_mat.memptr() + arma_mat.n_elem);
  return matlab_mat;
};

/**
 * @brief      Convert armadillo to matlab vector
 *
 * @param      arma_vec  armadillo vector
 * @param      factory   matlab "array factory"
 *
 * @tparam     T         type
 *
 * @return     matlab matrix
 */
template <class T>
auto a2m_vec(const arma::Col<T>& arma_vec, matlab::data::ArrayFactory& factory)
    -> matlab::data::TypedArray<T> {
  const matlab::data::TypedArray<T> matlab_mat =
      factory.createArray<T>({arma_vec.n_elem, 1}, arma_vec.memptr(),
                             arma_vec.memptr() + arma_vec.n_elem);
  return matlab_mat;
};

/**
 * @brief      Convert vector of scalar T to matlab matrix
 *
 * @param      std_vec  standard vector
 * @param      factory  matlab "array factory"
 *
 * @tparam     T        type
 *
 * @return     matlab matrix
 */
template <class T>
auto s2m_vec(const std::vector<T>& std_vec, matlab::data::ArrayFactory& factory)
    -> matlab::data::TypedArray<T> {
  const matlab::data::TypedArray<T> matlab_mat = factory.createArray<T>(
      {std_vec.size(), 1}, std_vec.data(), std_vec.data() + std_vec.size());
  return matlab_mat;
};
}  // namespace armamexcpp

#endif
