//===-- ldsCtrlEst_h/lds_sctrl.h - Switched Controller ----------*- C++ -*-===//
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
/// This file declares the type for switched control of a system approximated as
/// multiple discrete Gaussian-output linear dynamical systems
/// (lds::gaussian::SwitchedController).
///
/// \brief SwitchedController type
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_SCTRL_H
#define LDSCTRLEST_LDS_SCTRL_H

#include "lds_ctrl.h"
#include "lds_uniform_mats.h"
#include "lds_uniform_vecs.h"

namespace lds {
/// SwitchedController Type
template <typename System>
class SwitchedController : public Controller<System> {
 public:
  /**
   * @brief      Constructs a new SwitchedController.
   */
  SwitchedController() = default;

  /**
   * @brief      Constructs a new SwitchedController.
   *
   * @param      systems       vector of sub-systems
   * @param      u_lb          lower bound on control (u)
   * @param      u_ub          upper bound on control (u)
   * @param      control_type  [optional] control type bit mask
   */
  SwitchedController(const std::vector<System>& systems, data_t u_lb,
                     data_t u_ub, size_t control_type = 0);

  /**
   * @brief      Constructs a new SwitchedController (moves systems).
   *
   * @param      systems       vector of sub-systems
   * @param      u_lb          lower bound on control (u)
   * @param      u_ub          upper bound on control (u)
   * @param      control_type  [optional] control type bit mask
   */
  SwitchedController(std::vector<System>&& systems, data_t u_lb, data_t u_ub,
                     size_t control_type = 0);

  /**
   * @brief      Switch to a different sub-system/controller
   *
   * @param      idx              index
   * @param      do_force_switch  whether to force a system switch even if
   *                              already there.
   */
  void Switch(size_t idx, bool do_force_switch = false);

  /// sets state feedback gains
  void set_Kc(const UniformMatrixList<>& Kc) {
    Kc_list_ = Kc;
    Kc_ = Kc_list_.at(0);  // set to first
    if (idx_ != 0) {
      Switch(idx_, true);
    }
  };
  /// sets state feedback gains (moving)
  void set_Kc(UniformMatrixList<>&& Kc) {
    Kc_list_ = std::move(Kc);
    Kc_ = Kc_list_.at(0);  // set to first
    if (idx_ != 0) {
      Switch(idx_, true);
    }
  };

  /// sets integral feedback gains
  void set_Kc_inty(const UniformMatrixList<>& Kc_inty) {
    Kc_inty_list_ = Kc_inty;
    Kc_inty_ = Kc_inty_list_.at(0);  // set to first
    if (idx_ != 0) {
      Switch(idx_, true);
    }
  };
  /// sets integral feedback gains (moving)
  void set_Kc_inty(UniformMatrixList<>&& Kc_inty) {
    Kc_inty_list_ = std::move(Kc_inty);
    Kc_inty_ = Kc_inty_list_.at(0);  // set to first
    if (idx_ != 0) {
      Switch(idx_, true);
    }
  };

  /// sets input feedback gains
  void set_Kc_u(const UniformMatrixList<>& Kc_u) {
    Kc_u_list_ = Kc_u;
    Kc_u_ = Kc_u_list_.at(0);  // set to first
    if (idx_ != 0) {
      Switch(idx_, true);
    }
  };
  /// sets input feedback gains (moving)
  void set_Kc_u(UniformMatrixList<>&& Kc_u) {
    Kc_u_list_ = std::move(Kc_u);
    Kc_u_ = Kc_u_list_.at(0);  // set to first
    if (idx_ != 0) {
      Switch(idx_, true);
    }
  };

  /// sets input gain used during controller design
  void set_g_design(const UniformVectorList& g) {
    g_design_list_ = g;
    g_design_ = g_design_list_.at(0);  // set to first
    if (idx_ != 0) {
      Switch(idx_, true);
    }
  };
  /// sets input gain used during controller design (moving)
  void set_g_design(UniformVectorList&& g) {
    g_design_list_ = std::move(g);
    g_design_ = g_design_list_.at(0);  // set to first
    if (idx_ != 0) {
      Switch(idx_, true);
    }
  };

  // make sure base class template methods available
  using lds::Controller<System>::Controller;
  using lds::Controller<System>::Control;
  using lds::Controller<System>::ControlOutputReference;

  using lds::Controller<System>::sys;
  using lds::Controller<System>::Kc;
  using lds::Controller<System>::Kc_inty;
  using lds::Controller<System>::Kc_u;
  using lds::Controller<System>::g_design;
  using lds::Controller<System>::u_ref;
  using lds::Controller<System>::x_ref;
  using lds::Controller<System>::y_ref;
  using lds::Controller<System>::control_type;

  using lds::Controller<System>::set_u_ref;
  using lds::Controller<System>::set_x_ref;
  using lds::Controller<System>::set_y_ref;
  using lds::Controller<System>::set_tau_awu;
  using lds::Controller<System>::set_control_type;

  using lds::Controller<System>::Reset;
  using lds::Controller<System>::Print;

 protected:
  std::vector<System>
      systems_;     ///< underlying sub-systems which are switched between
  size_t n_sys_{};  ///< number of systems
  size_t idx_{};    ///< current system/controller index.

  // controller gains could be different for each
  UniformMatrixList<> Kc_list_;
  UniformMatrixList<> Kc_inty_list_;
  UniformMatrixList<> Kc_u_list_;

  // design-phase input gain could also be different
  UniformVectorList g_design_list_;

  // TODO(mfbolus): not sure why I need to do this.
  using Controller<System>::Kc_;
  using Controller<System>::Kc_inty_;
  using Controller<System>::Kc_u_;
  using Controller<System>::g_design_;
  using Controller<System>::sys_;
  // using Controller<System>::u_ref_;
  // using Controller<System>::x_ref_;
  // using Controller<System>::y_ref_;
  //
  using Controller<System>::control_type_;

 private:
  void InitVars();

  using lds::Controller<System>::set_sys;
  // using Controller<System>::set_Kc;
  // using Controller<System>::set_Kc_inty;
  // using Controller<System>::set_Kc_u;
  // using Controller<System>::set_g_design;
};

template <typename System>
inline SwitchedController<System>::SwitchedController(
    const std::vector<System>& systems, data_t u_lb, data_t u_ub,
    size_t control_type)
    : Controller<System>(systems.at(0), u_lb, u_ub, control_type),
      systems_(systems) {
  InitVars();
}

template <typename System>
inline SwitchedController<System>::SwitchedController(
    std::vector<System>&& systems, data_t u_lb, data_t u_ub,
    size_t control_type)
    : Controller<System>(System(systems.at(0).n_u(), systems.at(0).n_x(),
                                systems.at(0).n_y(), systems.at(0).dt()),
                         u_lb, u_ub, control_type),
      systems_(std::move(systems)) {
  InitVars();
}

template <typename System>
inline void SwitchedController<System>::InitVars() {
  n_sys_ = systems_.size();
  sys_ = systems_.at(0);

  Kc_list_ = UniformMatrixList<>(std::vector<Matrix>(n_sys_, Kc_));
  Kc_inty_list_ = UniformMatrixList<>(std::vector<Matrix>(n_sys_, Kc_inty_));
  Kc_u_list_ = UniformMatrixList<>(std::vector<Matrix>(n_sys_, Kc_inty_));
  g_design_list_ = UniformVectorList(std::vector<Vector>(n_sys_, g_design_));
}

template <typename System>
inline void SwitchedController<System>::Switch(size_t idx,
                                               bool do_force_switch) {
  if ((idx == idx_) && !do_force_switch) {
    return;  // already there.
  }

  // put old up and get new one out
  systems_.at(idx_) = std::move(sys_);
  sys_ = std::move(systems_.at(idx));

  // set the state of this system to that of the previous system
  // TODO(mfbolus): This will only work as intended if state matrix is the same.
  // See example fudge in 0.4 branch src/lds_poisson_sctrl.cpp.
  sys_.set_m(systems_.at(idx_).m(), true);
  sys_.set_x(systems_.at(idx_).x());

  // swap controller gains
  Kc_list_.Swap(Kc_, idx_);
  Kc_list_.Swap(Kc_, idx);

  if (control_type_ & kControlTypeIntY) {
    Kc_inty_list_.Swap(Kc_inty_, idx_);
    Kc_inty_list_.Swap(Kc_inty_, idx);
  }

  if (control_type_ & kControlTypeDeltaU) {
    Kc_u_list_.Swap(Kc_u_, idx_);
    Kc_u_list_.Swap(Kc_u_, idx);
  }

  g_design_list_.Swap(g_design_, idx_);
  g_design_list_.Swap(g_design_, idx);

  idx_ = idx;
}  // Switch

}  // namespace lds

#endif
