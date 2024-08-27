//===-- ldsCtrlEst_h/lds_sys.h - LDS ----------------------------*- C++ -*-===//
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
/// This file declares and partially defines the base type for linear dynamical
/// systems (`lds::System`). Note that this class defines the underlying linear
/// dynamics, but does not have output functions.Gaussian- and Poisson-output
/// variants will be built upon this class.
///
/// \brief LDS base type
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_SYS_H
#define LDSCTRLEST_LDS_SYS_H

#include "lds.h"
#include "lds_uniform_mats.h"

namespace lds {
/// Linear Dynamical System Type
class System {
 public:
  /**
   * @brief      Constructs a new System.
   */
  System() = default;

  /**
   * @brief      constructs a new System
   *
   * @param      n_u   number of inputs
   * @param      n_x   number of states
   * @param      n_y   number of outputs
   * @param      dt    sample period
   * @param      p0    diagonal elements for state estimate covariance
   * @param      q0    diagonal elements for process noise covariance
   */
  System(size_t n_u, size_t n_x, size_t n_y, data_t dt, data_t p0 = kDefaultP0,
         data_t q0 = kDefaultQ0);

  virtual ~System() {}

  /**
   * Given current measurement and input, filter data to produce causal state
   * estimates using Kalman filtering, which procedes by predicting the state
   * and subsequently updating.
   *
   * @brief      Filter data to produce causal state estimates
   *
   * @param      u_tm1  input at t-minus-1
   * @param      z_t    current measurement
   */
  void Filter(const Vector& u_tm1, const Vector& z);

  /**
   * @brief      simulates system (single time step)
   *
   * @param      u_tm1  input at time t-1
   *
   * @return     simulated measurement at time t
   */
  virtual const Vector& Simulate(const Vector& u_tm1) = 0;

  /**
   * @brief      system dynamics function
   *
   * @param      u             input
   * @param      do_add_noise  whether to add simulated process noise
   */
  void f(const Vector& u, bool do_add_noise = false) {
    x_ = A_ * x_ + B_ * (g_ % u) + m_;
    if (do_add_noise) {
      x_ += arma::mvnrnd(Vector(n_x_).fill(0), Q_);
    }
  };

  /**
   * @brief      system output function
   */
  virtual void h() = 0;

  /**
   * @brief      system output function (stateless)
   * @param      x_t  state at time t
   * @return     predicted state at time t + 1
   */
  virtual Vector h_(Vector x) = 0;

  /// Get number of inputs
  size_t n_u() const { return n_u_; };
  /// Get number of states
  size_t n_x() const { return n_x_; };
  /// Get number of outputs
  size_t n_y() const { return n_y_; };
  /// Get sample period
  data_t dt() const { return dt_; };

  /// Get current state
  const Vector& x() const { return x_; };
  /// Get covariance of state estimate
  const Matrix& P() const { return P_; };
  /// Get current process disturbance/bias
  const Vector& m() const { return m_; };
  /// Get covariance of process disturbance estimate
  const Matrix& P_m() const { return P_m_; };
  /// Get C*x
  const Vector& cx() const { return cx_; };
  /// Get output
  const Vector& y() const { return y_; };

  /// Get initial state
  const Vector& x0() const { return x0_; };
  /// Get initial disturbance
  const Vector& m0() const { return m0_; };

  /// Get state matrix
  const Matrix& A() const { return A_; };
  /// Get input matrix
  const Matrix& B() const { return B_; };
  /// Get input gain/conversion factor
  const Vector& g() const { return g_; };
  /// Get output matrix
  const Matrix& C() const { return C_; };
  /// Get output bias
  const Vector& d() const { return d_; };
  /// Get estimator gain
  const Matrix& Ke() const { return Ke_; };
  /// Get estimator gain for process disturbance (m)
  const Matrix& Ke_m() const { return Ke_m_; };
  /// Get process noise covariance
  const Matrix& Q() { return Q_; };
  /// Get process noise covariance of disturbance evoluation
  const Matrix& Q_m() { return Q_m_; };
  /// Get covariance of initial state
  const Matrix& P0() { return P0_; };
  /// Get covariance of initial process disturbance
  const Matrix& P0_m() { return P0_m_; };

  /// Set state matrix
  void set_A(const Matrix& A) { Reassign(A_, A); };
  /// Set input matrix
  void set_B(const Matrix& B) { Reassign(B_, B); };
  /// Set process disturbance
  void set_m(const Vector& m, bool do_force_assign = false) {
    Reassign(m0_, m);
    if ((!do_adapt_m) || do_force_assign) {
      Reassign(m_, m);
    }
  };
  /// Set input gain
  void set_g(const Vector& g) { Reassign(g_, g); };
  /// Set process noise covariance
  void set_Q(const Matrix& Q) { Reassign(Q_, Q); };
  /// Set process noise covariance of disturbance evoluation
  void set_Q_m(const Matrix& Q_m) { Reassign(Q_m_, Q_m); };
  /// Set initial state
  void set_x0(const Vector& x0) { Reassign(x0_, x0); };
  /// Set covariance of initial state
  void set_P0(const Matrix& P0) { Reassign(P0_, P0); };
  /// Set covariance of initial process disturbance
  void set_P0_m(const Matrix& P0_m) { Reassign(P0_m_, P0_m); };
  /// Set output matrix
  void set_C(const Matrix& C) { Reassign(C_, C); };
  /// Set output bias
  void set_d(const Vector& d) { Reassign(d_, d); };
  /// Set state of system
  void set_x(const Vector& x) {
    Reassign(x_, x);
    h();
  };

  /// Reset system variables
  void Reset();

  std::vector<UniformMatrixList<kMatFreeDim2>> nstep_pred_block(
      UniformMatrixList<kMatFreeDim2> u, UniformMatrixList<kMatFreeDim2> z,
      size_t n_pred = 1);

  /// Print system variables to stdout
  void Print();

  // safe to leave this public and non-const
  bool do_adapt_m{};  ///< whether to adaptively estimate disturbance m

 protected:
  /// Recursively recalculate estimator gain (Ke)
  virtual void RecurseKe() = 0;
  void InitVars(data_t p0 = kDefaultP0, data_t q0 = kDefaultQ0);

  std::size_t n_x_{};  ///< number of states
  std::size_t n_u_{};  ///< number of inputs
  std::size_t n_y_{};  ///< number of outputs
  data_t dt_{};        ///< sample period

  // Signals:
  Vector x_;    ///< state
  Matrix P_;    ///< covariance of state estimate
  Vector m_;    ///< process disturbance
  Matrix P_m_;  ///< covariance of disturbance estimate
  Vector cx_;   ///< C*x
  Vector y_;    ///< output
  Vector z_;    ///< measurement

  // Parameters:
  Vector x0_;    ///< initial state
  Matrix P0_;    ///< covariance of initial state estimate
  Vector m0_;    ///< initial process disturbance
  Matrix P0_m_;  ///< covariance of initial disturbance est.
  Matrix A_;     ///< state matrix
  Matrix B_;     ///< input matrix
  Vector g_;     ///< input gain
  Matrix Q_;     ///< covariance of process noise
  Matrix Q_m_;   ///< covariance of disturbance random walk
  Matrix C_;     ///< output matrix
  Vector d_;     ///< output bias

  Matrix Ke_;    ///< estimator gain
  Matrix Ke_m_;  ///< estimator gain for process disturbance
};  // System

}  // namespace lds

#endif
