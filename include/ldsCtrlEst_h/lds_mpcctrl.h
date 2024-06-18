//===-- ldsCtrlEst_h/lds_gaussian_mpcctrl.h - MPC Controller ----*- C++ -*-===//
//
// Copyright 2024 Chia-Chien Hung and Kyle Johnsen
// Copyright 2024 Georgia Institute of Technology
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
/// This file defines the type for model predictive Model Predictive
/// Control (MPC) on linear system dynamics by converting the MPC optimization
/// problem to a quadratic cost problem. The new problem is optimized using the
/// Operator Splitting Quadratic Program (OSQP).
///
/// \brief MPC Controller
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_MPCCTRL_H
#define LDSCTRLEST_LDS_MPCCTRL_H

// namespace
#include "lds.h"
// system type
#include "lds_sys.h"

// osqp
#include "osqp_arma.h"

namespace lds {

template <typename System>
class MpcController {
  static_assert(std::is_base_of<lds::System, System>::value,
                "System must be derived from lds::System");

 public:
  /**
   * @brief     Constructs a new MpcController.
   */
  MpcController() = default;

  /**
   * @brief     Constructs a new MpcController.
   *
   * @param     sys   The system being controlled
   * @param     u_lb  The lower bound of the control input
   * @param     u_ub  The upper bound of the control input
   *
   * @tparam    System  The system type
   */
  MpcController(const System& sys, Vector u_lb, Vector u_ub);

  /**
   * @brief     Constructs a new MpcController.
   *
   * @param     sys   The system being controlled
   * @param     u_lb  The lower bound of the control input
   * @param     u_ub  The upper bound of the control input
   *
   * @tparam    System  The system type
   */
  MpcController(System&& sys, Vector u_lb, Vector u_ub);

  /**
   *
   */
  Vector Control(data_t t_sim, const Vector& x0, const Vector& u0,
                 const Matrix& xr);

  // getters
  const System& sys() const { return sys_; }

  // setters
  void set_control(Matrix Q, Matrix R, Matrix S, size_t N, size_t M) {
    Q_ = Q;
    // R_ = R; // Isn't used
    S_ = S;
    N_ = N;
    M_ = M;

    // Set up P matrix
    Matrix Px = arma::kron(Matrix(N_, N_, arma::fill::eye), Q_);
    Matrix Pu1 = arma::kron(Matrix(M_, M_, arma::fill::eye), 2 * S_ + R);
    Matrix Pu2 =
        arma::kron(Matrix(eye_offset(M)) + Matrix(eye_offset(M, 1)), -S_);
    Matrix Pu3 = block_diag(
        Matrix((M_ - 1) * m_, (M_ - 1) * m_, arma::fill::zeros), -S_);
    Matrix Pu = Pu1 + Pu2 + Pu3;
    P_ = Sparse(arma::trimatu(
        2 * block_diag(Px, Pu)));  // Taking only the upper triangular part
  }

  void set_constraint(Vector xmin, Vector xmax, Vector umin, Vector umax) {
    lineq_ = join_horiz(arma::kron(Vector(N_, arma::fill::ones), xmin).t(),
                        arma::kron(Vector(M_, arma::fill::ones), umin).t());
    uineq_ = join_horiz(arma::kron(Vector(N_, arma::fill::ones), xmax).t(),
                        arma::kron(Vector(M_, arma::fill::ones), umax).t());
    size_t Aineq_dim = N_ * n_ + M_ * m_;
    Aineq_ = arma::eye<Sparse>(Aineq_dim, Aineq_dim);
  }

  void Print() {
    sys_.Print();
    // TODO: Implement print
  }

 protected:
  System sys_;  ///< system being controlled
  size_t n_;    ///< number of states
  size_t m_;    ///< number of output states
  size_t N_;    ///< number of steps
  size_t M_;    ///< number of inputs
  Matrix A_;    ///< state transition matrix
  Matrix B_;    ///< input matrix
  Sparse P_;    ///< penalty matrix
  Matrix Q_;
  Matrix S_;

  Matrix lineq_;  ///< lower inequality bound
  Matrix uineq_;  ///< upper inequality bound
  Sparse Aineq_;  ///< inequality condition matrix

  Matrix Acon_;  ///< update condition matrix
  Vector lb_;    ///< lower bound
  Vector ub_;    ///< upper bound

  Vector xi_;     ///< previous step end state
  size_t t_sim_;  ///< previous step simulation time step

 private:
  /**
   * @brief     Calculate the trajectory for the simulation step,
   *            used when the simulation time step is the same as
   *            the prior step
   *
   * @param     x0  The initial state
   * @param     u0  The initial control input
   * @param     xr  The reference trajectory
   * @param     out Print out step information
   *
   * @return    The trajectory for the simulation step
   */
  osqp_arma::Solution* fast_update(const Vector& x0, const Vector& u0,
                                   const Matrix& xr, size_t n_sim);

  /**
   * @brief     Calculate the trajectory for the simulation step
   *
   * @param     x0  The initial state
   * @param     u0  The initial control input
   * @param     xr  The reference trajectory
   * @param     out Print out step information
   *
   * @return    The trajectory for the simulation step
   */
  osqp_arma::Solution* slow_update(const Vector& x0, const Vector& u0,
                                   const Matrix& xr, size_t n_sim);

  /**
   * @brief     Create an identity matrix with an offset axis
   *
   * @param     n The size of the matrix
   * @param     k The offset axis
   *
   * @return    The identity matrix with an offset axis
   */
  Sparse eye_offset(size_t n, int k = -1) {
    Sparse mat(n, n);
    mat.diag(k).ones();
    return mat;
  }

  /**
   * @brief     Create a block diagonal matrix given two input matrices
   *
   * @param     m1 The first matrix
   * @param     m2 The second matrix
   *
   * @return    The block diagonal matrix
   */
  Matrix block_diag(Matrix m1, Matrix m2) {
    // Calculate the total rows and columns of the block diagonal matrix
    size_t rows = m1.n_rows + m2.n_rows;
    size_t cols = m1.n_cols + m2.n_cols;

    // Create the block diagonal matrix
    Matrix bd = arma::zeros<Matrix>(rows, cols);
    bd.submat(0, 0, m1.n_rows - 1, m1.n_cols - 1) = m1;
    bd.submat(m1.n_rows, m1.n_cols, rows - 1, cols - 1) = m2;

    return bd;
  }

  void Init() {
    A_ = sys_.A();
    B_ = sys_.B();
    n_ = B_.n_rows;
    m_ = B_.n_cols;
    xi_ = arma::zeros<Vector>(n_);
    t_sim_ = 0;
  }
};

// Implement methods

template <typename System>
MpcController<System>::MpcController(const System& sys, Vector u_lb,
                                     Vector u_ub)
    : sys_(sys), lb_(u_lb), ub_(u_ub) {
  Init();
}

template <typename System>
MpcController<System>::MpcController(System&& sys, Vector u_lb, Vector u_ub)
    : sys_(std::move(sys)), lb_(u_lb), ub_(u_ub) {
  Init();
}

template <typename System>
Vector MpcController<System>::Control(data_t t_sim, const Vector& x0,
                                      const Vector& u0, const Matrix& xr) {
  size_t n_sim = t_sim / sys_.dt();  // Number of points per simulation step

  osqp_arma::Solution* sol;
  if (arma::all(xi_ == x0) && (t_sim_ == t_sim)) {
    sol = fast_update(x0, u0, xr, n_sim);
  } else {
    sol = slow_update(x0, u0, xr, n_sim);
  }
  t_sim_ = t_sim;

  Vector ui(m_);
  for (int i = 0; i < m_; i++) {
    ui(i) = sol->x(N_ * n_ + i);
  }

  if (sol) free(sol);

  return ui;
}

template <typename System>
osqp_arma::Solution* MpcController<System>::fast_update(const Vector& x0,
                                                        const Vector& u0,
                                                        const Matrix& xr,
                                                        size_t n_sim) {
  lb_.rows(0, n_ - 1) = -x0.t();
  ub_.rows(0, n_ - 1) = -x0.t();

  // Convert state penalty from reference to OSQP format
  arma::Row<data_t> q;
  {
    arma::uvec indices = arma::regspace<arma::uvec>(0, n_sim, N_ * n_sim - 1);
    Matrix sliced_xr = xr.cols(indices);
    Matrix Qxr_full = -2 * Q_ * sliced_xr;
    arma::Row<data_t> Qxr =
        Qxr_full.as_col().t();  // Qxr for every simulation time step

    arma::Row<data_t> qu =
        join_horiz(-2 * S_ * u0, arma::zeros<Vector>((M_ - 1) * m_));
    arma::Row<data_t> qx = Qxr.cols(0, N_ * n_ - 1);
    q = join_horiz(qx, qu);
  }

  osqp_arma::OSQP* OSQP = new osqp_arma::OSQP();

  // set settings
  OSQP->set_default_settings();
  OSQP->set_verbose(false);

  // set problem
  OSQP->setup(P_, q, Acon_, lb_, ub_);

  osqp_arma::Solution* sol = OSQP->solve();

  return sol;
}

template <typename System>
osqp_arma::Solution* MpcController<System>::slow_update(const Vector& x0,
                                                        const Vector& u0,
                                                        const Matrix& xr,
                                                        size_t n_sim) {
  Matrix leq = join_horiz(
      -x0.t(), arma::zeros((N_ - 1) * n_).t());  // Lower equality bound
  Matrix ueq = leq;                              // Upper equality bound

  // Update x over n_sim many steps
  Matrix Axs = arma::real(
      arma::powmat(A_, static_cast<double>(n_sim)));  // State multiplier
  Matrix Aus = arma::zeros(n_, n_);                   // Input multiplier
  for (int i = 0; i < n_sim; i++) {
    Aus += arma::powmat(A_, i);
  }

  // Ax + Bu = 0
  Matrix Ax(
      arma::kron(arma::speye<Sparse>(N_, N_), -arma::speye<Sparse>(n_, n_)) +
      arma::kron(eye_offset(N_), Sparse(Axs)));
  Matrix B0(1, M_);
  Matrix Bstep(M_, M_, arma::fill::eye);
  Matrix Bend = arma::join_horiz(Matrix(N_ - M_ - 1, M_ - 1),
                                 Matrix(N_ - M_ - 1, 1, arma::fill::ones));
  Matrix Bu = kron(join_vert(B0, Matrix(Bstep), Bend), Aus * B_);
  Matrix Aeq = join_horiz(Ax, Bu);  // Equality condition

  Acon_ = join_vert(Aeq, Matrix(Aineq_));  // Update condition
  lb_ = join_horiz(leq, lineq_).t();       // Lower bound
  ub_ = join_horiz(ueq, uineq_).t();       // Upper bound

  // Convert state penalty from reference to OSQP format
  Vector q;
  {
    arma::uvec indices = arma::regspace<arma::uvec>(0, n_sim, N_ * n_sim - 1);
    Matrix sliced_xr = xr.cols(indices);
    Matrix Qxr_full = -2 * Q_ * sliced_xr;
    Vector Qxr = Qxr_full.as_col();  // Qxr for every simulation time step

    Vector qu =
        join_vert((-2 * S_ * u0), Vector((M_ - 1) * m_, arma::fill::zeros));
    Vector qx = Qxr.rows(0, N_ * n_ - 1);
    q = join_vert(qx, qu);
  }

  osqp_arma::OSQP* OSQP = new osqp_arma::OSQP();

  // set settings
  OSQP->set_default_settings();
  OSQP->set_verbose(false);

  // set problem
  OSQP->setup(P_, q, Acon_, lb_, ub_);

  osqp_arma::Solution* sol = OSQP->solve();

  return sol;
}

}  // namespace lds

#endif