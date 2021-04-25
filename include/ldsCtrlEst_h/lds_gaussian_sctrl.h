//===-- ldsCtrlEst_h/lds_gaussian_sctrl.h - Switched Controller -*- C++ -*-===//
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
/// This file declares and partially defines the type for switched feedback
/// control of a system approximated as multiple discrete Gaussian-output linear
/// dynamical systems
/// (lds::gaussian::SwitchedController).
///
/// \brief GLDS switched controller type
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_GAUSSIAN_SCTRL_H
#define LDSCTRLEST_LDS_GAUSSIAN_SCTRL_H

// controller type
#include "lds_gaussian_ctrl.h"
// switched controller
#include "lds_sctrl.h"

namespace lds {
namespace gaussian {
/// Gaussian-observation SwitchedController Type
class SwitchedController : public lds::SwitchedController<System> {
 public:
  /// sets reference output
  void set_y_ref(const Vector& y_ref) override {
    Reassign(y_ref_, y_ref);
    cx_ref_ = y_ref - sys_.d();
  }

  // make sure base class template methods available
  using lds::SwitchedController<System>::SwitchedController;
  using lds::SwitchedController<System>::Switch;
  using lds::SwitchedController<System>::Control;
  using lds::SwitchedController<System>::ControlOutputReference;

  using lds::SwitchedController<System>::sys;
  using lds::SwitchedController<System>::Kc;
  using lds::SwitchedController<System>::Kc_inty;
  using lds::SwitchedController<System>::Kc_u;
  using lds::SwitchedController<System>::g_design;
  using lds::SwitchedController<System>::u_ref;
  using lds::SwitchedController<System>::x_ref;
  using lds::SwitchedController<System>::y_ref;
  using lds::SwitchedController<System>::control_type;

  using lds::SwitchedController<System>::set_g_design;
  using lds::SwitchedController<System>::set_u_ref;
  using lds::SwitchedController<System>::set_x_ref;
  using lds::SwitchedController<System>::set_y_ref;
  using lds::SwitchedController<System>::set_Kc;
  using lds::SwitchedController<System>::set_Kc_inty;
  using lds::SwitchedController<System>::set_Kc_u;
  using lds::SwitchedController<System>::set_tau_awu;
  using lds::SwitchedController<System>::set_control_type;

  using lds::SwitchedController<System>::Reset;
  using lds::SwitchedController<System>::Print;
};  // SwitchedController
}  // namespace gaussian
}  // namespace lds

#endif
