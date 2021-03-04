//===-- lds_gaussian_sctrl.cpp - Switched Controller ----------------------===//
//
// Copyright 2021 [name of copyright owner]
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
/// This file implements the type for switched feedback control of a system
/// approximated as multiple discrete Gaussian-output linear dynamical systems
/// (`lds::gaussian::sctrl_t`).
///
/// \brief GLDS switched controller type
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>

using namespace std;
using namespace glds;

glds::sctrl_t::sctrl_t(size_t nSys, size_t nU, size_t nX, size_t nY,
                       data_t& uLB, data_t& uUB, data_t& dt, data_t& p0,
                       data_t& q0, data_t& r0, size_t controlType)
    : glds::ctrl_t(nU, nX, nY, uLB, uUB, dt, p0, q0, r0, controlType),
      nSys(nSys),
      sysIdx(0) {
  // initial wrapper controller
  // (will hold parameters of toggled underlying systems set by `setSystem`)
  ctrl_t sys0(nU, nX, nY, uLB, uUB, dt, p0, q0, r0, controlType);
  systems = std::vector<ctrl_t>(nSys, sys0);
  adaptM = false;
}

void glds::sctrl_t::setSystem(size_t sysIdx, ctrl_t& sys) {
  // Make sure system Idx in bounds
  if ((sysIdx + 1) > nSys) {
    cerr << "Provided system index is not consistent with switched system! "
            "Skipping."
         << endl;
    return;
  }

  // check to make sure dimensions correct.
  if ((sys.getNx() == nX) & (sys.getNu() == nU) & (sys.getNy() == nY))
    systems[sysIdx] = sys;
  else
    cerr << "System dimensions are not consistent with switched system! "
            "Skipping."
         << endl;

  // assign to parent
  this->sysIdx = sysIdx + 1;  // this forces switchSystem call to switch
  switchSystem(sysIdx);
}

void glds::sctrl_t::switchSystem(size_t sysIdx) {
  // Make sure system Idx in bounds
  if ((sysIdx + 1) > nSys) {
    cerr << "Provided system index is not consistent with switched system! "
            "Skipping."
         << endl;
    return;
  }

  if (sysIdx == this->sysIdx) return;  // already there.

  if (adaptM) {
    // Need to recalc equivalent disturbance for new dynamics [A]

    // TODO(mfbolus): This is a fudge. It is the disturbance such that the next
    // time step prediction should be the same for both systems.
    m += (A - systems[sysIdx].getA()) * x;
  } else
    m = systems[sysIdx].getM0();

  // Necessary params.
  A = systems[sysIdx].getA();
  B = systems[sysIdx].getB();
  Q = systems[sysIdx].getQ();
  Q_m = systems[sysIdx].getQ_m();
  C = systems[sysIdx].getC();
  d = systems[sysIdx].getD();
  x0 = systems[sysIdx].getX0();
  m0 = systems[sysIdx].getM0();
  P0 = systems[sysIdx].getP0();
  P0_m = systems[sysIdx].getP0_m();

  R = systems[sysIdx].getR();
  Ke = systems[sysIdx].getKe();
  Ke_m = systems[sysIdx].getKe_m();

  size_t controlType = systems[sysIdx].getControlType();
  setControlType(controlType);
  Kc_u = systems[sysIdx].getKc_u();
  Kc_x = systems[sysIdx].getKc_x();
  Kc_inty = systems[sysIdx].getKc_inty();

  // TODO(mfbolus): Keep the below commented out. Currently references and input
  // gains are set at the parent system level and not switched between systems.
  // It is conceivable input gains should be varied between systems, but that
  // can be accounted for in `B`, so keeping this a parent-level parameter for
  // now.

  // g = systems[sysIdx].getG();
  // gDesign = systems[sysIdx].getGDesign();

  this->sysIdx = sysIdx;
}

void glds::sctrl_t::setControlType(size_t controlType) {
  if (this->controlType == controlType) return;

  // creating a blank slate... (perhaps unnecessary)
  this->controlType = 0;
  Kc_u.zeros(0, 0);
  Kc_inty.zeros(0, 0);
  intE.zeros(0, 0);
  intE_awuAdjust.zeros(0, 0);

  // controller was designed to minimize deltaU
  // (i.e. state augmented with u)
  if (controlType & CONTROL_TYPE_U) {
    Kc_u.zeros(nU, nU);
    this->controlType = this->controlType | CONTROL_TYPE_U;
  }

  // controller was designed to minimize integral error
  if (controlType & CONTROL_TYPE_INTY) {
    Kc_inty.zeros(nU, nY);
    intE.zeros(nY);
    intE_awuAdjust.zeros(nY);
    this->controlType = this->controlType | CONTROL_TYPE_INTY;
  }

  // whether to adapt set point calculate with (re-estimated) process
  // disturbance (m)
  if (controlType & CONTROL_TYPE_ADAPT_M) {
    if (this->adaptM)  // only if adapting M...
      this->controlType = this->controlType | CONTROL_TYPE_ADAPT_M;
  }
}
