---
title: ldsCtrlEst_h/lds_fit_em.h
summary: subspace identification 

---

# ldsCtrlEst_h/lds_fit_em.h

subspace identification  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/lds-ctrl-est/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::EM](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/)**  |

## Detailed Description



This file declares the type for fitting a linear dynamical system by expectation-maximization ([lds::EM](/lds-ctrl-est/docs/api/classes/classlds_1_1_e_m/)). 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_fit_em.h - EM Fit ----------------------*- C++ -*-===//
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
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_EMAX_H
#define LDSCTRLEST_LDS_EMAX_H

#include "lds_fit.h"

namespace lds {

template <typename Fit>
class EM {
  static_assert(std::is_base_of<lds::Fit, Fit>::value,
                "Fit must be derived from lds::Fit type.");

 public:
  EM() = default;

  EM(size_t n_x, data_t dt, UniformMatrixList<kMatFreeDim2>&& u_train,
     UniformMatrixList<kMatFreeDim2>&& z_train);

  EM(const Fit& fit0, UniformMatrixList<kMatFreeDim2>&& u_train,
     UniformMatrixList<kMatFreeDim2>&& z_train);

  const Fit& Run(bool calc_dynamics = true, bool calc_Q = true,
                 bool calc_init = true, bool calc_output = true,
                 bool calc_measurement = true, size_t max_iter = 100,
                 data_t tol = 1e-2);

  std::tuple<UniformMatrixList<kMatFreeDim2>, UniformMatrixList<kMatFreeDim2>>
  ReturnData() {
    auto tuple = std::make_tuple(std::move(u_), std::move(z_));
    // auto tuple = std::make_tuple(u_, z_);
    u_ = UniformMatrixList<kMatFreeDim2>();
    z_ = UniformMatrixList<kMatFreeDim2>();
    return tuple;
  }

  const std::vector<Matrix>& x() const { return x_; };
  const std::vector<Matrix>& y() const { return y_; };

  const Matrix& sum_E_x_t_x_t() const { return sum_E_x_t_x_t_; };
  const Matrix& sum_E_xu_tm1_xu_tm1() const { return sum_E_xu_tm1_xu_tm1_; };
  const Matrix& sum_E_xu_t_xu_tm1() const { return sum_E_xu_t_xu_tm1_; };
  size_t n_t_tot() { return n_t_tot_; }

  const Vector& theta() const { return theta_; };

 protected:
  void Expectation(bool force_common_initial = false);

  void Maximization(bool calc_dynamics = true, bool calc_Q = true,
                    bool calc_init = false, bool calc_output = false,
                    bool calc_measurement = false);

  void MaximizeDynamics();
  void MaximizeQ();
  void MaximizeInitial();
  virtual void MaximizeOutput() = 0;
  virtual void MaximizeMeasurement() = 0;

  void Smooth(bool force_common_initial);

  virtual void RecurseKe(Matrix& Ke, Cube& P_pre, Cube& P_post, size_t t) = 0;

  void Reset();

  void InitVars();

  Vector UpdateTheta();

  // input/output training data
  UniformMatrixList<kMatFreeDim2> u_;  
  UniformMatrixList<kMatFreeDim2> z_;  
  std::vector<Matrix> x_;              
  std::vector<Cube> P_;                
  std::vector<Cube> P_t_tm1_;          
  std::vector<Matrix> y_;              
  Matrix diag_y_;

  // expectations calculated in E-step
  Matrix sum_E_x_t_x_t_;        
  Matrix sum_E_xu_tm1_xu_tm1_;  
  Matrix sum_E_xu_t_xu_tm1_;    

  Fit fit_;
  Vector theta_;

  data_t dt_{};              
  size_t n_u_{};             
  size_t n_x_{};             
  size_t n_y_{};             
  size_t n_trials_{};        
  std::vector<size_t> n_t_;  
  size_t n_t_tot_{};         
};

template <typename Fit>
EM<Fit>::EM(size_t n_x, data_t dt, UniformMatrixList<kMatFreeDim2>&& u_train,
            UniformMatrixList<kMatFreeDim2>&& z_train) {
  n_u_ = u_train.at(0).n_rows;
  n_y_ = z_train.at(0).n_rows;
  fit_ = Fit(n_u_, n_x, n_y_, dt);
  u_ = std::move(u_train);
  z_ = std::move(z_train);
  InitVars();
}

template <typename Fit>
EM<Fit>::EM(const Fit& fit0, UniformMatrixList<kMatFreeDim2>&& u_train,
            UniformMatrixList<kMatFreeDim2>&& z_train) {
  // make sure fit dims match I/O data
  if (fit0.n_u() != u_train.at(0).n_rows) {
    throw std::runtime_error(
        "Initial fit and input training data have inconsistent dimensions");
  }
  if (fit0.n_y() != z_train.at(0).n_rows) {
    throw std::runtime_error(
        "Initial fit and output training data have inconsistent dimensions");
  }

  fit_ = fit0;
  u_ = std::move(u_train);
  z_ = std::move(z_train);

  InitVars();
}

template <typename Fit>
void EM<Fit>::InitVars() {
  // check input/output data dimensions are consistent
  if (z_.size() != u_.size()) {
    throw std::runtime_error(
        "I/O training data have different number of trials.");
  }
  n_trials_ = u_.size();

  n_t_tot_ = 0;
  n_t_ = std::vector<size_t>(n_trials_);
  for (size_t trial = 0; trial < n_trials_; trial++) {
    if (z_.at(trial).n_cols != u_.at(trial).n_cols) {
      throw std::runtime_error(
          "I/O training data have different number of time steps.");
    }
    n_t_[trial] = u_.at(trial).n_cols;
    n_t_tot_ += n_t_[trial];
  }

  n_u_ = fit_.n_u();
  n_x_ = fit_.n_x();
  n_y_ = fit_.n_y();
  dt_ = fit_.dt();

  x_ = std::vector<Matrix>(n_trials_);
  P_ = std::vector<Cube>(n_trials_);
  P_t_tm1_ = std::vector<Cube>(n_trials_);
  y_ = std::vector<Matrix>(n_trials_);
  for (size_t trial = 0; trial < n_trials_; trial++) {
    x_[trial] = Matrix(n_x_, n_t_[trial], fill::zeros);
    P_[trial] = Cube(n_x_, n_x_, n_t_[trial], fill::zeros);
    P_t_tm1_[trial] = Cube(n_x_, n_x_, n_t_[trial], fill::zeros);
    y_[trial] = Matrix(n_y_, n_t_[trial], fill::zeros);
  }

  diag_y_ = Matrix(n_y_, n_y_, fill::zeros);

  // covariances in expectation step
  sum_E_x_t_x_t_ = Matrix(n_x_, n_x_, fill::zeros);
  sum_E_xu_tm1_xu_tm1_ = Matrix(n_x_ + n_u_, n_x_ + n_u_, fill::zeros);
  sum_E_xu_t_xu_tm1_ = Matrix(n_x_ + n_u_, n_x_ + n_u_, fill::zeros);
}

template <typename Fit>
const Fit& EM<Fit>::Run(bool calc_dynamics, bool calc_Q, bool calc_init,
                        bool calc_output, bool calc_measurement,
                        size_t max_iter, data_t tol) {
  Reset();  // to initial conditions

  size_t n_params =
      3 * n_x_ * n_x_ + n_x_ * n_u_ + n_x_ + n_y_ * n_x_ + n_y_ * n_y_;
  Vector theta(n_params);
  Vector theta_new(n_params);
  data_t max_dtheta = 1;

  // if solving for initial conditions, allow them be varied.
  // otherwise, freeze at provided values.
  bool force_common_initial = !calc_init;

  // go until parameter convergence
  for (size_t l = 0; l < max_iter; l++) {
    theta_ = UpdateTheta();

    std::cout << "Iteration " << l + 1 << "/" << max_iter << " ...\n";

    Expectation(force_common_initial);
    Maximization(calc_dynamics, calc_Q, calc_init, calc_output,
                 calc_measurement);

    // check convergence
    theta_new = UpdateTheta();

    Vector dtheta = abs(theta_new - theta_) / abs(theta_);
    // some parameters could be zero...
    arma::uvec ubi_finite = find_finite(dtheta);

    max_dtheta = max(dtheta.elem(ubi_finite));
    std::cout << "max dtheta: " << max_dtheta << "\n";
    if (max_dtheta < tol) {
      std::cout << "Converged.\n";
      break;
    }

    std::cout << "\n";
  }

  return fit_;
}

template <typename Fit>
void EM<Fit>::Smooth(bool force_common_initial) {
  Matrix k_e(n_x_, n_y_);  // estimator gain
  Cube k_backfilt;         // back-filtering gains

  // TODO(mfbolus): this loop could be made parallel
  for (size_t trial = 0; trial < z_.size(); trial++) {
    Matrix x_pre(n_x_, n_t_[trial], fill::zeros);
    Cube p_pre(n_x_, n_x_, n_t_[trial], fill::zeros);
    Matrix x_post(n_x_, n_t_[trial], fill::zeros);
    Cube p_post(n_x_, n_x_, n_t_[trial], fill::zeros);

    if (force_common_initial)  // forces all trials to have same initial
                               // conditions.
    {
      x_[trial].col(0) = fit_.x0();
      P_[trial].slice(0) = fit_.P0();
    }
    y_[trial].col(0) = fit_.C() * x_[trial].col(0) + fit_.d();

    // This *should not* be necessary but make sure P is symmetric.
    ForceSymPD(P_[trial].slice(0));

    x_pre.col(0) = x_[trial].col(0);
    p_pre.slice(0) = P_[trial].slice(0);

    x_post.col(0) = x_[trial].col(0);
    p_post.slice(0) = P_[trial].slice(0);

    // filter
    for (size_t t = 1; t < n_t_[trial]; t++) {
      // predict
      fit_.f(x_pre, x_post, u_.at(trial), t);
      fit_.h(y_[trial], x_pre, t);
      diag_y_.diag() = y_[trial].col(t);  // TODO(mfbolus): change if parallel

      // update --> posterior estimation
      RecurseKe(k_e, p_pre, p_post, t);
      x_post.col(t) =
          x_pre.col(t) + k_e * (z_.at(trial).col(t) - y_[trial].col(t));
      y_[trial].col(t) = fit_.C() * x_post.col(t) + fit_.d();
    }

    // backfilter -> Smoothed estimate
    // Reference:
    // Shumway et Stoffer (1982)
    ForceSymPD(p_post.slice(n_t_[trial] - 1));
    k_backfilt = Cube(n_x_, n_x_, n_t_[trial], fill::zeros);
    x_[trial].col(n_t_[trial] - 1) = x_post.col(n_t_[trial] - 1);
    P_[trial].slice(n_t_[trial] - 1) = p_post.slice(n_t_[trial] - 1);
    for (size_t t = (n_t_[trial] - 1); t > 0; t--) {
      // TODO(mfmbolus): should not be necessary to force symm positive def
      ForceSymPD(p_pre.slice(t));
      ForceSymPD(p_post.slice(t - 1));
      ForceSymPD(P_[trial].slice(t));
      k_backfilt.slice(t - 1) =
          p_post.slice(t - 1) * fit_.A().t() * inv_sympd(p_pre.slice(t));
      x_[trial].col(t - 1) =
          x_post.col(t - 1) +
          k_backfilt.slice(t - 1) * (x_[trial].col(t) - x_pre.col(t));
      P_[trial].slice(t - 1) =
          p_post.slice(t - 1) + k_backfilt.slice(t - 1) *
                                    (P_[trial].slice(t) - p_pre.slice(t)) *
                                    k_backfilt.slice(t - 1).t();
    }

    // do the same for P_t_tm1
    Matrix id(n_x_, n_x_, fill::eye);
    P_t_tm1_[trial].slice(n_t_[trial] - 1) =
        (id - k_e * fit_.C()) * fit_.A() * p_post.slice(n_t_[trial] - 2);
    for (size_t t = (n_t_[trial] - 1); t > 1; t--) {
      P_t_tm1_[trial].slice(t - 1) =
          p_post.slice(t - 1) * k_backfilt.slice(t - 2).t() +
          k_backfilt.slice(t - 1) *
              (P_t_tm1_[trial].slice(t) - fit_.A() * p_post.slice(t - 1)) *
              k_backfilt.slice(t - 2).t();
    }

    // finally, get smoothed estimate of output
    for (size_t t = 0; t < n_t_[trial]; t++) {
      fit_.h(y_[trial], x_[trial], t);
    }  // samps loop
  }    // trial loop
}  // Smooth

// template <typename Fit>
// void EM<Fit>::RecurseKe(Matrix& Ke, Cube& P_pre, Cube& P_post, size_t t) {
//   // predict covar
//   P_pre.slice(t) = fit_.A() * P_post.slice(t - 1) * fit_.A().t() + fit_.Q();

//   // update Ke
//   Ke = P_pre.slice(t) * fit_.C().t() *
//        inv_sympd(fit_.C() * P_pre.slice(t) * fit_.C().t() + fit_.R());

//   // update cov
//   // Reference: Ghahramani et Hinton (1996)
//   P_post.slice(t) = P_pre.slice(t) - Ke * fit_.C() * P_pre.slice(t);

//   // // n.b. for poisson :
//   // P_pre.slice(t) = fit_.A() * P_post.slice(t - 1) * fit_.A().t() +
//   fit_.Q();
//   // // update cov
//   // P_post.slice(t) = pinv(pinv(P_pre.slice(t)) + fit_.C().t() * diag_y_ *
//   // fit_.C());
//   // // update Ke
//   // Ke = P_post.slice(t) * fit_.C();
// }

template <typename Fit>
void EM<Fit>::Expectation(bool force_common_initial) {
  // calculate the mean/cov of state needed for maximizing E[pr(z|theta)]
  Smooth(force_common_initial);

  // now get the various forms of sum(E[xx']) needed
  // n.b. Going to start at t=1 rather than 0 bc most max terms need that.
  // so really "n_t_tot_" is (n_t_tot_-1)
  n_t_tot_ = 0;
  sum_E_x_t_x_t_ = Matrix(n_x_, n_x_, fill::zeros);
  sum_E_xu_tm1_xu_tm1_ = Matrix(n_x_ + n_u_, n_x_ + n_u_, fill::zeros);
  sum_E_xu_t_xu_tm1_ = Matrix(n_x_ + n_u_, n_x_ + n_u_, fill::zeros);

  Vector xu_tm1(n_x_ + n_u_, fill::zeros);
  Vector xu_t(n_x_ + n_u_, fill::zeros);

  for (size_t trial = 0; trial < z_.size(); trial++) {
    for (size_t t = 1; t < n_t_[trial]; t++) {
      // ------------ sum_E_x_t_x_t ------------
      sum_E_x_t_x_t_ += x_[trial].col(t) * x_[trial].col(t).t();
      sum_E_x_t_x_t_ += P_[trial].slice(t);

      // ------------ sum_E_xu_tm1_xu_tm1 ------------
      xu_tm1 = join_vert(x_[trial].col(t - 1), u_.at(trial).col(t - 1));
      sum_E_xu_tm1_xu_tm1_ += xu_tm1 * xu_tm1.t();
      sum_E_xu_tm1_xu_tm1_.submat(0, 0, n_x_ - 1, n_x_ - 1) +=
          P_[trial].slice(t - 1);

      // ------------ sum_E_xu_t_xu_tm1 ------------
      xu_t = join_vert(x_[trial].col(t), u_.at(trial).col(t));
      sum_E_xu_t_xu_tm1_ += xu_t * xu_tm1.t();
      sum_E_xu_t_xu_tm1_.submat(0, 0, n_x_ - 1, n_x_ - 1) +=
          P_t_tm1_[trial].slice(t);

      n_t_tot_ += 1;
    }  // time
  }    // trial
}  // Expectation

template <typename Fit>
void EM<Fit>::Maximization(bool calc_dynamics, bool calc_Q, bool calc_init,
                           bool calc_output, bool calc_measurement) {
  if (calc_output) {
    MaximizeOutput();
  }

  if (calc_measurement) {
    MaximizeMeasurement();
  }

  if (calc_dynamics) {
    MaximizeDynamics();
  }

  if (calc_Q) {
    MaximizeQ();
  }

  if (calc_init) {
    MaximizeInitial();
  }
}  // Maximization

template <typename Fit>
void EM<Fit>::MaximizeDynamics() {
  // Shumway, Stoffer (1982); Ghahgramani, Hinton (1996)
  Matrix ab = sum_E_xu_t_xu_tm1_.submat(0, 0, n_x_ - 1, n_x_ + n_u_ - 1) *
              inv_sympd(sum_E_xu_tm1_xu_tm1_);
  fit_.set_A(ab.submat(0, 0, n_x_ - 1, n_x_ - 1));
  fit_.set_B(ab.submat(0, n_x_, n_x_ - 1, n_x_ + n_u_ - 1));
  std::cout << "A_new[0]: " << fit_.A()[0] << "\n";
  std::cout << "B_new[0]: " << fit_.B()[0] << "\n";
}

template <typename Fit>
void EM<Fit>::MaximizeQ() {
  // // Shumway, Stoffer (1982); Ghahgramani, Hinton (1996)
  // View sum_e_x_t_xu_tm1 =
  //     sum_E_xu_t_xu_tm1_.submat(0, 0, n_x_ - 1, n_x_ + n_u_ - 1);
  // Matrix q = sum_E_x_t_x_t_ - sum_e_x_t_xu_tm1 *
  //                                inv_sympd(sum_E_xu_tm1_xu_tm1_) *
  //                                sum_e_x_t_xu_tm1.t();
  // q /= n_t_tot_;

  // this way is same as above iff dynamics were just updated:
  // View sum_e_x_t_xu_tm1 =
  //     sum_E_xu_t_xu_tm1_.submat(0, 0, n_x_ - 1, n_x_ + n_u_ - 1);
  // Matrix ab = arma::join_horiz(fit_.A(), fit_.B());
  // Matrix q = sum_E_x_t_x_t_ - ab * sum_e_x_t_xu_tm1.t();
  // q /= n_t_tot_;

  // From scratch method:
  // Q is covariance of the error between state and dynamics-predicted state
  // (aka process noise)
  // Q* = E[(x_t - Ax_{t-1} - Bu_{t-1})*(x_t - Ax_{t-1} - Bu_{t-1})']
  // t-1 terms:
  View sum_e_x_tm1_x_tm1 =
      sum_E_xu_tm1_xu_tm1_.submat(0, 0, n_x_ - 1, n_x_ - 1);
  View sum_e_u_tm1_u_tm1 =
      sum_E_xu_tm1_xu_tm1_.submat(n_x_, n_x_, n_x_ + n_u_ - 1, n_x_ + n_u_ - 1);
  View sum_e_x_tm1_u_tm1 =
      sum_E_xu_tm1_xu_tm1_.submat(0, n_x_, n_x_ - 1, n_x_ + n_u_ - 1);

  // t, t-1 terms:
  View sum_e_x_t_x_tm1 = sum_E_xu_t_xu_tm1_.submat(0, 0, n_x_ - 1, n_x_ - 1);
  View sum_e_x_t_u_tm1 =
      sum_E_xu_t_xu_tm1_.submat(0, n_x_, n_x_ - 1, n_x_ + n_u_ - 1);

  Matrix q = sum_E_x_t_x_t_;
  q += fit_.A() * sum_e_x_tm1_x_tm1 * fit_.A().t();
  q -= sum_e_x_t_x_tm1 * fit_.A().t();
  q -= fit_.A() * sum_e_x_t_x_tm1.t();
  // input-related terms:
  q += fit_.B() * sum_e_u_tm1_u_tm1 * fit_.B().t();
  q -= sum_e_x_t_u_tm1 * fit_.B().t();
  q -= fit_.B() * sum_e_x_t_u_tm1.t();
  q += fit_.A() * sum_e_x_tm1_u_tm1 * fit_.B().t();
  q += fit_.B() * sum_e_x_tm1_u_tm1.t() * fit_.A().t();
  q /= n_t_tot_;

  fit_.set_Q(q);
  std::cout << "Q_new[0]: " << fit_.Q()[0] << "\n";
  // std::cout << "Q_new: \n" << fit_.Q() << "\n";
}

template <typename Fit>
void EM<Fit>::MaximizeInitial() {
  Vector x0 = fit_.x0();
  x0.zeros();
  for (size_t trial = 0; trial < z_.size(); trial++) {
    x0 += x_[trial].col(0);
  }
  x0 /= z_.size();
  std::cout << "x0_new[0]: " << x0[0] << "\n";

  // always recalc P0 even if the initial state is fixed (at zero, for
  // example)
  Matrix e_var(n_x_, n_x_, fill::zeros);
  for (size_t trial = 0; trial < z_.size(); trial++) {
    e_var += (x_[trial].col(0) - x0) * (x_[trial].col(0) - x0).t();
  }
  e_var /= z_.size();

  // go ahead and subtract x0*x0' so don't have to below.
  e_var -= x0 * x0.t();

  // To get P0, going to get initial P_ per trial and average.
  // (which might be wrong, but need a single number)
  Matrix p0 = fit_.P0();
  p0.zeros();
  for (size_t trial = 0; trial < z_.size(); trial++) {
    p0 +=
        (x_[trial].col(0) * x_[trial].col(0).t()) + P_[trial].slice(0) + e_var;
  }
  p0 /= z_.size();

  fit_.set_P0(p0);
  std::cout << "P0_new[0]: " << fit_.P0()[0] << "\n";
}

template <typename Fit>
void EM<Fit>::MaximizeOutput() {
  // solve for C+d:
  Matrix sum_zx(n_y_, n_x_ + 1, fill::zeros);
  Vector x1(n_x_ + 1, fill::zeros);
  x1[n_x_] = 1.0;  // augment with one to solve for bias
  Matrix sum_e_x1_x1(n_x_ + 1, n_x_ + 1, fill::zeros);
  for (size_t trial = 0; trial < z_.size(); trial++) {
    for (size_t t = 1; t < n_t_[trial]; t++) {
      x1.subvec(0, n_x_ - 1) = x_[trial].col(t);
      sum_zx += z_.at(trial).col(t) * x1.t();
      sum_e_x1_x1 += x1 * x1.t();
      sum_e_x1_x1.submat(0, 0, n_x_ - 1, n_x_ - 1) += P_[trial].slice(t);
    }
  }
  Matrix cd = sum_zx * inv_sympd(sum_e_x1_x1);
  fit_.set_C(cd.submat(0, 0, n_y_ - 1, n_x_ - 1));
  fit_.set_d(vectorise(cd.submat(0, n_x_, n_y_ - 1, n_x_)));
  std::cout << "C_new[0]: " << fit_.C()[0] << "\n";
  std::cout << "d_new[0]: " << fit_.d()[0] << "\n";
}

template <typename Fit>
void EM<Fit>::MaximizeMeasurement() {
  // Solve for measurement noise covar
  size_t n_t_tot = 0;
  // Ghahgramani, Hinton 1996:
  Matrix sum_zz(n_y_, n_y_, fill::zeros);
  Matrix sum_yz(n_y_, n_y_, fill::zeros);
  for (size_t trial = 0; trial < z_.size(); trial++) {
    for (size_t t = 1; t < n_t_[trial]; t++) {
      sum_zz += z_.at(trial).col(t) * z_.at(trial).col(t).t();
      // Use Cnew:
      sum_yz +=
          (fit_.C() * x_[trial].col(t) + fit_.d()) * z_.at(trial).col(t).t();
      n_t_tot += 1;
    }
  }
  fit_.set_R((sum_zz - sum_yz) / n_t_tot);
  std::cout << "R_new[0]: " << fit_.R()[0] << "\n";
}

template <typename Fit>
void EM<Fit>::Reset() {
  // reset to initial conditions
  for (size_t trial = 0; trial < n_trials_; trial++) {
    x_[trial].col(0) = fit_.x0();
    P_[trial].slice(0) = fit_.P0();
    y_[trial].col(0) = fit_.C() * x_[trial].col(0) + fit_.d();
  }
}

template <typename Fit>
Vector EM<Fit>::UpdateTheta() {
  // TODO(mfbolus): This should include n_y_ more params for d.
  size_t n_params = 3 * n_x_ * n_x_ + n_x_ * n_u_ + n_x_ + n_y_ * n_x_ + n_y_;
  if (fit_.R().n_elem > 0) {
    n_params += n_y_ * n_y_;
  }
  Vector theta(n_params);

  size_t idx_start = 0;
  theta.subvec(idx_start, idx_start + n_x_ * n_x_ - 1) = vectorise(fit_.A());
  idx_start += n_x_ * n_x_;
  theta.subvec(idx_start, idx_start + n_x_ * n_u_ - 1) = vectorise(fit_.B());
  idx_start += n_x_ * n_u_;
  theta.subvec(idx_start, idx_start + n_x_ * n_x_ - 1) = vectorise(fit_.Q());
  idx_start += n_x_ * n_x_;
  theta.subvec(idx_start, idx_start + n_x_ - 1) = vectorise(fit_.x0());
  idx_start += n_x_;
  theta.subvec(idx_start, idx_start + n_x_ * n_x_ - 1) = vectorise(fit_.P0());
  idx_start += n_x_ * n_x_;
  theta.subvec(idx_start, idx_start + n_y_ * n_x_ - 1) = vectorise(fit_.C());
  idx_start += n_y_ * n_x_;
  theta.subvec(idx_start, idx_start + n_y_ - 1) = vectorise(fit_.d());
  idx_start += n_y_;
  if (fit_.R().n_elem > 0) {
    theta.subvec(idx_start, idx_start + n_y_ * n_y_ - 1) = vectorise(fit_.R());
  }

  return theta;
}

}  // namespace lds

#endif
```


-------------------------------

Updated on 22 June 2021 at 23:08:17 CDT
