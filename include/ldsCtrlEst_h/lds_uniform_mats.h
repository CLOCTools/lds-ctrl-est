//===-- ldsCtrlEst_h/lds_uniform_mats.h - Uniform Matrices ------*- C++ -*-===//
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
///
/// \file
/// This file provides a container for uniformly sized matrices. Users may
/// specify one dimension to be free to vary in the list.
///
/// \brief List of uniformly sized matrices
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_UNIFORM_MATS_H
#define LDSCTRLEST_LDS_UNIFORM_MATS_H

#include <array>   // std::array
#include <vector>  // std::vector

#include "lds.h"

namespace lds {
template <MatrixListFreeDim D = kMatFreeDimNone>
class UniformMatrixList : public std::vector<Matrix> {
 private:
  // TODO(mfbolus): would rather *uncomment* the below for sake of conversion
  // using std::vector<Matrix>::vector;
  // don't allow push_back to be used since it doesn't check dims
  using std::vector<Matrix>::push_back;

 public:
  using std::vector<Matrix>::operator=;
  using std::vector<Matrix>::operator[];
  using std::vector<Matrix>::begin;
  using std::vector<Matrix>::end;
  using std::vector<Matrix>::size;
  using std::vector<Matrix>::at;
  /**
   * @brief      Constructs a new UniformMatrixList.
   */
  UniformMatrixList() = default;

  /**
   * @brief      Constructs a new UniformMatrixList by copying existing vector
   *             of Matrix if dimensions consistent.
   *
   * @param      mats  input matrices
   * @param      dim   dimensions
   */
  explicit UniformMatrixList(const std::vector<Matrix>& mats,
                             std::array<size_t, 2> dim = {0, 0});

  /**
   * @brief      Constructs a new UniformMatrixList by moving existing vector of
   *             Matrix if dimensions consistent.
   *
   * @param      mats  input matrices
   * @param      dim   dimensions
   */
  explicit UniformMatrixList(std::vector<Matrix>&& mats,
                             std::array<size_t, 2> dim = {0, 0});

  /**
   * @brief      Constructs a new UniformMatrixList from initializer_list of
   *             Matrix if dimensions consistent.
   *
   * @param      mats  input matrices
   * @param      dim   dimensions
   */
  UniformMatrixList(std::initializer_list<Matrix> mats,
                    std::array<size_t, 2> dim = {0, 0});

  /**
   * @brief      Constructs a new UniformMatrixList (copy).
   *
   * @param      that  another UniformMatrixList
   */
  UniformMatrixList(const UniformMatrixList<D>& that);

  /**
   * @brief      Constructs a new UniformMatrixList (move).
   *
   * @param      that  another UniformMatrixList
   */
  UniformMatrixList(UniformMatrixList<D>&& that) noexcept;

  /**
   * @brief      Destroys the object.
   */
  ~UniformMatrixList() = default;

  /**
   * @brief      gets dimensions of uniformly sized matrices
   *
   * @param      n     [optional] index in list of matrices
   *
   * @return     dimensions
   */
  const std::array<size_t, 2>& dim(size_t n = 0) const { return dim_.at(n); }

  /// size of container
  size_t size() { return std::vector<Matrix>::size(); };

  /// gets reference to n^th element
  const Matrix& at(size_t n) { return std::vector<Matrix>::at(n); };

  /**
   * @brief      swaps input matrix with n^th matrix of list
   *
   * @param      that  input matrix
   * @param      n     index where the matrix is moved
   */
  void Swap(Matrix& that, size_t n);

  /**
   * @brief      assigns the contents (copy)
   *
   * @param      that  another UniformMatrixList
   *
   * @return     reference to object
   */
  UniformMatrixList<D>& operator=(const UniformMatrixList<D>& that);
  /**
   * @brief      assigns the contents (move)
   *
   * @param      that  another UniformMatrixList
   *
   * @return     reference to object
   */
  UniformMatrixList<D>& operator=(UniformMatrixList<D>&& that) noexcept;
  /**
   * @brief      appends a matrix to the list
   *
   * @param      mat   input matrix
   */
  void append(const Matrix& mat);

 private:
  void CheckDimensions(std::array<size_t, 2> dim);
  std::vector<std::array<size_t, 2>> dim_;
};

template <MatrixListFreeDim D>
inline void UniformMatrixList<D>::Swap(Matrix& that, size_t n) {
  // make sure request in range
  if (n >= this->size()) {
    std::cerr
        << "Requested UniformMatrixList element out of bounds. Skipping.\n";
    return;
  }
  // check dim
  bool does_match = true;
  if (!(D == kMatFreeDim1)) {
    does_match = does_match && (dim_[0][0] == that.n_rows);
  }
  if (!(D == kMatFreeDim2)) {
    does_match = does_match && (dim_[0][1] == that.n_cols);
  }
  if (!does_match) {
    std::cerr << "Cannot swap a UniformMatrixList element for an element of "
                 "different size. Skipping.\n";
    return;
  }
  // if checks pass, perform swap
  // not moving, since it causes memory issues.
  // so this method isn't a memory-saver as designed for now
  Matrix tmp = (*this)[n];
  (*this)[n] = that;
  that = tmp;

  if (D == kMatFreeDim1) {
    this->dim_[n][0] = (*this)[n].n_rows;
  }
  if (D == kMatFreeDim2) {
    this->dim_[n][1] = (*this)[n].n_cols;
  }
}

template <MatrixListFreeDim D>
void UniformMatrixList<D>::append(const Matrix& mat) {
  std::array<size_t, 2> dim({mat.n_rows, mat.n_cols});
  CheckDimensions(dim);
  std::vector<Matrix>::push_back(mat);
  dim_.push_back(dim);
}

template <MatrixListFreeDim D>
inline UniformMatrixList<D>& UniformMatrixList<D>::operator=(
    const UniformMatrixList<D>& that) {
  // make sure dim_ vector is initialized
  if (dim_.empty()) {
    dim_ = std::vector<std::array<size_t, 2>>(that.size(), {0, 0});
  }
  // check dimensions
  if (!this->empty()) {
    if (this->size() != that.size()) {
      std::ostringstream ss;
      ss << "cannot reassign " << this->size() << " matrices with "
         << that.size() << " matrices";
      throw std::runtime_error(ss.str());
    }

    // if dimensions a not zero and do not match, skip move with error message.
    bool dims_nonzero = true;
    for (auto d : dim_) {
      if (!(D == kMatFreeDim1) && d[0] < 1) {
        dims_nonzero = false;
        break;
      }
      if (!(D == kMatFreeDim2) && d[1] < 1) {
        dims_nonzero = false;
        break;
      }
    }
    if (dims_nonzero) {
      bool does_match = true;
      if (!(D == kMatFreeDim1)) {
        does_match = does_match && (dim_[0][0] == that.at(0).n_rows);
      }
      if (!(D == kMatFreeDim2)) {
        does_match = does_match && (dim_[0][1] == that.at(0).n_cols);
      }
      if (!does_match) {
        std::ostringstream ss;
        ss << "cannot reassign matrices of size " << dim_[0][0] << "x"
           << dim_[0][1] << " with matrices of size " << that.at(0).n_rows
           << "x" << that.at(0).n_cols;
        throw std::runtime_error(ss.str());
      }
    }
  }

  for (size_t k = 0; k < this->size(); k++) {
    (*this)[k] = that[k];
    dim_[k] = that.dim(k);
  }

  return (*this);
}

template <MatrixListFreeDim D>
inline UniformMatrixList<D>& UniformMatrixList<D>::operator=(
    UniformMatrixList<D>&& that) noexcept {
  // // check dimensions
  // // if empty, assume a default constructed object and safe to move
  // if (!this->empty()) {
  //   if (this->size() != that.size()) {
  //     std::cerr << "Cannot reassign " << this->size() << " matrices with "
  //               << that.size() << " matrices. Skipping.\n";
  //     return (*this);
  //   }
  //
  //   // if dimensions a not zero and do not match, skip move with error
  //   message. bool dims_nonzero = true; for (auto d : dim_) {
  //     if (!(D == kMatFreeDim1) && (d[0] < 1)) {
  //       dims_nonzero = false;
  //       break;
  //     }
  //     if (!(D == kMatFreeDim2) && (d[1] < 1)) {
  //       dims_nonzero = false;
  //       break;
  //     }
  //   }
  //
  //   if (dims_nonzero) {
  //     bool does_match = true;
  //     if (!(D == kMatFreeDim1)) {
  //       does_match = does_match && (dim_[0][0] == that.at(0).n_rows);
  //     }
  //
  //     if (!(D == kMatFreeDim2)) {
  //       does_match = does_match && (dim_[0][1] == that.at(0).n_cols);
  //     }
  //
  //     if (!does_match) {
  //       this->at(0).print("this[0] = ");
  //       that.at(0).print("that[0] = ");
  //       std::cerr
  //           << "Cannot move a UniformMatrixList element of size (" <<
  //           that.at(0).n_rows << "," << that.at(0).n_cols << ") for an
  //           element of size (" << dim_[0][0] << "," << dim_[0][1] << ").
  //           Skipping.\n";
  //       return (*this);
  //     }
  //   }
  // }

  dim_ = that.dim_;
  std::vector<Matrix>::operator=(std::move(that));

  return (*this);
}

template <MatrixListFreeDim D>
UniformMatrixList<D>::UniformMatrixList(const std::vector<Matrix>& mats,
                                        std::array<size_t, 2> dim)
    : vector(mats) {
  CheckDimensions(dim);
}

template <MatrixListFreeDim D>
UniformMatrixList<D>::UniformMatrixList(std::vector<Matrix>&& mats,
                                        std::array<size_t, 2> dim)
    : vector(std::move(mats)) {
  CheckDimensions(dim);
};

template <MatrixListFreeDim D>
UniformMatrixList<D>::UniformMatrixList(std::initializer_list<Matrix> mats,
                                        std::array<size_t, 2> dim)
    : vector(mats) {
  CheckDimensions(dim);
};

template <MatrixListFreeDim D>
UniformMatrixList<D>::UniformMatrixList(const UniformMatrixList<D>& that)
    : vector(that) {
  (*this) = that;
}

template <MatrixListFreeDim D>
UniformMatrixList<D>::UniformMatrixList(UniformMatrixList<D>&& that) noexcept
    : vector(std::move(that)) {
  for (size_t k = 0; k < this->size(); k++) {
    std::array<size_t, 2> dim_k({this->at(k).n_rows, this->at(k).n_cols});
    dim_.push_back(dim_k);
  }
}

template <MatrixListFreeDim D>
void UniformMatrixList<D>::CheckDimensions(std::array<size_t, 2> dim) {
  // change behavior based on free dim D
  if ((dim[0] == 0) && !(D == kMatFreeDim1)) {
    dim[0] = this->at(0).n_rows;
  }
  if ((dim[1] == 0) && !(D == kMatFreeDim2)) {
    dim[1] = this->at(0).n_cols;
  }

  // make sure dimensiolaties are all uniform
  bool does_match(true);
  for (const Matrix& mat : *this) {
    if (!(D == kMatFreeDim1)) {
      does_match = does_match && (mat.n_rows == dim[0]);
    }
    if (!(D == kMatFreeDim2)) {
      does_match = does_match && (mat.n_cols == dim[1]);
    }
    if (!does_match) {
      throw std::runtime_error(
          "Dimensionality of one or more input matrices are not uniform.");
    }
  }

  dim_ = std::vector<std::array<size_t, 2>>(this->size(), dim);
  for (size_t k = 0; k < this->size(); k++) {
    dim_[k][0] = (*this)[k].n_rows;
    dim_[k][1] = (*this)[k].n_cols;
  }
}

}  // namespace lds

#endif
