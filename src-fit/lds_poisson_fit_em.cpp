//===-- lds_poisson_fit_em.cpp - PLDS Fit (EM) ----------------------------===//
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
/// This file partially defines a type by which Gaussian-output LDS models are
/// fit by the expectation-maximization (EM) algorithm
/// (`lds::gaussian::emFit_t`).
///
/// References:
/// [1] Shumway RH, Stoffer DS. (1982) An Approach to Time Series Smoothing and
/// Forecasting Using the EM Algorithm.Journal of Time Series Analysis 3(2).
///
/// [2] Ghahramani Z, Hinton GE. (1996) Parameter Estimation for Linear
/// Dynamical Systems. Technical Report CRG-TR-96-2.
///
/// [3] Smith A, Brown E. (2003) Estimating a State-Space Model from Point
/// Process Observations. Neural Computation.
///
/// \brief PLDS E-M fit type
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>

namespace lds {
namespace poisson {

void FitEM::MaximizeOutput() {
  data_t tol = 1e-2;           // frac abs change
  size_t iters_allowed = 100;  // how many iterations allowed for convergence

  data_t nll(0);
  data_t nll_prev(-1);

  Vector d_new = fit_.d();
  Matrix c_new = fit_.C();

  Vector theta = join_vert(d_new, vectorise(c_new));
  Vector theta_new = theta;

  data_t crit(0);
  data_t n_events(0);
  for (size_t k = 0; k < z_.size(); k++) {             // trial loop
    for (size_t kk = 0; kk < z_.at(k).n_cols; kk++) {  // time samp loop
      n_events += sum(z_.at(k).col(kk));
    }
  }

  // loop through multiple intereations (l)...
  for (size_t l = 0; l <= iters_allowed; l++) {
    theta = join_vert(fit_.d(), vectorise(fit_.C()));
    nll_prev = nll;

    // joint iterations.
    AnalyticalSolveD();
    nll = NewtonSolveC();
    std::cout << "-logL = " << (nll / n_events) << "\n";
    if ((l > 0) && ((nll - nll_prev) > 0)) {
      break;
    }

    theta_new = join_vert(fit_.d(), vectorise(fit_.C()));
    crit = max(abs(theta - theta_new) / abs(theta));

    if (crit < tol) {
      // std::cout << "MaximizeOutput converged.\n";
      break;
    }
    if (l == (iters_allowed - 1)) {
      std::cerr << "MaximizeOutput failed to converge.\n";
    }
  }  // iterations loop
  std::cout << "C_new[0]: " << fit_.C()[0] << "\n";
  std::cout << "d_new[0]: " << fit_.d()[0] << "\n";
}

void FitEM::RecurseKe(Matrix& Ke, Cube& P_pre, Cube& P_post, size_t t) {
  // predict cov
  P_pre.slice(t) = fit_.A() * P_post.slice(t - 1) * fit_.A().t() + fit_.Q();
  // update cov
  P_post.slice(t) =
      inv_sympd(inv_sympd(P_pre.slice(t)) + fit_.C().t() * diag_y_ * fit_.C());
  // update Ke
  Ke = P_post.slice(t) * fit_.C().t();
}

void FitEM::AnalyticalSolveD() {
  Vector sum_r(n_y_, fill::zeros);
  Vector sum_events(n_y_, fill::zeros);
  Vector logr(n_y_, fill::zeros);
  Vector cpc(n_y_, fill::zeros);

  for (size_t k = 0; k < x_.size(); k++) {
    sum_events += sum(z_.at(k), 1);
    for (size_t kk = 0; kk < x_[k].n_cols; kk++) {
      logr = fit_.C() * x_[k].col(kk);
      for (size_t p = 0; p < n_y_; p++) {
        auto c_p = fit_.C().submat(p, 0, p, n_x_ - 1);
        cpc[p] = (c_p * P_[k].slice(kk) * c_p.t() / 2)[0];
      }
      logr += cpc;
      Limit(logr, -50, 50);
      sum_r += exp(logr);
    }
  }

  Vector d = log(sum_events) - log(sum_r);
  Limit(d, -7, 7);
  fit_.set_d(d);
}

// TODO(mfbolus): Not sure this is correct! See note below.
data_t FitEM::NewtonSolveC() {
  data_t tol = 1e-2;  // frac abs change
  size_t iters_allowed =
      100;  // how many iterations of newtons allowed for convergence
  Vector nll(n_y_, fill::zeros);

  Vector f(n_x_, fill::zeros);
  Matrix fprime(n_x_, n_x_, fill::zeros);
  Matrix f_over_fprime(1, n_x_, fill::zeros);
  data_t crit(0);

  Matrix c = fit_.C();

  // Since they are independent, conditioned on state,
  // solve output-by-output (p)
  for (size_t p = 0; p < n_y_; p++) {  // output loop
    auto c_p = c.submat(p, 0, p, c.n_cols - 1);
    Vector c_p_new = vectorise(c_p);

    // loop through multiple intereations (l)...
    for (size_t l = 0; l <= iters_allowed; l++) {
      f.zeros();
      fprime.zeros();

      for (size_t k = 0; k < x_.size(); k++) {          // trial loop
        for (size_t kk = 0; kk < x_[0].n_cols; kk++) {  // time samp loop
          // TODO(mfbolus): not sure this is correct!
          // From a version of EM implementation written years ago, and cannot
          // tell if these expectations are correct
          Vector cx_cpc =
              c_p * x_[k].col(kk) + c_p * P_[k].slice(kk) * c_p.t() / 2;
          f += exp(fit_.d()[p] + cx_cpc)[0] *
                   (x_[k].col(kk) + P_[k].slice(kk) * c_p.t()) -
               z_.at(k).col(kk)[p] * x_[k].col(kk);

          Matrix tmp = P_[k].slice(kk) +
                       (x_[k].col(kk) + P_[k].slice(kk) * c_p.t()) *
                           (x_[k].col(kk) + P_[k].slice(kk) * c_p.t()).t();
          fprime += exp(fit_.d()[p] + cx_cpc)[0] * tmp;
        }  // time
      }    // trial
      f_over_fprime = inv(fprime) * f;
      // f_over_fprime = solve(fprime,f);
      // optionally, could change the step size if there are numerical issues
      // f_over_fprime *= 0.1;//0.05;//0.1;
      c_p_new -= f_over_fprime;
      crit = max(abs(c_p - c_p_new.t()) / abs(c_p));
      c_p = c_p_new.t();  // assign to old val.
      if (crit < tol) {
        break;
      }
      if (l == iters_allowed) {
        std::cerr << "NewtonSolveC failed to converge for output " << p + 1
                  << ".\n";
      }
    }  // iterations loop

    // calculate likelihood
    nll[p] = 0;
    Vector cx;
    for (size_t k = 0; k < x_.size(); k++) {          // trial loop
      for (size_t kk = 0; kk < x_[k].n_cols; kk++) {  // time samp loop
        cx = c_p * x_[k].col(kk);
        nll[p] += exp(fit_.d()[p] + cx[0]) -
                  z_.at(k).col(kk)[p] * (fit_.d()[p] + cx[0]);
      }  // time
    }    // trial
  }      // outputs loop

  fit_.set_C(c);
  return (arma::sum(nll));
}

}  // namespace poisson
}  // namespace lds
