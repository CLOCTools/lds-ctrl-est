//===-- ldsCtrlEst_h/lds_control.h - Controller -----------------*- C++ -*-===//
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
/// This file declares the type for control of a linear dynamical system
/// (lds::Controller).
///
/// \brief Controller
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_CTRL_H
#define LDSCTRLEST_LDS_CTRL_H

// namespace
#include "lds.h"
// system type
#include "lds_sys.h"

namespace lds {

template <typename System>
class Controller {
  static_assert(std::is_base_of<lds::System, System>::value,
                "System must be derived from lds::System type.");

 public:
  /**
   * @brief      Constructs a new Controller.
   */
  Controller() = default;

  /**
   * @brief      Constructs a new Controller.
   *
   * @param      sys           System (derived from lds::System)
   * @param      u_lb          lower bound on control (u)
   * @param      u_ub          upper bound on control (u)
   * @param      control_type  [optional] control type bit mask
   *
   * @tparam     System        type derived from lds::System
   */
  Controller(const System& sys, data_t u_lb, data_t u_ub,
             size_t control_type = 0);

  /**
   * @brief      Constructs a new Controller by moving the system object.
   *
   * @param      sys           System (derived from lds::System)
   * @param      u_lb          lower bound on control (u)
   * @param      u_ub          upper bound on control (u)
   * @param      control_type  [optional] control type bit mask
   *
   * @tparam     System        type derived from lds::System
   */
  Controller(System&& sys, data_t u_lb, data_t u_ub, size_t control_type = 0);

  /**
   * Updates the control signal (single-step). This is the most flexible option,
   * but requires user to have set the controller's y_ref, x_ref, and u_ref
   * variables.
   *
   * @brief      updates control signal (single-step)
   *
   * @param      z                          measurement
   * @param      do_control                 [optional] whether to update control
   *                                        (true) or simply feed through u_ref
   *                                        (false)
   * @param      do_lock_control            [optional] whether to lock control
   * at its current value
   * @param      sigma_soft_start           [optional] standard deviation
   * (sigma) of a Gaussian soft-start to control (do_control from false to true)
   * @param      sigma_u_noise              [optional] standard deviation
   * (sigma) of Gaussian noise added on top of control signal
   * @param      do_reset_at_control_onset  [optional] whether to reset
   * controller at control epoch onset (i.e., do_control from false to true)
   *
   * @return     updated control signal
   */
  const Vector& Control(const Vector& z, bool do_control = true,
                        bool do_lock_control = false,
                        data_t sigma_soft_start = 0, data_t sigma_u_noise = 0,
                        bool do_reset_at_control_onset = true);

  /**
   * Updates the control signal (single-step), given previously-set y_ref. This
   * method calculates the rest of the set point (u_ref, x_ref) that is required
   * to for the system to be at y_ref at steady state. This is accomplished by
   * linearly-constrained least-squares. For a single-output system, the
   * solution should be exact within control saturation limits. For a
   * multi-output system, it provides the least-squares comprimise across the
   * outputs.
   *
   * @brief      updates control signal, given previously-set (single-step)
   *
   * @param      z                          measurement
   * @param      do_control                 [optional] whether to update control
   *                                        (true) or simply feed through u_ref
   *                                        (false)
   * @param      do_estimation              [optional] whether to update state
   *                                        estimate (if false, effectively
   *                                        open-loop control)
   * @param      do_lock_control            [optional] whether to lock control
   *                                        at its current value
   * @param      sigma_soft_start           [optional] standard deviation
   *                                        (sigma) of a Gaussian soft-start to
   *                                        control (do_control from false to
   *                                        true)
   * @param      sigma_u_noise              [optional] standard deviation
   *                                        (sigma) of Gaussian noise added on
   *                                        top of control signal
   * @param      do_reset_at_control_onset  [optional] whether to reset
   *                                        controller at control epoch onset
   *                                        (i.e., do_control from false to
   *                                        true)
   *
   * @return     updated control signal
   */
  const Vector& ControlOutputReference(const Vector& z, bool do_control = true,
                                       bool do_estimation = true,
                                       bool do_lock_control = false,
                                       data_t sigma_soft_start = 0,
                                       data_t sigma_u_noise = 0,
                                       bool do_reset_at_control_onset = true);

  // get methods:
  const System& sys() const { return sys_; };
  /// Get state feedback controller gain
  const Matrix& Kc() const { return Kc_; };
  /// Get integral controller gain
  const Matrix& Kc_inty() const { return Kc_inty_; };
  /// Get input feedback controller gain
  const Matrix& Kc_u() const { return Kc_u_; };
  /// Get input gain used in controller design
  const Vector& g_design() const { return g_design_; };
  /// Get reference input
  const Vector& u_ref() const { return u_ref_; };
  /// Get reference state
  const Vector& x_ref() const { return x_ref_; };
  /// Get reference output
  const Vector& y_ref() const { return y_ref_; };
  /// Get controller type
  size_t control_type() const { return control_type_; };
  /// Get time constant of anti-integral-windup
  data_t tau_awu() const { return tau_awu_; };
  /// Get control lower bound
  data_t u_lb() const { return u_lb_; };
  /// Get control upper bound
  data_t u_ub() const { return u_ub_; };

  // set methods
  /// Set system
  void set_sys(const System& sys) {
    bool does_match = sys_.n_u() == sys.n_u();
    does_match = does_match && (sys_.n_x() == sys.n_x());
    does_match = does_match && (sys_.n_y() == sys.n_y());
    if (does_match) {
      sys_ = sys;
    } else {
      throw std::runtime_error(
          "new system argument to `set_sys` does not match dimensionality of "
          "existing system");
    }
  };
  /// Set input gain used in controller design (g_design)
  void set_g_design(const Vector& g_design) { Reassign(g_design_, g_design); };
  /// Set reference input (u_ref)
  void set_u_ref(const Vector& u_ref) { Reassign(u_ref_, u_ref); };
  /// Set reference state (x_ref)
  void set_x_ref(const Vector& x_ref) {
    Reassign(x_ref_, x_ref);
    cx_ref_ = sys_.C() * x_ref_;
  };

  // y_ref needs to be handled differently depending on output fn.
  // (need to populate cx_ref_ too, which depends on output fn)
  /// Set reference output (y_ref)
  virtual void set_y_ref(const Vector& y_ref) { Reassign(y_ref_, y_ref); };

  /// Set state controller gain
  void set_Kc(const Matrix& Kc) { Reassign(Kc_, Kc); };
  /// Set integral controller gain
  void set_Kc_inty(const Matrix& Kc_inty) { Reassign(Kc_inty_, Kc_inty); };
  /// Set input controller gain
  void set_Kc_u(const Matrix& Kc_u) { Reassign(Kc_u_, Kc_u); };
  /// Set time constant of anti-integral-windup
  void set_tau_awu(data_t tau) {
    tau_awu_ = tau;
    k_awu_ = sys_.dt() / tau_awu_;
  };

  /**
   * @brief      Sets the control type
   *
   * @param  control_type  control type bit mask
   *
   * @tparam     System        type derived from lds::System
   */
  void set_control_type(size_t control_type);

  // There is no reason u_lb/ub should not be public, but making set methods
  // anyway.
  /**
   * @brief      sets control lower bound
   *
   * @param      u_lb  control lower bound
   */
  void set_u_lb(data_t u_lb) { u_lb_ = u_lb; };

  /**
   * @brief      Sets control upper bound
   *
   * @param      u_ub  control upper bound
   */
  void set_u_ub(data_t u_ub) { u_ub_ = u_ub; };

  /// reset system and control variables.
  void Reset() {
    sys_.Reset();
    u_ref_.zeros();
    u_ref_prev_.zeros();
    int_e_.zeros();
    int_e_awu_adjust_.zeros();
    u_sat_.zeros();
    u_saturated_ = false;
    t_since_control_onset_ = 0.0;
  };

  /// prints variables to stdout
  void Print() {
    sys_.Print();
    std::cout << "g_design : " << g_design_ << "\n";
    std::cout << "u_lb : " << u_lb_ << "\n";
    std::cout << "u_ub : " << u_ub_ << "\n";
  };

 protected:
  System sys_;  ///< underlying LDS

  Vector u_;         ///< control signal
  Vector u_return_;  ///< control signal that is *returned* to user

  Vector g_design_;  ///< input gain of the system used for controller design

  //  reference signals
  Vector u_ref_;  ///< reference input
  // create no set method for this:
  Vector u_ref_prev_;  ///< reference input at previous time step
  Vector x_ref_;       ///< reference state
  Vector y_ref_;       ///< reference output
  Vector cx_ref_;

  // Controller gains
  Matrix Kc_;  ///< state controller gain
  Matrix
      Kc_u_;  ///< input controller gain (optional when control updates \deltaU)
  Matrix Kc_inty_;  ///< integral controller gain

  // control after g inversion
  // do not need set methods for these.
  Vector du_ref_;
  Vector dv_ref_;
  Vector v_ref_;
  Vector dv_;
  Vector v_;  ///< Control after g inversion (e.g., control in physical units)

  // integral error
  // do not need set method for this
  Vector int_e_;             ///< integrated error
  Vector int_e_awu_adjust_;  ///< anti-windup adjustment to intE
  Vector u_sat_;  ///< control signal after saturation (for antiWindup)

  bool do_control_prev_ = false;
  bool do_lock_control_prev_ = false;

  // whether the g of system has become inverted from what you think it is
  // (gain_ref)
  bool u_saturated_ =
      false;  ///< whether control signal has reached saturation limits

  // should be safe to have references here bc nothing needs to be done
  // (like reset vars) when it changes...
  data_t u_lb_{};  ///< lower bound on control
  data_t u_ub_{};  ///< upper bound on control

  data_t tau_awu_{};  ///< antiwindup time constant
  data_t k_awu_ = 0;

  data_t t_since_control_onset_ = 0;  ///< time since control epoch onset
  size_t control_type_{};             ///< controller type

 private:
  /**
   * @brief      calculates the control signal update (single-step)
   *
   * @param      do_control                 [optional] whether to update control
   *                                        (true) or simply feed through u_ref
   *                                        (false)
   * @param      do_estimation              [optional] whether to update state
   *                                        estimate (if false, effectively
   *                                        open-loop control)
   * @param      do_lock_control            [optional] whether to lock control
   *                                        at its current value
   * @param      sigma_soft_start           [optional] standard deviation
   *                                        (sigma) of a Gaussian soft-start to
   *                                        control (do_control from false to
   *                                        true)
   * @param      sigma_u_noise              [optional] standard deviation
   *                                        (sigma) of Gaussian noise added on
   *                                        top of control signal
   * @param      do_reset_at_control_onset  [optional] whether to reset
   *                                        controller at control epoch onset
   *                                        (i.e., do_control from false to
   *                                        true)
   */
  void CalcControl(bool do_control = true, bool do_estimation = true,
                   bool do_lock_control = false, data_t sigma_soft_start = 0,
                   data_t sigma_u_noise = 0,
                   bool do_reset_at_control_onset = true);

  /**
   * @brief      calculates rest of set-point (u_ref, x_ref) assuming goal is to
   *             track y_ref at steady state
   */
  void CalcSteadyStateSetPoint();

  /**
   * Performs saturation check on control signal and antiwindup adjustment of
   * integral error.
   *
   * Reference:
   *
   * Astroem KJ, Rundqwist L. (1989) Integrator Windup and How to Avoid It. 1989
   * ACC.
   *
   * @brief      performs saturation check on control signal and antiwindup
   *             adjustment of integral error
   */
  void AntiWindup();

  /// Initialize variables (common amongst user-defined constructors)
  void InitVars(size_t control_type);
};

// Implement the above:

template <typename System>
inline Controller<System>::Controller(const System& sys, data_t u_lb,
                                      data_t u_ub, size_t control_type)
    : sys_(sys),
      u_lb_(u_lb),
      u_ub_(u_ub),
      tau_awu_(lds::kInf) {
  InitVars(control_type);
}

template <typename System>
inline Controller<System>::Controller(System&& sys, data_t u_lb, data_t u_ub,
                                      size_t control_type)
    : sys_(std::move(sys)),
      u_lb_(u_lb),
      u_ub_(u_ub),
      tau_awu_(lds::kInf) {
  InitVars(control_type);
}

template <typename System>
inline void Controller<System>::set_control_type(size_t control_type) {
  if (control_type_ == control_type) {
    return;
  }

  // creating a blank slate...
  control_type_ = 0;
  Kc_inty_.zeros(0, 0);
  Kc_u_.zeros(0, 0);
  int_e_.zeros(0, 0);
  int_e_awu_adjust_.zeros(0, 0);

  // controller was designed to minimize integral error
  if (control_type & kControlTypeIntY) {
    Kc_inty_.zeros(sys_.n_u(), sys_.n_y());
    int_e_.zeros(sys_.n_y());
    int_e_awu_adjust_.zeros(sys_.n_u());
    control_type_ = control_type_ | kControlTypeIntY;
  }

  // controller was designed to minimize deltaU
  // (i.e. state augmented with u)
  if (control_type & kControlTypeDeltaU) {
    Kc_u_.zeros(sys_.n_u(), sys_.n_u());
    control_type_ = control_type_ | kControlTypeDeltaU;
  }

  // whether to adapt set point calculate with (re-estimated) process
  // disturbance (m)
  if (control_type & kControlTypeAdaptM) {
    if (sys_.do_adapt_m)  // only if adapting m...
    {
      control_type_ = control_type_ | kControlTypeAdaptM;
    }
  }
}  // set_control_type

template <typename System>
inline const Vector& Controller<System>::Control(
    const Vector& z, bool do_control, bool do_lock_control,
    data_t sigma_soft_start, data_t sigma_u_noise,
    bool do_reset_at_control_onset) {
  // update state estimates, given latest measurement
  sys_.Filter(u_, z);

  bool do_estimation = true;  // always have estimator on in this case

  // calculate control signal
  CalcControl(do_control, do_estimation, do_lock_control, sigma_soft_start,
              sigma_u_noise, do_reset_at_control_onset);

  return u_return_;
}

template <typename System>
inline const Vector& Controller<System>::ControlOutputReference(
    const Vector& z, bool do_control, bool do_estimation, bool do_lock_control,
    data_t sigma_soft_start, data_t sigma_u_noise,
    bool do_reset_at_control_onset) {
  // update state estimates, given latest measurement
  if (do_estimation) {
    sys_.Filter(u_, z);
  } else {
    sys_.f(u_);
  }

  // calculate the set point
  // solves for u_ref and x_ref when output is at y_ref at steady state.
  if (do_control) {
    CalcSteadyStateSetPoint();
  }

  // calculate control signal
  CalcControl(do_control, do_estimation, do_lock_control, sigma_soft_start,
              sigma_u_noise, do_reset_at_control_onset);

  return u_return_;
}

template <typename System>
inline void Controller<System>::CalcControl(bool do_control, bool do_estimation,
                                            bool do_lock_control,
                                            data_t sigma_soft_start,
                                            data_t sigma_u_noise,
                                            bool do_reset_at_control_onset) {
  if (do_control && do_estimation) {
    if (!do_control_prev_) {
      if (do_reset_at_control_onset) {
        Reset();
      }
      t_since_control_onset_ = 0.0;
    } else {
      t_since_control_onset_ += sys_.dt();
    }

    // enforce softstart on control vars.
    if (sigma_soft_start > 0) {
      // half-Gaussian soft-start scaling factor
      data_t soft_start_sf = 1 - exp(-pow(t_since_control_onset_, 2) /
                                     (2 * pow(sigma_soft_start, 2)));
      u_ref_ *= soft_start_sf;
      // TODO(mfbolus): May be appropriate to soft-start x_ref, y_ref too
      // x_ref_ *= soft_start_sf;
      // cx_ref_ *= soft_start_sf;
      // y_ref_ *= soft_start_sf;
    }

    if (!do_lock_control) {
      // first do u -> v change of vars. (v = g.*u)
      // e.g., convert into physical units (e.g., v[=] mW/mm2 rather than driver
      // control voltage u[=]V)
      v_ref_ = g_design_ % u_ref_;

      // Given FB, calc. the change in control
      if (control_type_ & kControlTypeDeltaU) {
        // if control designed to minimize not u but deltaU (i.e. state aug with
        // u):

        // TODO(mfbolus): Commented out for now. See note below.
        // du_ref_ = u_ref_ - u_ref_prev_;
        // dv_ref_ = g_design_ % du_ref_;

        // TODO(mfbolus): Assuming users want *smooth* control signals if using
        // kControlTypeDeltaU, it should be the case that dv_ref_ is --> 0. May
        // want to revisit, but I am going to force it to be zero unless a
        // situation arises that argues for keeping the above.
        dv_ref_.zeros();

        dv_ = dv_ref_;                     // nominally-optimal.
        dv_ -= Kc_ * (sys_.x() - x_ref_);  // instantaneous state error
        dv_ -= Kc_u_ * (v_ - v_ref_);      // penalty on amp u (rel to ref)

        if (control_type_ & kControlTypeIntY) {
          // TODO(mfbolus): one approach to protection against integral windup
          // would be to not integrate error when control signal saturated:

          // if(!uSaturated)
          int_e_ += (sys_.cx() - cx_ref_) * sys_.dt();  // integrated error
          dv_ -= Kc_inty_ * int_e_;  // control for integrated error
        }

        // update the control
        v_ += dv_;
      } else {
        v_ = v_ref_;                      // nominally-optimal.
        v_ -= Kc_ * (sys_.x() - x_ref_);  // instantaneous state error

        if (control_type_ & kControlTypeIntY) {
          // TODO(mfbolus): one approach to protection against integral windup
          // would be to not integrate error when control signal saturated:

          // if (!uSaturated)
          int_e_ += (sys_.cx() - cx_ref_) * sys_.dt();  // integrated error
          v_ -= Kc_inty_ * int_e_;  // control for integrated error
        }
      }

      // convert back to control voltage u[=]V
      u_ = v_ / sys_.g();
    }       // else do nothing until lock is low
  } else {  // if not control
    // feed through u_ref in open loop
    u_ = u_ref_ % g_design_ / sys_.g();
    v_ = sys_.g() % u_;
    u_ref_.zeros();
    int_e_.zeros();
    int_e_awu_adjust_.zeros();
    u_sat_.zeros();
  }  // ends do_control

  // enforce box constraints (and antiwindup)
  AntiWindup();

  // add noise to input?
  // The value for u that is *returned* to user after addition of any noise,
  // while keeping controller/estimator blind to this addition.
  u_return_ = u_;
  if ((sigma_u_noise > 0.0) && (do_control && !do_lock_control)) {
    u_return_ += sigma_u_noise * Vector(sys_.n_u(), fill::randn);
    Limit(u_return_, u_lb_, u_ub_);
  };

  // For next time step:
  u_ref_prev_ = u_ref_;
  do_control_prev_ = do_control;
  do_lock_control_prev_ = do_lock_control;
}  // CalcControl

template <typename System>
inline void Controller<System>::CalcSteadyStateSetPoint() {
  // Linearly-constrained least squares (ls).
  //
  // _reference:
  //  Boyd & Vandenberghe (2018) Introduction to Applied Linear Algebra
  //
  Matrix a_ls =
      join_horiz(sys_.C(), Matrix(sys_.n_y(), sys_.n_u(), fill::zeros));
  Vector b_ls = cx_ref_;
  Matrix c_ls = join_horiz(sys_.A() - Matrix(sys_.n_x(), sys_.n_x(), fill::eye),
                           sys_.B() * arma::diagmat(sys_.g()));
  Vector d_ls = -sys_.m0();
  if (control_type_ & kControlTypeAdaptM) {
    d_ls = -sys_.m();  // adapt setpoint calc with disturbance?
  }

  Matrix a_ls_t = a_ls.t();  // TODO(mfbolus): not sure why but causes seg
                             // fault if I do not do this.
  Matrix phi_ls =
      join_vert(join_horiz(2 * a_ls_t * a_ls, c_ls.t()),
                join_horiz(c_ls, Matrix(sys_.n_x(), sys_.n_x(), fill::zeros)));
  // TODO(mfbolus): should be actual inverse, rather than pseudo-inverse:
  Matrix inv_phi = pinv(phi_ls);
  Vector xulam = inv_phi * join_vert(2 * a_ls_t * b_ls, d_ls);
  x_ref_ = xulam.subvec(0, sys_.n_x() - 1);
  u_ref_ = xulam.subvec(sys_.n_x(), sys_.n_x() + sys_.n_u() - 1);
  cx_ref_ = sys_.C() * x_ref_;
}  // CalcSteadyStateSetPoint

template <typename System>
void Controller<System>::AntiWindup() {
  u_saturated_ = false;
  u_sat_ = u_;

  // limit u and flag whether saturated
  for (size_t k = 0; k < u_.n_elem; k++) {
    if (u_[k] < u_lb_) {
      u_sat_[k] = u_lb_;
      u_saturated_ = true;
    }

    if (u_[k] > u_ub_) {
      u_sat_[k] = u_ub_;
      u_saturated_ = true;
    }
  }

  if ((control_type_ & kControlTypeIntY) && (tau_awu_ < lds::kInf)) {
    // one-step back-calculation (calculate intE for u=u_sat)
    // (Astroem, Rundqwist 1989 warn against using this...)
    // int_e_awu_adjust_ =
    //     solve(Kc_inty_, (u_ - u_sat_));  // pinv(Kc_inty) * (u-uSat);

    // gradual: see Astroem, Rundqwist 1989
    // this is a fudge for doing MIMO gradual
    // n.b., went ahead and multiplied 1/T by dt so don't have to do that here.
    int_e_awu_adjust_ =
        k_awu_ * (sign(Kc_inty_).t() / sys_.n_u()) * (u_ - u_sat_);
    // int_e_awu_adjust_ = k_awu_ * (u_-u_sat_);

    int_e_ += int_e_awu_adjust_;
  }

  // set u to saturated version
  u_ = u_sat_;
}

template <typename System>
void Controller<System>::InitVars(size_t control_type) {
  // initialize to default values
  u_ref_ = Vector(sys_.n_u(), fill::zeros);
  u_ref_prev_ = Vector(sys_.n_u(), fill::zeros);
  x_ref_ = Vector(sys_.n_x(), fill::zeros);
  y_ref_ = Vector(sys_.n_y(), fill::zeros);
  cx_ref_ = Vector(sys_.n_y(), fill::zeros);

  u_ = Vector(sys_.n_u(), fill::zeros);
  u_return_ = Vector(sys_.n_u(), fill::zeros);
  u_sat_ = Vector(sys_.n_u(), fill::zeros);

  // Might not need all these, so zero elements until later.
  Kc_ = Matrix(sys_.n_u(), sys_.n_x(), fill::zeros);
  Kc_u_ = Matrix(0, 0, fill::zeros);
  Kc_inty_ = Matrix(0, 0, fill::zeros);

  g_design_ = sys_.g();  // by default, same as model
  dv_ = Vector(sys_.n_u(), fill::zeros);
  v_ = Vector(sys_.n_u(), fill::zeros);
  du_ref_ = Vector(sys_.n_u(), fill::zeros);
  dv_ref_ = Vector(sys_.n_u(), fill::zeros);
  v_ref_ = Vector(sys_.n_u(), fill::zeros);

  int_e_ = Vector(0, fill::zeros);
  int_e_awu_adjust_ = Vector(0, fill::zeros);

  set_control_type(control_type);
}

}  // namespace lds

#endif
