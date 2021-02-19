#ifndef LDSCTRLEST_MEXCPP_UTIL_HPP
#define LDSCTRLEST_MEXCPP_UTIL_HPP

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

#include "mex.hpp"
#include "mexAdapter.hpp"

/// utilities for arma/mex interface
/// *using Matlab C++ API*
namespace armamexcpp {
/// Matlab CellArray of Mats to std::vector of Aramadillo Mats
template <class T>
auto m2a_cellmat(matlab::data::CellArray& matlab_cell)
    -> std::vector<arma::Mat<T>> {
  size_t n_cells = matlab_cell.getNumberOfElements();
  std::vector<arma::Mat<T>> arma_mat(n_cells,
                                     arma::mat(1, 1, arma::fill::zeros));
  for (size_t k = 0; k < n_cells; k++) {
    matlab::data::TypedArray<T> matlab_mat = matlab_cell[k];
    auto dims = matlab_mat.getDimensions();
    arma_mat[k] = arma::Mat<T>(matlab_mat.release().get(), dims[0], dims[1]);
  }
  return arma_mat;
};

template <class T>
auto m2s_vec(matlab::data::TypedArray<T>& matlab_array) -> std::vector<T> {
  size_t n_elem = matlab_array.getNumberOfElements();
  double* ptr = matlab_array.release().get();
  std::vector<T> vec(ptr, ptr + n_elem);
  return vec;
};

template <class T>
auto m2a_vec(matlab::data::TypedArray<T> matlab_array) -> arma::Col<T> {
  size_t n_elem = matlab_array.getNumberOfElements();
  T* ptr = matlab_array.release().get();
  arma::Col<T> vec(ptr, n_elem);  //, false);
  return vec;
};

template <class T>
auto m2a_mat(matlab::data::TypedArray<T> matlab_array) -> arma::Mat<T> {
  // ArrayDimensions == std::vector<size_t>
  auto dims = matlab_array.getDimensions();
  T* ptr = matlab_array.release().get();

  // n.b., I am pretty sure matlab makes vectors have 2dims still,
  // so going to assume it's safe to look at 0, 1...
  // mat(ptr_aux_mem, n_rows, n_cols, copy_aux_mem = true, strict = false)
  arma::Mat<T> mat(ptr, dims[0], dims[1]);  //, false);
  return mat;
};

template <class T>
auto a2m_mat(arma::Mat<T>& arma_mat, matlab::data::ArrayFactory& factory)
    -> matlab::data::TypedArray<T> {
  const matlab::data::TypedArray<T> matlab_mat = factory.createArray<T>(
      {arma_mat.n_rows, arma_mat.n_cols}, arma_mat.memptr(),
      arma_mat.memptr() + arma_mat.n_elem);
  return matlab_mat;
};

template <class T>
auto a2m_vec(arma::Col<T>& arma_vec, matlab::data::ArrayFactory& factory)
    -> matlab::data::TypedArray<T> {
  const matlab::data::TypedArray<T> matlab_mat =
      factory.createArray<T>({arma_vec.n_elem, 1}, arma_vec.memptr(),
                             arma_vec.memptr() + arma_vec.n_elem);
  return matlab_mat;
};

template <class T>
auto s2m_vec(std::vector<T>& std_vec, matlab::data::ArrayFactory& factory)
    -> matlab::data::TypedArray<T> {
  const matlab::data::TypedArray<T> matlab_mat = factory.createArray<T>(
      {std_vec.size(), 1}, std_vec.data(), std_vec.data() + std_vec.size());
  return matlab_mat;
};
}  // namespace armamexcpp

#endif
