//===-- ldsCtrlEst_h/osqp_arma.h - OSQP/armadillo wrapper ---------*- C++
//-*-===//
//
// Copyright 2024 Aaron Hung
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
/// This file defines a wrapper class for the osqp solver that uses
/// armadillo matrices and vectors for some methods that are used in
/// lds_mpcctrl.h
///
/// \brief osqp/armadillo partial wrapper class
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_OSQP_ARMA_H
#define LDSCTRLEST_OSQP_ARMA_H

#include <armadillo>

#include "osqp.h"
#include "osqp_api_constants.h"

namespace osqp_arma {

/**
 * @brief Converts an OSQP CSC matrix to an Armadillo dense matrix.
 *
 * @param osqpMatrix Pointer to the OSQP CSC matrix.
 *
 * @return Armadillo dense matrix.
 */
arma::mat to_matrix(OSQPCscMatrix* osqpMatrix) {
  arma::mat result(osqpMatrix->m, osqpMatrix->n, arma::fill::zeros);
  for (OSQPInt col = 0; col < osqpMatrix->n; ++col) {
    for (OSQPInt idx = osqpMatrix->p[col]; idx < osqpMatrix->p[col + 1];
         ++idx) {
      result(osqpMatrix->i[idx], col) = osqpMatrix->x[idx];
    }
  }
  return result;
}

/**
 * @brief Converts an OSQP CSC matrix to an Armadillo sparse matrix.
 *
 * @param osqpMatrix Pointer to the OSQP CSC matrix.
 *
 * @return Armadillo sparse matrix.
 */
arma::sp_mat to_sparse(OSQPCscMatrix* osqpMatrix) {
  arma::sp_mat result(osqpMatrix->m, osqpMatrix->n);
  for (OSQPInt col = 0; col < osqpMatrix->n; ++col) {
    for (OSQPInt idx = osqpMatrix->p[col]; idx < osqpMatrix->p[col + 1];
         ++idx) {
      result(osqpMatrix->i[idx], col) = osqpMatrix->x[idx];
    }
  }
  return result;
}

/**
 * @brief Converts an OSQP float array to an Armadillo vector.
 *
 * @param osqpFloatArr Pointer to the OSQP float array.
 *
 * @return Armadillo vector.
 */
arma::vec to_vector(OSQPFloat* osqpFloatArr, OSQPInt size) {
  arma::vec result(size);
  for (OSQPInt i = 0; i < size; ++i) {
    result[i] = osqpFloatArr[i];
  }
  return result;
}

/**
 * @brief Converts an Armadillo dense matrix to an OSQP CSC matrix.
 *
 * @param armaMatrix Armadillo dense matrix.
 *
 * @return Pointer to the converted OSQP CSC matrix.
 */
OSQPCscMatrix* from_matrix(arma::mat armaMatrix) {
  OSQPInt m = armaMatrix.n_rows;
  OSQPInt n = armaMatrix.n_cols;

  std::vector<OSQPInt> i_vec;
  std::vector<OSQPFloat> x_vec;
  std::vector<OSQPInt> p_vec(n + 1);

  OSQPInt nnz = 0;
  for (OSQPInt col = 0; col < n; ++col) {
    p_vec[col] = nnz;
    for (OSQPInt row = 0; row < m; ++row) {
      OSQPFloat value = armaMatrix(row, col);
      if (value != 0.0) {
        i_vec.push_back(row);
        x_vec.push_back(value);
        ++nnz;
      }
    }
  }
  p_vec[n] = nnz;

  OSQPCscMatrix* osqpMatrix = new OSQPCscMatrix;
  osqpMatrix->m = m;
  osqpMatrix->n = n;
  osqpMatrix->nzmax = nnz;
  osqpMatrix->nz = -1;
  osqpMatrix->x = new OSQPFloat[nnz];
  osqpMatrix->i = new OSQPInt[nnz];
  osqpMatrix->p = new OSQPInt[n + 1];

  std::copy(x_vec.begin(), x_vec.end(), osqpMatrix->x);
  std::copy(i_vec.begin(), i_vec.end(), osqpMatrix->i);
  std::copy(p_vec.begin(), p_vec.end(), osqpMatrix->p);

  return osqpMatrix;
}

/**
 * @brief Converts an Armadillo sparse matrix to an OSQP CSC matrix.
 *
 * @param armaSparse Armadillo sparse matrix.
 *
 * @return Pointer to the converted OSQP CSC matrix.
 */
OSQPCscMatrix* from_sparse(arma::sp_mat armaSparse) {
  OSQPInt m = armaSparse.n_rows;
  OSQPInt n = armaSparse.n_cols;
  OSQPInt nnz = armaSparse.n_nonzero;

  OSQPCscMatrix* osqpMatrix = new OSQPCscMatrix;
  osqpMatrix->m = m;
  osqpMatrix->n = n;
  osqpMatrix->nzmax = nnz;
  osqpMatrix->nz = -1;
  osqpMatrix->x = new OSQPFloat[nnz];
  osqpMatrix->i = new OSQPInt[nnz];
  osqpMatrix->p = new OSQPInt[n + 1];

  OSQPInt idx = 0;
  for (OSQPInt col = 0; col < n; ++col) {
    osqpMatrix->p[col] = idx;
    for (auto it = armaSparse.begin_col(col); it != armaSparse.end_col(col);
         ++it) {
      osqpMatrix->i[idx] = it.row();
      osqpMatrix->x[idx] = *it;
      ++idx;
    }
  }
  osqpMatrix->p[n] = idx;

  return osqpMatrix;
}

/**
 * @brief Converts an Armadillo vector to an OSQP float array.
 *
 * @param armaVector Armadillo vector.
 *
 * @return Pointer to the converted OSQP float array.
 */
OSQPFloat* from_vector(arma::vec armaVector) {
  OSQPInt size = armaVector.n_elem;
  OSQPFloat* osqpFloatArr = new OSQPFloat[size];
  for (OSQPInt i = 0; i < size; ++i) {
    osqpFloatArr[i] = armaVector[i];
  }
  return osqpFloatArr;
}

class Solution {
 public:
  Solution(OSQPSolution* sol, OSQPInfo* info, int n, int m) {
    x_ = to_vector(sol->x, n);
    y_ = to_vector(sol->y, m);
    prim_inf_cert_ = to_vector(sol->prim_inf_cert, n);
    dual_inf_cert_ = to_vector(sol->dual_inf_cert, n);
    obj_val_ = info->obj_val;
  }

  /**
   * @brief get the primal solution
   *
   * @return primal solution
   */
  arma::vec* x() { return &x_; }

  double x(int i) { return x_(i); }

  /**
   * @brief get the Lagrange multiplier
   *
   * @return Lagrange multiplier associated with Ax
   */
  arma::vec* y() { return &y_; }

  double y(int i) { return y_(i); }

  /**
   * @brief get the primal infeasibility certificate
   *
   * @return Primal infeasibility certificate
   */
  arma::vec* prim_inf_cert() { return &prim_inf_cert_; }

  /**
   * @brief get the dual infeasibility certificate
   *
   * @return Dual infeasibility certificate
   */
  arma::vec* dual_inf_cert() { return &dual_inf_cert_; }

  double obj_val() { return obj_val_; }

 protected:
  arma::vec x_;
  arma::vec y_;
  arma::vec prim_inf_cert_;
  arma::vec dual_inf_cert_;
  double obj_val_;
};  // class Solution

class OSQP {
 public:
  OSQP() {
    solver = new OSQPSolver();
    settings = new OSQPSettings();
  }

  /**
   * @brief set up the problem
   *
   * @param P Problem data (upper triangular part of quadratic cost term, csc
   * format)
   * @param q Problem data (linear cost term)
   * @param A Problem data (constraint matrix, csc format)
   * @param l Problem data (constraint lower bound)
   * @param u Problem data (constraint upper bound)
   * @param m Problem data (number of constraints)
   * @param n Problem data (number of variables)
   */
  void setup(arma::sp_mat P, arma::vec q, arma::mat A, arma::vec l,
             arma::vec u) {
    P_ = from_sparse(P);
    q_ = from_vector(q);
    A_ = from_matrix(A);
    l_ = from_vector(l);
    u_ = from_vector(u);
    m_ = A_->m;
    n_ = A_->n;
    updated_problem_ = true;
  }

  /**
   * @brief solve the problem
   *
   * @return Solution containing the optimal trajectory
   */
  Solution* solve() {
    if (updated_problem_ || updated_settings_) {
      OSQPInt exitflag =
          osqp_setup(&solver, P_, q_, A_, l_, u_, m_, n_, settings);
      if (exitflag != 0) {
        throw std::runtime_error("osqp_setup failed with exit flag: " +
                                 std::to_string(exitflag));
      }
      updated_problem_ = false;
      updated_settings_ = false;
      updated_data_ = false;
    } else if (updated_data_) {
      OSQPInt exitflag = osqp_update_data_vec(solver, q_, NULL, NULL);
      if (exitflag != 0) {
        throw std::runtime_error(
            "osqp_update_data_vec failed with exit flag: " +
            std::to_string(exitflag));
      }
    }

    OSQPInt exitflag = osqp_solve(solver);
    if (exitflag != 0) {
      throw std::runtime_error("osqp_solve failed with exit flag: " +
                               std::to_string(exitflag));
    }

    return new Solution(solver->solution, solver->info, n_, m_);
  }

  /**
   * @brief set the settings of the solver to the default settings
   */
  void set_default_settings() {
    osqp_set_default_settings(settings);
    updated_settings_ = true;
  }

  /**
   * @brief set the settings of the solver
   *
   * @param newSettings new settings to assign to solver settings
   */
  void set_settings(OSQPSettings* newSettings) {
    std::memcpy(settings, newSettings, sizeof(OSQPSettings));
    updated_settings_ = true;
  }

  void set_P(arma::sp_mat P) {
    P_ = from_sparse(P);
    updated_problem_ = true;
  }

  void set_q(arma::vec q) {
    q_ = from_vector(q);
    updated_data_ = true;
  }

  void set_A(arma::mat A) {
    A_ = from_matrix(A);
    n_ = A_->n;
    m_ = A_->m;
    updated_problem_ = true;
  }

  void set_l(arma::vec l) {
    l_ = from_vector(l);
    updated_problem_ = true;
  }

  void set_u(arma::vec u) {
    u_ = from_vector(u);
    updated_problem_ = true;
  }

  void set_m(OSQPInt m) {
    m_ = m;
    updated_problem_ = true;
  }

  void set_n(OSQPInt n) {
    n_ = n;
    updated_problem_ = true;
  }

  void set_device(OSQPInt device) {
    settings->device = device;
    updated_settings_ = true;
  }

  void set_linsys_solver(osqp_linsys_solver_type linsys_solver) {
    settings->linsys_solver = linsys_solver;
    updated_settings_ = true;
  }

  void set_allocate_solution(OSQPInt allocate_solution) {
    settings->allocate_solution = allocate_solution;
    updated_settings_ = true;
  }

  void set_verbose(OSQPInt verbose) {
    settings->verbose = verbose;
    updated_settings_ = true;
  }

  void set_profiler_level(OSQPInt profiler_level) {
    settings->profiler_level = profiler_level;
    updated_settings_ = true;
  }

  void set_warm_starting(OSQPInt warm_starting) {
    settings->warm_starting = warm_starting;
    updated_settings_ = true;
  }

  void set_scaling(OSQPInt scaling) {
    settings->scaling = scaling;
    updated_settings_ = true;
  }

  void set_polishing(OSQPInt polishing) {
    settings->polishing = polishing;
    updated_settings_ = true;
  }

  void set_rho(OSQPFloat rho) {
    settings->rho = rho;
    updated_settings_ = true;
  }

  void set_rho_is_vec(OSQPInt rho_is_vec) {
    settings->rho_is_vec = rho_is_vec;
    updated_settings_ = true;
  }

  void set_sigma(OSQPFloat sigma) {
    settings->sigma = sigma;
    updated_settings_ = true;
  }

  void set_alpha(OSQPFloat alpha) {
    settings->alpha = alpha;
    updated_settings_ = true;
  }

  void set_cg_max_iter(OSQPInt cg_max_iter) {
    settings->cg_max_iter = cg_max_iter;
    updated_settings_ = true;
  }

  void set_cg_tol_reduction(OSQPInt cg_tol_reduction) {
    settings->cg_tol_reduction = cg_tol_reduction;
    updated_settings_ = true;
  }

  void set_cg_tol_fraction(OSQPFloat cg_tol_fraction) {
    settings->cg_tol_fraction = cg_tol_fraction;
    updated_settings_ = true;
  }

  void set_cg_precond(osqp_precond_type cg_precond) {
    settings->cg_precond = cg_precond;
    updated_settings_ = true;
  }

  void set_adaptive_rho(OSQPInt adaptive_rho) {
    settings->adaptive_rho = adaptive_rho;
    updated_settings_ = true;
  }

  void set_adaptive_rho_interval(OSQPInt adaptive_rho_interval) {
    settings->adaptive_rho_interval = adaptive_rho_interval;
    updated_settings_ = true;
  }

  void set_adaptive_rho_fraction(OSQPFloat adaptive_rho_fraction) {
    settings->adaptive_rho_fraction = adaptive_rho_fraction;
    updated_settings_ = true;
  }

  void set_adaptive_rho_tolerance(OSQPFloat adaptive_rho_tolerance) {
    settings->adaptive_rho_tolerance = adaptive_rho_tolerance;
    updated_settings_ = true;
  }

  void set_max_iter(OSQPInt max_iter) {
    settings->max_iter = max_iter;
    updated_settings_ = true;
  }

  void set_eps_abs(OSQPFloat eps_abs) {
    settings->eps_abs = eps_abs;
    updated_settings_ = true;
  }

  void set_eps_rel(OSQPFloat eps_rel) {
    settings->eps_rel = eps_rel;
    updated_settings_ = true;
  }

  void set_eps_prim_inf(OSQPFloat eps_prim_inf) {
    settings->eps_prim_inf = eps_prim_inf;
    updated_settings_ = true;
  }

  void set_eps_dual_inf(OSQPFloat eps_dual_inf) {
    settings->eps_dual_inf = eps_dual_inf;
    updated_settings_ = true;
  }

  void set_scaled_termination(OSQPInt scaled_termination) {
    settings->scaled_termination = scaled_termination;
    updated_settings_ = true;
  }

  void set_check_termination(OSQPInt check_termination) {
    settings->check_termination = check_termination;
    updated_settings_ = true;
  }

  void set_time_limit(OSQPFloat time_limit) {
    settings->time_limit = time_limit;
    updated_settings_ = true;
  }

  void set_delta(OSQPFloat delta) {
    settings->delta = delta;
    updated_settings_ = true;
  }

  void set_polish_refine_iter(OSQPInt polish_refine_iter) {
    settings->polish_refine_iter = polish_refine_iter;
    updated_settings_ = true;
  }

 protected:
  OSQPSolver* solver;
  OSQPCscMatrix* P_;  // Problem data (upper triangular part of quadratic cost
                      // term, csc format)
  OSQPFloat* q_;      // Problem data (linear cost term)
  OSQPCscMatrix* A_;  // Problem data (constraint matrix, csc format)
  OSQPFloat* l_;      // Problem data (constraint lower bound)
  OSQPFloat* u_;      // Problem data (constraint upper bound)
  OSQPInt m_;         // Problem data (number of constraints)
  OSQPInt n_;         // Problem data (number of variables)
  bool updated_problem_ = true;  // whether the problem has been updated
  bool updated_data_ = true;     // whether the data has been updated

  OSQPSettings* settings;
  bool updated_settings_ = true;  // whether any settings have been updated
};  // class OSQP
}  // namespace osqp_arma

#endif  // LDSCTRLEST_OSQP_ARMA_H
