//===-- ldsCtrlEst_h/lds_gaussian_ctrl.h - GLDS Controller ------*- C++ -*-===//
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
/// This file declares and partially defines the type for control of a
/// gaussian-observation linear dynamical system (lds::gaussian::Controller). It
/// inherits functionality from the underlying GLDS model type
/// (lds::gaussian::System), including state estimation.
///
/// \brief GLDS Controller
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_GAUSSIAN_CTRL_H
#define LDSCTRLEST_LDS_GAUSSIAN_CTRL_H

// namespace
#include "lds_gaussian.h"
// system
#include "lds_gaussian_sys.h"
// controller
#include "lds_ctrl.h"

namespace lds {
namespace gaussian {
/// Gaussian-observation Controller Type
class Controller : public lds::Controller<System> {
 public:
  /// sets reference output
  void set_y_ref(const Vector& y_ref) override {
    Reassign(y_ref_,y_ref);
    cx_ref_ = y_ref - sys_.d();
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

  using lds::Controller<System>::set_sys;
  using lds::Controller<System>::set_g_design;
  using lds::Controller<System>::set_u_ref;
  using lds::Controller<System>::set_x_ref;
  using lds::Controller<System>::set_y_ref;
  using lds::Controller<System>::set_Kc;
  using lds::Controller<System>::set_Kc_inty;
  using lds::Controller<System>::set_Kc_u;
  using lds::Controller<System>::set_tau_awu;
  using lds::Controller<System>::set_control_type;

  using lds::Controller<System>::Reset;
  using lds::Controller<System>::Print;
};
}  // namespace gaussian
}  // namespace lds

#endif
