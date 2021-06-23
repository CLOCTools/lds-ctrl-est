---
title: ldsCtrlEst_h/lds_fit_ssid.h
summary: subspace identification 

---

# ldsCtrlEst_h/lds_fit_ssid.h

subspace identification  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/lds-ctrl-est/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/)**  |

## Detailed Description



This file declares and partially defines a template type by which LDS models are fit by a subspace identification (SSID) algorithm (`[lds::SSID](/lds-ctrl-est/docs/api/classes/classlds_1_1_s_s_i_d/)<Fit>`).

References: [1] van Overschee P, de Moore B. (1996) Subspace Identification for Linear Systems. Boston: Springer. 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_fit_ssid.h - SSID Fit ------------------*- C++ -*-===//
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

#ifndef LDSCTRLEST_LDS_FIT_SSID_H
#define LDSCTRLEST_LDS_FIT_SSID_H

#include "lds_fit.h"

namespace lds {

template <typename Fit>
class SSID {
  static_assert(std::is_base_of<lds::Fit, Fit>::value,
                "Fit must be derived from lds::Fit type.");

 public:
  SSID() = default;

  SSID(size_t n_x, size_t n_h, data_t dt,
       UniformMatrixList<kMatFreeDim2>&& u_train,
       UniformMatrixList<kMatFreeDim2>&& z_train,
       const Vector& d = Vector(1).fill(-kInf));

  std::tuple<Fit, Vector> Run(SSIDWt ssid_wt);

  std::tuple<UniformMatrixList<kMatFreeDim2>, UniformMatrixList<kMatFreeDim2>>
  ReturnData() {
    auto tuple = std::make_tuple(std::move(u_), std::move(z_));
    u_ = UniformMatrixList<kMatFreeDim2>();
    z_ = UniformMatrixList<kMatFreeDim2>();
    return tuple;
  }

 protected:
  void CalcD(data_t t_silence = 0.1, data_t thresh_silence = 0.001);

  void CreateHankelDataMat();

  virtual void DecomposeData() = 0;

  void CalcSVD(SSIDWt wt);

  void Solve(data_t wt_dc);

  void RecomputeExtObs();

  // input/output training data
  UniformMatrixList<kMatFreeDim2> u_;  
  UniformMatrixList<kMatFreeDim2> z_;  
  Matrix D_;                           

  Fit fit_;      
  Matrix g_dc_;  

  data_t dt_{};   
  size_t n_u_{};  
  size_t n_x_{};  
  size_t n_y_{};  
  size_t n_h_{};
  size_t n_trials_{};        
  std::vector<size_t> n_t_;  
  size_t n_t_tot_{};         

  Matrix L_;          
  Vector s_;          
  Matrix ext_obs_t_;  
};

template <typename Fit>
SSID<Fit>::SSID(size_t n_x, size_t n_h, data_t dt,
                UniformMatrixList<kMatFreeDim2>&& u_train,
                UniformMatrixList<kMatFreeDim2>&& z_train, const Vector& d) {
  // check input/output data dimensions are consistent
  if (z_train.size() != u_train.size()) {
    throw std::runtime_error(
        "I/O training data have different number of trials.");
  }
  n_trials_ = u_train.size();

  n_t_tot_ = 0;
  n_t_ = std::vector<size_t>(n_trials_);
  for (size_t trial = 0; trial < n_trials_; trial++) {
    if (z_train.at(trial).n_cols != u_train.at(trial).n_cols) {
      throw std::runtime_error(
          "I/O training data have different number of time steps.");
    }
    n_t_[trial] = u_train.at(trial).n_cols;
    n_t_tot_ += n_t_[trial];
  }

  dt_ = dt;
  n_x_ = n_x;
  n_u_ = u_train.at(0).n_rows;
  n_y_ = z_train.at(0).n_rows;
  n_h_ = n_h;

  // dimensionality check for eventual block-hankel data matrix
  size_t len = n_t_tot_ - 2 * n_h_ + 1;
  if (len < (2 * n_h_ * (n_u_ + n_y_))) {
    std::ostringstream ss;
    ss << "Dataset problem! More rows than columns in block-hankel data "
          "matrix: 2*(n_u+n_y)*n_h > data-length! Need higher data-length or "
          "lower n_h.";
    throw std::runtime_error(ss.str());
  }

  fit_ = Fit(n_u_, n_x_, n_y_, dt_);

  u_ = std::move(u_train);
  z_ = std::move(z_train);

  if (!d.is_finite() || (d.n_rows != n_y_)) {
    // TODO(mfbolus): implement least-square solution for impulse response with
    // a second input of ones. Data-driven way of accounting for offset *not*
    // driven by an input.
    //
    // For now, calculate output bias (d) as the
    // output wherever the stimulus has not been on for some amount of time.
    // convolve u with rectangle and take all samples. This is a reasonable
    // approach, since often when autonomous systems are fit (i.e., systems with
    // no input), they will subtract off the mean of the output. This
    // essentially amounts to setting output bias to the mean of the output when
    // there is no stimulation.
    data_t t_silence = 0.1;
    data_t thresh_silence = 0.001;
    CalcD(t_silence, thresh_silence);
  } else {
    fit_.set_d(d);
  }
}

template <typename Fit>
std::tuple<Fit, Vector> SSID<Fit>::Run(SSIDWt ssid_wt) {
  // the weight on minimizing dc I/O gain only works for gaussian,
  // and hopefully not necessary with appropriate dataset.
  data_t wt_dc = 0;
  // std::cout << "creating hankel mat\n";
  CreateHankelDataMat();
  // std::cout << "decomposing data\n";
  DecomposeData();
  // std::cout << "calculating svd\n";
  CalcSVD(ssid_wt);
  // std::cout << "solving for params\n";
  Solve(wt_dc);
  // std::cout << "fin\n";
  return std::make_tuple(fit_, s_);
}

template <typename Fit>
void SSID<Fit>::CalcD(data_t t_silence, data_t thresh_silence) {
  Vector d(z_.at(0).n_rows, fill::zeros);
  Vector win(static_cast<size_t>(t_silence / dt_), fill::ones);
  Vector sum_z_silence(n_y_, fill::zeros);
  size_t n_silence(0);
  for (size_t trial = 0; trial < u_.size(); trial++) {
    // find silent samples
    // start by convolving with
    Vector sum_u = vectorise(sum(abs(u_.at(trial)), 0));
    Vector u_conv = conv(sum_u, win, "same");

    // get only the samples that are silent...
    arma::uvec ubi_silence = find(u_conv <= thresh_silence);
    if (ubi_silence.n_elem > 0) {
      sum_z_silence += arma::sum(z_.at(trial).cols(ubi_silence), 1);
      n_silence += ubi_silence.n_elem;
    }
  }
  if (n_silence > 0) {
    d = sum_z_silence / n_silence;
  }
  fit_.set_d(d);
}

template <typename Fit>
void SSID<Fit>::CreateHankelDataMat() {
  // temporary copy of data
  Matrix z(n_y_, n_t_tot_, fill::zeros);
  Matrix u(n_u_, n_t_tot_, fill::zeros);
  size_t so_far(0);

  for (size_t trial = 0; trial < z_.size(); trial++) {
    z.submat(0, so_far, n_y_ - 1, so_far + n_t_.at(trial) - 1) = z_.at(trial);
    u.submat(0, so_far, n_u_ - 1, so_far + n_t_.at(trial) - 1) = u_.at(trial);
    so_far += n_t_.at(trial);
  }

  // remove output bias
  z.each_col() -= fit_.d();

  // calculate I/O gain @ DC while data in convenient form
  g_dc_ = z * pinv(u);
  // std::cout << "G0_data = " << g_dc_ << "\n";

  // create hankel data matrix
  size_t len = z.n_cols - 2 * n_h_ + 1;  // data length in hankel mat

  // block-hankel data matrix
  D_ = Matrix(2 * n_h_ * (n_u_ + n_y_), len, fill::zeros);
  // past input
  auto u_p = D_.submat(0, 0, n_h_ * n_u_ - 1, len - 1);
  // future input
  auto u_f = D_.submat(n_h_ * n_u_, 0, 2 * n_h_ * n_u_ - 1, len - 1);
  // past output
  auto y_p =
      D_.submat(2 * n_h_ * n_u_, 0, n_h_ * (2 * n_u_ + n_y_) - 1, len - 1);
  // future output
  auto y_f = D_.submat(n_h_ * (2 * n_u_ + n_y_), 0,
                       2 * n_h_ * (n_u_ + n_y_) - 1, len - 1);

  size_t idx = 0;
  for (size_t k = 0; k < len; k++) {
    idx = 0;
    for (size_t kk = k; kk < (n_h_ + k); kk++) {
      u_p.col(k).subvec(idx, idx + n_u_ - 1) = u.col(kk);
      idx += n_u_;
    }

    idx = 0;
    for (size_t kk = (n_h_ + k); kk < (2 * n_h_ + k); kk++) {
      u_f.col(k).subvec(idx, idx + n_u_ - 1) = u.col(kk);
      idx += n_u_;
    }

    idx = 0;
    for (size_t kk = k; kk < (n_h_ + k); kk++) {
      y_p.col(k).subvec(idx, idx + n_y_ - 1) = z.col(kk);
      idx += n_y_;
    }

    idx = 0;
    for (size_t kk = (n_h_ + k); kk < (2 * n_h_ + k); kk++) {
      y_f.col(k).subvec(idx, idx + n_y_ - 1) = z.col(kk);
      idx += n_y_;
    }
  }

  D_ /= sqrt(static_cast<data_t>(len));
}

// template <typename Fit>
// void SSID<Fit>::DecomposeData() {
//   // do LQ decomp instead of calculating covariance expensive way
//   // Note that "R" in van Overschee is lower-triangular (L), not "R" in QR
//   // decomp. Very confusing.
//   Matrix q_t;
//   lq(L_, q_t, D_);
//   // van Overschee zeros out the other elements.
//   L_ = trimatl(L_);
// }

template <typename Fit>
void SSID<Fit>::CalcSVD(SSIDWt wt) {
  // submats that will be needed:
  auto R_14_14 = L_.submat(0, 0, n_h_ * (2 * n_u_ + n_y_) - 1,
                           n_h_ * (2 * n_u_ + n_y_) - 1);
  auto R_11_14 = L_.submat(0, 0, n_h_ * n_u_ - 1, n_h_ * (2 * n_u_ + n_y_) - 1);
  auto R_11_13 = L_.submat(0, 0, n_h_ * n_u_ - 1, n_h_ * (2 * n_u_) - 1);
  auto R_23_13 =
      L_.submat(n_h_ * n_u_, 0, 2 * n_h_ * n_u_ - 1, 2 * n_h_ * n_u_ - 1);
  auto R_44_14 = L_.submat(2 * n_u_ * n_h_, 0, n_h_ * (2 * n_u_ + n_y_) - 1,
                           n_h_ * (2 * n_u_ + n_y_) - 1);
  auto R_44_13 = L_.submat(2 * n_u_ * n_h_, 0, n_h_ * (2 * n_u_ + n_y_) - 1,
                           n_h_ * (2 * n_u_) - 1);
  auto R_44 =
      L_.submat(2 * n_u_ * n_h_, 2 * n_u_ * n_h_, n_h_ * (2 * n_u_ + n_y_) - 1,
                n_h_ * (2 * n_u_ + n_y_) - 1);
  auto R_56_14 =
      L_.submat(n_h_ * (2 * n_u_ + n_y_), 0, n_h_ * (2 * n_u_ + 2 * n_y_) - 1,
                n_h_ * (2 * n_u_ + n_y_) - 1);

  Matrix Lup_Luf_Lyp = R_56_14 * pinv(R_14_14);
  auto Lup = Lup_Luf_Lyp.submat(0, 0, n_h_ * n_y_ - 1, n_h_ * n_u_ - 1);
  auto Luf =
      Lup_Luf_Lyp.submat(0, n_h_ * n_u_, n_h_ * n_y_ - 1, 2 * n_h_ * n_u_ - 1);
  auto Lyp = Lup_Luf_Lyp.submat(0, 2 * n_h_ * n_u_, n_h_ * n_y_ - 1,
                                n_h_ * (2 * n_u_ + n_y_) - 1);

  // aka: R_f
  Matrix R_56_16 = L_.submat(n_h_ * (2 * n_u_ + n_y_), 0,
                             2 * n_h_ * (n_u_ + n_y_) - 1, L_.n_cols - 1);
  // from van Overschee subid.m:
  // Rf = R((2*m+l)*i+1:2*(m+l)*i,:);   % Future outputs

  Matrix U;
  Matrix V;
  switch (wt) {
    case kSSIDNone: {
      // No weighting. (what van Overschee calls "N4SID")
      Matrix O_k_sans_Qt = Lup * R_11_14 + Lyp * R_44_14;
      arma::svd(U, s_, V, O_k_sans_Qt, "std");
    } break;
    case kSSIDMOESP: {
      // MOESP weighting
      // This is what they use in the "robust" algorithm van Overschee, de Moor
      // 1996
      Matrix Pi = Matrix(2 * n_h_ * n_u_, 2 * n_h_ * n_u_, fill::eye) -
                  R_23_13.t() * inv(R_23_13 * R_23_13.t()) * R_23_13;
      Matrix O_k_ortho_Uf_sans_Qt =
          join_horiz((Lup * R_11_13 + Lyp * R_44_13) * Pi, Lyp * R_44);
      svd(U, s_, V, O_k_ortho_Uf_sans_Qt, "std");
    } break;
    case kSSIDCVA: {
      // CVA weighting
      // See van Overschee's matlab code (subid.m):
      // https://www.mathworks.com/matlabcentral/fileexchange/2290-subspace-identification-for-linear-systems
      Matrix Pi = Matrix(2 * n_h_ * n_u_, 2 * n_h_ * n_u_, fill::eye) -
                  R_23_13.t() * inv(R_23_13 * R_23_13.t()) * R_23_13;
      Matrix O_k_ortho_Uf_sans_Qt =
          join_horiz((Lup * R_11_13 + Lyp * R_44_13) * Pi, Lyp * R_44);

      Matrix inv_w1;
      Matrix qt1;
      lq(inv_w1, qt1, R_56_16);  // lq decomp of R_f (future output data)
      inv_w1 = trimatl(inv_w1);
      inv_w1 = inv_w1.submat(0, 0, n_y_ * n_h_ - 1, n_y_ * n_h_ - 1);
      Matrix w_o_w = arma::solve(
          inv_w1, O_k_ortho_Uf_sans_Qt);  // alternatively
                                          // pinv(inv_W1)*O_k_ortho_Uf_sans_Qt
      svd(U, s_, V, w_o_w, "std");

      U = inv_w1 * U;
      break;
    }
  }

  // Truncate to model order (heart of ssid method)
  auto s_hat = s_.subvec(0, n_x_ - 1);
  Matrix diag_sqrt_s = diagmat(sqrt(s_hat));
  auto u_hat = U.submat(0, 0, U.n_rows - 1, n_x_ - 1);

  // get extended observability and controllability mats
  ext_obs_t_ = u_hat * diag_sqrt_s;  // extended observability matrix
}

template <typename Fit>
void SSID<Fit>::Solve(data_t wt_dc) {
  // required submats
  auto R_56_14 =
      L_.submat(n_h_ * (2 * n_u_ + n_y_), 0, n_h_ * (2 * n_u_ + 2 * n_y_) - 1,
                n_h_ * (2 * n_u_ + n_y_) - 1);
  auto R_23_15 = L_.submat(n_h_ * n_u_, 0, 2 * n_h_ * n_u_ - 1,
                           n_h_ * (2 * n_u_ + n_y_) + n_y_ - 1);
  auto R_66_15 = L_.submat(n_h_ * (2 * n_u_ + n_y_) + n_y_, 0,
                           2 * n_h_ * (n_u_ + n_y_) - 1,
                           n_h_ * (2 * n_u_ + n_y_) + n_y_ - 1);
  auto R_55_15 = L_.submat(n_h_ * (2 * n_u_ + n_y_), 0,
                           n_h_ * (2 * n_u_ + n_y_) + n_y_ - 1,
                           n_h_ * (2 * n_u_ + n_y_) + n_y_ - 1);
  auto R_56_15 =
      L_.submat(n_h_ * (2 * n_u_ + n_y_), 0, 2 * n_h_ * (n_u_ + n_y_) - 1,
                n_h_ * (2 * n_u_ + n_y_) + n_y_ - 1);

  // Solve for params using appropriate algorithm:
  // robust deterministic/stochastic algorithm in van Overschee 1996
  // algorithm that the authors say "works" in practice.
  auto ext_obs_tm1 = ext_obs_t_.submat(
      0, 0, ext_obs_t_.n_rows - 1 - n_y_,
      ext_obs_t_.n_cols - 1);  // extended observability matrix

  // This is what textbook (1996) says:
  //
  // Matrix Tr = join_vert(pinv(ext_obs_t_) * R_56_15, R_23_15);
  //
  // HOWEVER, do not know why but have to fill the last place with zeros like
  // authors' matlab implementation (see `subid.m`)
  // Otherwise, get ridiculous covariances (although A,C estimates are close to
  // same...)
  Matrix Tr = join_vert(
      join_horiz(pinv(ext_obs_t_) * R_56_14, Matrix(n_x_, n_y_, fill::zeros)),
      R_23_15);
  Matrix Tl = join_vert(pinv(ext_obs_tm1) * R_66_15, R_55_15);
  Matrix S = Tl * pinv(Tr);

  // Use alternative in van Overschee 1996, p. 129. Apparently, should ensure
  // stability.
  fit_.set_C(ext_obs_t_.submat(0, 0, n_y_ - 1, ext_obs_t_.n_cols - 1));
  Matrix ext_obs_t_p1 = join_vert(
      ext_obs_t_.submat(n_y_, 0, ext_obs_t_.n_rows - 1, ext_obs_t_.n_cols - 1),
      Matrix(n_y_, ext_obs_t_.n_cols, fill::zeros));
  fit_.set_A(pinv(ext_obs_t_) * ext_obs_t_p1);

  // At this point, van Overschee & de Moor suggest re-calculating ext_obs_t_,
  // ext_obs_tm1 from (A, C) because it was just an approximation. This is
  RecomputeExtObs();
  ext_obs_tm1 = ext_obs_t_.submat(
      0, 0, ext_obs_t_.n_rows - 1 - n_y_,
      ext_obs_t_.n_cols - 1);  // extended observability matrix
  Tl = join_vert(pinv(ext_obs_tm1) * R_66_15, R_55_15);
  Tr = join_vert(
      join_horiz(pinv(ext_obs_t_) * R_56_14, Matrix(n_x_, n_y_, fill::zeros)),
      R_23_15);
  S = Tl * pinv(Tr);

  Matrix Lcurly = S.submat(0, 0, n_x_ + n_y_ - 1, n_x_ - 1) * pinv(ext_obs_t_);
  Matrix Mcurly = pinv(ext_obs_tm1);
  Matrix Pcurly = Tl - Lcurly * R_56_15;
  Vector Pvec = vectorise(Pcurly);
  Matrix Qcurly = R_23_15;

  // Identify [D; B], assuming D=0 and ensuring DC gain is correct
  Matrix sum_QcurlyT_kron_Ncurly(
      (n_h_ * (2 * n_u_ + n_y_) + n_y_) * (n_y_ + n_x_), n_u_ * (n_y_ + n_x_),
      fill::zeros);

  Matrix eye_ext_obs_tm1(n_y_ + ext_obs_tm1.n_rows, n_y_ + ext_obs_tm1.n_cols,
                         fill::eye);
  eye_ext_obs_tm1.submat(n_y_, n_y_, eye_ext_obs_tm1.n_rows - 1,
                         eye_ext_obs_tm1.n_cols - 1) = ext_obs_tm1;

  // van Overschee (1996) p. 126
  Matrix N1_Tl = -Lcurly;
  N1_Tl.submat(0, 0, n_x_ - 1, N1_Tl.n_cols - 1) +=
      join_horiz(Matrix(n_x_, n_y_, fill::zeros), Mcurly);
  N1_Tl.submat(n_x_, 0, n_x_ + n_y_ - 1, n_y_ - 1) +=
      Matrix(n_y_, n_y_, fill::eye);

  Matrix Nk_Tl(N1_Tl.n_rows, N1_Tl.n_cols, fill::zeros);
  Matrix N_k;
  for (size_t k = 0; k < n_h_; k++) {
    auto Qcurly_k =
        Qcurly.submat(n_u_ * k, 0, n_u_ * (k + 1) - 1, Qcurly.n_cols - 1);

    Nk_Tl.zeros();
    Nk_Tl.submat(0, 0, n_x_ + n_y_ - 1, Nk_Tl.n_cols - k * n_y_ - 1) =
        N1_Tl.submat(0, k * n_y_, N1_Tl.n_rows - 1, N1_Tl.n_cols - 1);
    N_k = Nk_Tl * eye_ext_obs_tm1;

    sum_QcurlyT_kron_Ncurly += kron(Qcurly_k.t(), N_k);
  }

  Matrix err_vec;
  if (wt_dc > 0) {
    // Constraints enforced by weighted least squares
    //
    // Reference:
    //
    // Privara S, ..., Ferkl L_. (2010) Subspace Identification of Poorly
    // Excited Industrial Systems. Conference in Decision and Control.

    // constraint 1: assume D=0 --> remove the components for Dvec (this is
    // actually a hard constraint in that it ignores D)
    Matrix sum_QcurlyT_kron_Ncurly_db = sum_QcurlyT_kron_Ncurly;
    sum_QcurlyT_kron_Ncurly =
        Matrix(sum_QcurlyT_kron_Ncurly_db.n_rows, n_x_ * n_u_);

    size_t kkk = 0;
    for (size_t k = 1; k < (n_u_ + 1); k++) {
      size_t start_idx = k * (n_y_ + n_x_) - n_x_;
      for (size_t kk = 0; kk < n_x_; kk++) {
        sum_QcurlyT_kron_Ncurly.col(kkk) =
            sum_QcurlyT_kron_Ncurly_db.col(start_idx + kk);
        kkk++;
      }
    }

    // constraint 2: Make sure DC I/O gain is correct
    Matrix b_to_g0 = fit_.C() * inv(Matrix(n_x_, n_x_, fill::eye) - fit_.A());
    Matrix Pvec_Gvec = join_vert(Pvec, vectorise(g_dc_));
    Matrix eye_kron_b_to_g0 = kron(Matrix(n_u_, n_u_, fill::eye), b_to_g0);
    Matrix sum_QcurlyT_kron_Ncurly_b_to_g0 =
        join_vert(sum_QcurlyT_kron_Ncurly, eye_kron_b_to_g0);

    // WEIGHTED LS
    // Important in practice because I care a lot about at least getting the DC
    // gain correct. Put x weight on minimizing error at DC, relative to others
    Matrix w(sum_QcurlyT_kron_Ncurly_b_to_g0.n_rows,
             sum_QcurlyT_kron_Ncurly_b_to_g0.n_rows, fill::eye);
    // Make weight on minimizing DC error immense so at least that
    // should be nailed.
    size_t start_row = sum_QcurlyT_kron_Ncurly.n_rows;
    size_t start_col = sum_QcurlyT_kron_Ncurly.n_rows;
    size_t stop_row = w.n_rows - 1;
    size_t stop_col = w.n_cols - 1;
    // w.submat(start_row, start_col, stop_row, stop_col) *= wt_dc*N;// scale
    // weight with data length?
    w.submat(start_row, start_col, stop_row, stop_col) *= wt_dc;
    Vector b_vec = inv(sum_QcurlyT_kron_Ncurly_b_to_g0.t() * w *
                       sum_QcurlyT_kron_Ncurly_b_to_g0) *
                   sum_QcurlyT_kron_Ncurly_b_to_g0.t() * w * Pvec_Gvec;

    fit_.set_B(Matrix(b_vec.memptr(), n_x_, n_u_));

    // Calculate residuals and their cov.
    // Because I've added constraints, I need to re-calculate the right term
    // with b_vec instead of how van Overschee do in final algorithm.
    err_vec = Pvec - sum_QcurlyT_kron_Ncurly * b_vec;
  } else {
    // default way: *no* constraint on G0 or D=0
    Vector db_vec = pinv(sum_QcurlyT_kron_Ncurly) * Pvec;
    // TODO(mfbolus) n.b., this gets thrown away...
    // Matrix D = Matrix(db_vec.memptr(), n_y_, n_u_);
    fit_.set_B(Matrix(db_vec.memptr() + (n_u_ * n_y_), n_x_, n_u_));
    err_vec = Pvec - sum_QcurlyT_kron_Ncurly * db_vec;
  }
  // Matrix err = Matrix(err_vec.memptr(), Pcurly.n_rows, Pcurly.n_cols);

  // TODO(mfbolus): Something is wrong with the error calculation above.
  // Use the way van overschee does it in `subid.m`
  // WARNING: this ignores any above constraints, so Q, R will be approximate...
  Matrix err = Tl - S * Tr;
  Matrix cov_err = err * err.t();
  fit_.set_Q(cov_err.submat(0, 0, n_x_ - 1, n_x_ - 1));
  fit_.set_R(cov_err.submat(n_x_, n_x_, n_x_ + n_y_ - 1, n_x_ + n_y_ - 1));
}

template <typename Fit>
void SSID<Fit>::RecomputeExtObs() {
  ext_obs_t_.submat(0, 0, n_y_ - 1, ext_obs_t_.n_cols - 1) = fit_.C();
  for (size_t k = 2; k < (n_h_ + 1); k++) {
    ext_obs_t_.submat((k - 1) * n_y_, 0, k * n_y_ - 1, ext_obs_t_.n_cols - 1) =
        ext_obs_t_.submat((k - 2) * n_y_, 0, (k - 1) * n_y_ - 1,
                          ext_obs_t_.n_cols - 1) *
        fit_.A();
  }
}

}  // namespace lds

#endif
```


-------------------------------

Updated on 22 June 2021 at 23:08:17 CDT
