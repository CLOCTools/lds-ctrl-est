//===-- ldsCtrlEst_h/lds_gaussian_mpcctrl.h - MPC Controller ------*- C++
//-*-===//
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
#include "osqp.h"
#include "osqp_api_constants.h"

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

  // setters
  void set_control(Matrix Q, Matrix R, Matrix S, size_t N, size_t M) {
    Q_ = Q;
    // R_ = R; // Isn't used
    S_ = S;
    N_ = N;
    M_ = M;

    // Set up P matrix
    Matrix Px   = arma::kron(Matrix(N_, N_, arma::fill::eye), Q_);
    Matrix Pu1  = arma::kron(Matrix(M_, M_, arma::fill::eye), 2 * S_ + R);
    Matrix Pu2  = arma::kron(Matrix(eye_offset(M)) + Matrix(eye_offset(M, 1)), -S_);
    Matrix Pu3  = block_diag(Matrix((M_ - 1) * m_, (M_ - 1) * m_, arma::fill::zeros), -S_);
    Matrix Pu = Pu1 + Pu2 + Pu3;
    P_ = arma::trimatu(2 * block_diag(Px, Pu)); // Taking only the upper triangular part
  }

  void set_constraint(Vector xmin, Vector xmax, Vector umin, Vector umax) {
    lineq_ = join_horiz(arma::kron(Vector(N_, arma::fill::ones), xmin).t(),
                        arma::kron(Vector(M_, arma::fill::ones), umin).t());
    uineq_ = join_horiz(arma::kron(Vector(N_, arma::fill::ones), xmax).t(),
                        arma::kron(Vector(M_, arma::fill::ones), umax).t());
    size_t Aineq_dim = N_ * n_ + M_ * m_;
    Aineq_ = arma::eye<arma::SpMat<data_t>>(Aineq_dim, Aineq_dim);
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
  Matrix P_;    ///< penalty matrix
  Matrix Q_;
  Matrix S_;

  Matrix lineq_;               ///< lower inequality bound
  Matrix uineq_;               ///< upper inequality bound
  arma::SpMat<data_t> Aineq_;  ///< inequality condition matrix

  Matrix Acon_;  ///< update condition matrix
  Vector lb_;    ///< lower bound
  Vector ub_;    ///< upper bound

  Vector xi_;     ///< previous step end state
  size_t t_sim_;  ///< previous step simulation time step

 private:
  /**
   * @brief     Set the matrix for the OSQP solver from an Armadillo matrix
   *
   * @param     A The matrix to convert
   */
  OSQPCscMatrix* from_matrix(const Matrix& A);

  /**
   * @brief     Set the matrix for the OSQP solver from an Armadillo sparse
   * matrix
   *
   * @param     A The sparse matrix to convert
   */
  OSQPCscMatrix* from_sparse(const arma::SpMat<data_t>& A);

  /**
   * @brief     Set the vector for the OSQP solver from an Armadillo vector
   *
   * @param     v The vector to convert
   */
  OSQPFloat* from_vec(const Vector& v);

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
  OSQPSolution* fast_update(const Vector& x0, const Vector& u0,
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
  OSQPSolution* slow_update(const Vector& x0, const Vector& u0,
                            const Matrix& xr, size_t n_sim);

  /**
   * @brief     Create an identity matrix with an offset axis
   * 
   * @param     n The size of the matrix
   * @param     k The offset axis
   * 
   * @return    The identity matrix with an offset axis
   */
  arma::SpMat<data_t> eye_offset(size_t n, int k = -1) {
    arma::SpMat<data_t> mat(n, n);
    int start = (k < 0) ? -k : 0;
    for (int i = start; i < n && i + k < n; i++) {
      mat(i, i + k) = 1;
    }
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

  // void print_osqp_csc_matrix(const OSQPCscMatrix* matrix) {
  //   std::cout << "[" << matrix->m << ", " << matrix->n << "]:\n";

  //   for (int j = 0, k = 0; j < (matrix->p[matrix->n]); j++) {
  //     while (k <= matrix->n && matrix->p[k + 1] <= j) k++;
  //     std::cout << "(" << matrix->i[j]  << ", " << k << ") " << matrix->x[j] << "\n";
  //   }
  //   std::cout << std::endl;
  // }

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
inline MpcController<System>::MpcController(const System& sys, Vector u_lb,
                                            Vector u_ub)
    : sys_(sys), lb_(u_lb), ub_(u_ub) {
  Init();
}

template <typename System>
inline MpcController<System>::MpcController(System&& sys, Vector u_lb,
                                            Vector u_ub)
    : sys_(std::move(sys)), lb_(u_lb), ub_(u_ub) {
  Init();
}

template <typename System>
Vector MpcController<System>::Control(data_t t_sim, const Vector& x0,
                                      const Vector& u0, const Matrix& xr) {
  size_t n_sim = t_sim / sys_.dt();  // Number of points per simulation step

  OSQPSolution* sol;
  if (arma::all(xi_ == x0) && (t_sim_ == t_sim)) {
    sol = fast_update(x0, u0, xr, n_sim);
  } else {
    sol = slow_update(x0, u0, xr, n_sim);
  }
  t_sim_ = t_sim;

  Vector ui(m_);
  for (int i = 0; i < m_; i++) {
    ui(i) = sol->x[N_ * n_ + i];
  }

  if (sol) free(sol);

  return ui;
}

template <typename System>
OSQPSolution* MpcController<System>::fast_update(const Vector& x0,
                                                 const Vector& u0,
                                                 const Matrix& xr,
                                                 size_t n_sim) {
  lb_.rows(0, n_ - 1) = -x0.t();
  ub_.rows(0, n_ - 1) = -x0.t();

  // Convert state penalty from reference to OSQP format
  arma::Row<data_t> q_arma;
  {
    arma::uvec indices = arma::regspace<arma::uvec>(0, n_sim, N_ * n_sim - 1);
    Matrix sliced_xr = xr.cols(indices);
    Matrix Qxr_full = -2 * Q_ * sliced_xr;
    arma::Row<data_t> Qxr =
        Qxr_full.as_col().t();  // Qxr for every simulation time step

    arma::Row<data_t> qu =
        join_horiz(-2 * S_ * u0, arma::zeros<Vector>((M_ - 1) * m_));
    arma::Row<data_t> qx = Qxr.cols(0, N_ * n_ - 1);
    q_arma = join_horiz(qx, qu);
  }

  // Solver, settings, matrices
  OSQPSolver* solver;
  OSQPSettings* settings;
  OSQPCscMatrix* A = from_matrix(Acon_);
  OSQPCscMatrix* P = from_matrix(P_);
  OSQPFloat* q = from_vec(q_arma.t());
  OSQPFloat* lb = from_vec(lb_);
  OSQPFloat* ub = from_vec(ub_);

  // Set settings
  settings = (OSQPSettings*)malloc(sizeof(OSQPSettings));
  if (settings) {
    osqp_set_default_settings(settings);
    settings->verbose = false;
  }

  // Set up solver
  OSQPInt exit_flag = osqp_setup(&solver, P, q, A, lb, ub, A->m, A->n, settings); // n and m are set by A cols and rows

  if (solver->info->status_val != OSQP_SOLVED) {
    std::string error_message = "OSQP solver failed to solve.";
    throw std::runtime_error(error_message);
  }

  // Solve problem
  if (!exit_flag) exit_flag = osqp_solve(solver);

  OSQPSolution* sol = std::move(solver->solution);

  // Clean up
  osqp_cleanup(solver);
  if (q) free(q);
  if (lb) free(lb);
  if (ub) free(ub);
  if (A) free(A);
  if (P) free(P);
  if (settings) free(settings);

  return sol;
}

template <typename System>
OSQPSolution* MpcController<System>::slow_update(const Vector& x0,
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
  Matrix Ax(arma::kron(arma::speye<arma::SpMat<data_t>>(N_, N_),
                      -arma::speye<arma::SpMat<data_t>>(n_, n_)) +
            arma::kron(eye_offset(N_), arma::SpMat<data_t>(Axs)));
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
  Vector q_arma;
  {
    arma::uvec indices = arma::regspace<arma::uvec>(0, n_sim, N_ * n_sim - 1);
    Matrix sliced_xr = xr.cols(indices);
    Matrix Qxr_full = -2 * Q_ * sliced_xr;
    Vector Qxr = Qxr_full.as_col();  // Qxr for every simulation time step

    Vector qu =
        join_vert((-2 * S_ * u0), Vector((M_ - 1) * m_, arma::fill::zeros));
    Vector qx = Qxr.rows(0, N_ * n_ - 1);
    q_arma = join_vert(qx, qu);

  }

  // Problem values for OSQP
  OSQPFloat* q = from_vec(q_arma);
  OSQPFloat* lb = from_vec(lb_);
  OSQPFloat* ub = from_vec(ub_);

  // Solver, settings, matrices
  OSQPSolver* solver;
  OSQPSettings* settings;
  OSQPCscMatrix* A = from_matrix(Acon_);
  OSQPCscMatrix* P = from_matrix(P_);

  // Set settings
  settings = (OSQPSettings*)malloc(sizeof(OSQPSettings));
  if (settings) {
    osqp_set_default_settings(settings);
    settings->verbose = false;
    settings->warm_starting = false;
  }

  // Set up solver
  OSQPInt exit_flag = osqp_setup(&solver, P, q, A, lb, ub, A->m, A->n, settings); // n and m are set by A cols and rows

  // Solve problem
  if (!exit_flag) exit_flag = osqp_solve(solver);

  if (solver->info->status_val != OSQP_SOLVED) {
    std::string error_message = "OSQP solver failed to solve.";
    throw std::runtime_error(error_message);
  }

  OSQPSolution* sol = std::move(solver->solution);

  // Clean up
  osqp_cleanup(solver);
  if (q) free(q);
  if (lb) free(lb);
  if (ub) free(ub);
  if (A->i) free(A->i);
  if (A->p) free(A->p);
  if (A->x) free(A->x);
  if (A) free(A);
  if (P->i) free(P->i);
  if (P->p) free(P->p);
  if (P->x) free(P->x);
  if (P) free(P);
  if (settings) free(settings);

  return sol;
}

template <typename System>
OSQPCscMatrix* MpcController<System>::from_matrix(const Matrix& A) {
  OSQPCscMatrix* mat = (OSQPCscMatrix*)malloc(sizeof(OSQPCscMatrix));

  mat->m = A.n_rows;
  mat->n = A.n_cols;
  mat->nzmax = A.n_rows * A.n_cols;  // Can be made smaller if needed
  mat->nz = -1;  // -1 means the matrix is in CSC format (required for API)

  mat->p = (OSQPInt*)malloc((mat->n + 1) * sizeof(OSQPInt));
  mat->i = (OSQPInt*)malloc(mat->nzmax * sizeof(OSQPInt));
  mat->x = (OSQPFloat*)malloc(mat->nzmax * sizeof(OSQPFloat));
  
  if (!mat->p || !mat->i || !mat->x) {
    std::string error_message = "Failed to allocate memory for OSQP matrix.";
    throw std::runtime_error(error_message);
  }

  OSQPInt n = 0;
  mat->p[0] = 0;
  for (OSQPInt j = 0; j < mat->n; j++) {
    for (OSQPInt i = 0; i < mat->m; i++) {
      if (A(i, j) != 0) {
        mat->i[n] = i;
        mat->x[n] = A(i, j);
        n++;
      }
    }
    mat->p[j + 1] = n;
  }

  return mat;
}

template <typename System>
OSQPCscMatrix* MpcController<System>::from_sparse(
    const arma::SpMat<data_t>& A) {
  OSQPCscMatrix* mat = (OSQPCscMatrix*)malloc(sizeof(OSQPCscMatrix));

  mat->m = A.n_rows;
  mat->n = A.n_cols;
  mat->nzmax = A.n_nonzero;
  mat->nz = -1;  // -1 means the matrix is in CSC format (required for API)

  mat->p = (OSQPInt*)malloc((mat->n + 1) * sizeof(OSQPInt));
  mat->i = (OSQPInt*)malloc(mat->nzmax * sizeof(OSQPInt));
  mat->x = (OSQPFloat*)malloc(mat->nzmax * sizeof(OSQPFloat));
  
  if (!mat->p || !mat->i || !mat->x) {
    std::string error_message = "Failed to allocate memory for OSQP matrix.";
    throw std::runtime_error(error_message);
  }

  arma::sp_mat::iterator it = A.begin();
  for (OSQPInt j = 0; it != A.end(); ++it, ++j) {
    mat->i[j] = it.row();
    mat->x[j] = *it;
    mat->p[it.col() + 1] = j;
  }

  mat->p[0] = 0;
  for (OSQPInt j = 0; j < mat->n; j++) {
    // Set values for all empty columns
    if (mat->p[j + 1] == 0) {
      mat->p[j + 1] = mat->p[j];
    }
  }
  mat->p[mat->n] = mat->nzmax;

  return mat;
}

template <typename System>
OSQPFloat* MpcController<System>::from_vec(const Vector& v) {
  OSQPFloat* arr = (OSQPFloat*)malloc(v.n_elem * sizeof(OSQPFloat));

  for (OSQPInt i = 0; i < v.n_elem; i++) {
    arr[i] = v(i);
  }

  return arr;
}

}  // namespace lds

#endif