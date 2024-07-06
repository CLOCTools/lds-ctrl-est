//===-- ldsCtrlEst_h/lds_gaussian_mpcctrl.h - PLDS MPC Controller ------*- C++ -*-===//
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
/// This file declares the type for model predictive Model Predictive
/// Control (MPC) on linear system dynamics by converting the MPC optimization
/// problem to a quadratic cost problem. The new problem is optimized using the
/// Operator Splitting Quadratic Program (OSQP).
///
/// \brief MPC Controller
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_POISSON_MPCCTRL_H
#define LDSCTRLEST_LDS_POISSON_MPCCTRL_H

// namespace
#include "lds_poisson.h"
// system
#include "lds_poisson_sys.h"
// controller
#include "lds_mpcctrl.h"

namespace lds {
namespace poisson {
/// Poisson-observation MPC Controller Type
class MpcController : public lds::MpcController<System> {
 public:

  // make sure base class template methods available
  using lds::MpcController<System>::MpcController;
  using lds::MpcController<System>::Control;
  using lds::MpcController<System>::ControlOutputReference;

  // getters
  using lds::MpcController<System>::sys;

  // setters
  using lds::MpcController<System>::set_control;
  using lds::MpcController<System>::set_control_output;
  using lds::MpcController<System>::set_constraint;

  using lds::MpcController<System>::Print;
};
} // namespace poisson
} // namespace lds

#endif  // LDSCTRLEST_LDS_POISSON_MPCCTRL_H
