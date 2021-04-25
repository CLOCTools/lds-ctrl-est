//===-- ldsCtrlEst_h/lds_fit.h - Fit Type for LDS ---------------*- C++ -*-===//
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
///
/// \file
/// This file declares and partially defines the base fit type for a linear
/// dynamical system. It is expounded upon by variants with Gaussian and Poisson
/// observation assumptions for fitting.
///
/// \brief LDS base fit type
//===----------------------------------------------------------------------===//

#ifndef LDS_FIT_HPP
#define LDS_FIT_HPP

// namespace
#include "lds.h"
#include "lds_uniform_mats.h"

namespace lds {
/// LDS Fit Type
class Fit {
 public:
  /**
   * @brief      Constructs a new Fit.
   */
  Fit() = default;
  /**
   * @brief      Constructs a new Fit.
   *
   * @param  n_u   number of inputs
   * @param  n_x   number of states
   * @param  n_y   number of outputs
   * @param  dt    sample period
   */
  Fit(size_t n_u, size_t n_x, size_t n_y, data_t dt);

  // get methods
  /// gets number of inputs
  size_t n_u() const { return n_u_; };
  /// gets number of states
  size_t n_x() const { return n_x_; };
  /// gets number of outputs
  size_t n_y() const { return n_y_; };
  /// gets sample period
  data_t dt() const { return dt_; };
  /// gets state matrix
  const Matrix& A() const { return A_; };
  /// gets input matrix
  const Matrix& B() const { return B_; };
  /// gets input gain
  const Vector& g() const { return g_; };
  /// gets process disturbance
  const Vector& m() const { return m_; };
  /// gets process noise covariance
  const Matrix& Q() const { return Q_; };
  /// gets initial state estimate
  const Vector& x0() const { return x0_; };
  /// gets covariance of initial state estimate
  const Matrix& P0() const { return P0_; };
  /// gets output matrix
  const Matrix& C() const { return C_; };
  /// gets output bias
  const Vector& d() const { return d_; };
  // gets measurement noise
  virtual const Matrix& R() const = 0;

  // set methods (e.g., seeding initial fit values)
  /// sets state matrix
  void set_A(const Matrix& A) { Reassign(A_, A); };
  /// sets input matrix
  void set_B(const Matrix& B) { Reassign(B_, B); };
  /// sets input gain/conversion factor
  void set_g(const Vector& g) { Reassign(g_, g); };
  /// sets process disturbance
  void set_m(const Vector& m) { Reassign(m_, m); };
  /// sets process noise covariance
  void set_Q(const Matrix& Q) {
    Reassign(Q_, Q);
    ForceSymPD(Q_);
  };
  /// sets output noise covariance (if any)
  virtual void set_R(const Matrix& R) = 0;
  /// sets initial state estimate
  void set_x0(const Vector& x0) { Reassign(x0_, x0); };
  /// sets intial state estimate covariance
  void set_P0(const Matrix& P0) {
    Reassign(P0_, P0);
    ForceSymPD(P0_);
  };
  /// sets output matrix
  void set_C(const Matrix& C) { Reassign(C_, C); };
  /// sets output bias
  void set_d(const Vector& d) { Reassign(d_, d); };

  /**
   * @brief      system dynamics function
   *
   * @param      x     state estimate (over time)
   * @param      u     input (over time)
   * @param      t     time index
   *
   * @return     view of updated state
   */
  View f(Matrix& x, const Matrix& u, size_t t) {
    x.col(t) = A_ * x.col(t - 1) + B_ * (g_ % u.col(t - 1)) + m_;
    return x.col(t);
  };

  /**
   * @brief      system dynamics function
   *
   * @param      x_pre   predicted state est.
   * @param      x_post  posterior state est.
   * @param      u       input (over time)
   * @param      t       time index
   *
   * @return     view of predicted state
   */
  View f(Matrix& x_pre, const Matrix& x_post, const Matrix& u, size_t t) {
    x_pre.col(t) = A_ * x_post.col(t - 1) + B_ * (g_ % u.col(t - 1)) + m_;
    return x_pre.col(t);
  };

  /**
   * @brief      output function
   *
   * @param      y     output estimate (over time)
   * @param      x     state estimate (over time)
   * @param      t     time index
   *
   * @return     output
   */
  virtual View h(Matrix& y, const Matrix& x, size_t t) = 0;

 protected:
  data_t dt_{};  ///< sample period

  // Dynamics
  Matrix A_;  ///< state matrix
  Matrix B_;  ///< input matrix
  Vector g_;  ///< input gain
  Vector m_;  ///< process noise mean
  Matrix Q_;  ///< process noise cov

  // Output
  Matrix C_;  ///< output matrix
  Vector d_;  ///< output bias
  Matrix R_; ///< measurement noise

  // initial conditions
  Vector x0_;  ///< initial state
  Matrix P0_;  ///< initial covar

  size_t n_u_{};  ///< number of inputs
  size_t n_x_{};  ///< number of states
  size_t n_y_{};  ///< number of outputs
};

}  // namespace lds
#endif
