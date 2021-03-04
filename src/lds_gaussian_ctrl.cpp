//===-- lds_gaussian_ctrl.cpp - GLDS Controller ---------------------------===//
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
/// This file implements the type for feedback control of a gaussian-output
/// linear dynamical system (`lds::gaussian::ctrl_t`). It inherits functionality
/// from the underlying GLDS model type (`lds::gaussian::sys_t`), including
/// state estimation.
///
/// \brief GLDS controller type
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>

using namespace std;
using namespace glds;

// Constructor(s) for sys class
glds::ctrl_t::ctrl_t(size_t nU, size_t nX, size_t nY, data_t& uLB, data_t& uUB,
                     data_t& dt, data_t& p0, data_t& q0, data_t& r0,
                     size_t controlType)
    : glds::sys_t(nU, nX, nY, dt, p0, q0, r0),
      uLB(uLB),
      uUB(uUB),
      gateCtrl_prev(false),
      gateLock_prev(false),
      uSaturated(false),
      tauAntiWindup(lds::inf),
      t_since_ctrl_onset(0.0) {
  // initialize to default values
  uRef = armaVec(nU, fill::zeros);
  uRef_prev = armaVec(nU, fill::zeros);
  xRef = armaVec(nX, fill::zeros);
  yRef = armaVec(nY, fill::zeros);

  uSat = armaVec(nU, fill::zeros);

  // Might not need all these, so zero elements until later.
  Kc_x = armaMat(nU, nX, fill::zeros);
  Kc_u = armaMat(0, 0, fill::zeros);
  Kc_inty = armaMat(0, 0, fill::zeros);

  gDesign = g;
  dv = armaVec(nU, fill::zeros);
  v = armaVec(nU, fill::zeros);
  duRef = armaVec(nU, fill::zeros);
  dvRef = armaVec(nU, fill::zeros);
  vRef = armaVec(nU, fill::zeros);

  intE = armaVec(0, fill::zeros);
  intE_awuAdjust = armaVec(0, fill::zeros);
  kAntiWindup = dt / tauAntiWindup;

  setControlType(controlType);
}

void glds::ctrl_t::fbCtrl(armaVec& z, bool& gateCtrl, bool& gateLock,
                          data_t& sigma_softStart, data_t& sigma_uNoise,
                          bool& resetAtCtrlOnset, bool& doRecurse_Ke) {
  // update state estimates, given latest measurement
  filter(z, doRecurse_Ke);

  bool gateEst = true;  // always have estimator on in this case

  // calculate control signal
  calc_ctrl(gateCtrl, gateEst, gateLock, sigma_softStart, sigma_uNoise,
            resetAtCtrlOnset);
}

void glds::ctrl_t::steadyState_fbCtrl(armaVec& z, bool& gateCtrl, bool& gateEst,
                                      bool& gateLock, data_t& sigma_softStart,
                                      data_t& sigma_uNoise,
                                      bool& resetAtCtrlOnset,
                                      bool& doRecurse_Ke) {
  // update state estimates, given latest measurement
  if (gateEst) {
    filter(z, doRecurse_Ke);
  } else {
    predict();
  }

  // calculate the set point @ steady state.
  if (gateCtrl) calc_ssSetPt();

  // calculate control signal
  calc_ctrl(gateCtrl, gateEst, gateLock, sigma_softStart, sigma_uNoise,
            resetAtCtrlOnset);
}  // steadyState_fbCtrl

// private method for meat of control logic
void glds::ctrl_t::calc_ctrl(bool& gateCtrl, bool& gateEst, bool& gateLock,
                             data_t& sigma_softStart, data_t& sigma_uNoise,
                             bool& resetAtCtrlOnset) {
  if (gateCtrl && gateEst) {
    if (!gateCtrl_prev) {
      if (resetAtCtrlOnset) {
        reset();
      }
      t_since_ctrl_onset = 0.0;
    } else {
      t_since_ctrl_onset += dt;
    }

    // enforce softstart on control vars.
    if (sigma_softStart > 0) {
      // half-Gaussian soft-start scaling factor
      data_t softStart_sf =
          1 - exp(-pow(t_since_ctrl_onset, 2) / (2 * pow(sigma_softStart, 2)));
      uRef *= softStart_sf;
      // TODO(mfbolus): May be appropriate to soft-start set point xRef, yRef
      // too
      // xRef *= softStart_sf;
      // yRef *= softStart_sf;
    }

    if (!gateLock) {
      duRef = uRef - uRef_prev;

      // first do u -> v change of vars. (v = g.*u)
      // e.g., convert into physical units (e.g., v[=] mW/mm2 rather than driver
      // control voltage u[=]V)
      vRef = gDesign % uRef;
      dvRef = gDesign % duRef;

      // Given FB, calc. the change in control
      if (controlType & CONTROL_TYPE_U) {
        // if control designed to minimize not u but deltaU (i.e. state aug with
        // u):
        dv = dvRef;                    // nominally-optimal.
        dv -= Kc_x * (getX() - xRef);  // instantaneous state error
        dv -= Kc_u * (v - vRef);       // penalty on changes in u

        if (controlType & CONTROL_TYPE_INTY) {
          // TODO(mfbolus): one approach to protection against integral windup
          // would be to not integrate error when control signal saturated:

          // if(!uSaturated)
          intE += (y - yRef) * dt;  // integrated error
          dv -= Kc_inty * intE;     // control for integrated error
        }

        // update the control
        v += dv;
      } else {
        v = vRef;                     // nominally-optimal.
        v -= Kc_x * (getX() - xRef);  // instantaneous state error

        if (controlType & CONTROL_TYPE_INTY) {
          // TODO(mfbolus): one approach to protection against integral windup
          // would be to not integrate error when control signal saturated:

          // if (!uSaturated)
          intE += (y - yRef) * dt;  // integrated error
          v -= Kc_inty * intE;      // control for integrated error
        }
      }

      // convert back to control voltage u[=]V
      u = v / getG();
    }  // else do nothing until lock is low

    // It may be desireable to make inputs more variable.
    if (sigma_uNoise > 0.0) u += sigma_uNoise * armaVec(nU, fill::randn);

  } else {  // if not control
    // feed through uRef in open loop
    u = uRef % gDesign / getG();
    uRef.zeros();
    intE.zeros();
    intE_awuAdjust.zeros();
    uSat.zeros();
  }  // ends gateCtrl

  // enforce box constraints (and antiwindup)
  antiWindup();

  gateCtrl_prev = gateCtrl;
  gateLock_prev = gateLock;
}  // ends calc_ctrl

void glds::ctrl_t::setControlType(size_t controlType) {
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

// set methods:
// make sure cannot user-define value of u.
void glds::ctrl_t::setU(stdVec& uVec) {
  cerr << "Cannot assign a value to `u` in GLDS controller.\n";
}
void glds::ctrl_t::setU(armaVec& u) {
  cerr << "Cannot assign a value to `u` in GLDS controller.\n";
}

void glds::ctrl_t::setG(stdVec& gVec) { glds::sys_t::setG(gVec); }
void glds::ctrl_t::setG(armaVec& g) { glds::sys_t::setG(g); }

void glds::ctrl_t::setGDesign(stdVec& gVec) { reassign(gDesign, gVec); }
void glds::ctrl_t::setGDesign(armaVec& g) { reassign(gDesign, g); }

void glds::ctrl_t::setURef(stdVec& uRefVec) {
  uRef_prev = uRef;
  reassign(uRef, uRefVec);
}
void glds::ctrl_t::setURef(armaVec& uRef) {
  uRef_prev = this->uRef;
  reassign(this->uRef, uRef);
}

void glds::ctrl_t::setXRef(stdVec& xRefVec) { reassign(xRef, xRefVec); }
void glds::ctrl_t::setXRef(armaVec& xRef) { reassign(this->xRef, xRef); }

void glds::ctrl_t::setYRef(stdVec& yRefVec) { reassign(yRef, yRefVec); }
void glds::ctrl_t::setYRef(armaVec& yRef) { reassign(this->yRef, yRef); }

void glds::ctrl_t::setKc_x(stdVec& Kc_x_vec) { reassign(Kc_x, Kc_x_vec); }
void glds::ctrl_t::setKc_x(armaMat& Kc_x) { reassign(this->Kc_x, Kc_x); }

void glds::ctrl_t::setKc_u(stdVec& Kc_u_vec) {
  if (controlType & CONTROL_TYPE_U) reassign(Kc_u, Kc_u_vec);
}
void glds::ctrl_t::setKc_u(armaMat& Kc_u) {
  if (controlType & CONTROL_TYPE_U) reassign(this->Kc_u, Kc_u);
}

void glds::ctrl_t::setKc_inty(stdVec& Kc_inty_vec) {
  if (controlType & CONTROL_TYPE_INTY) reassign(Kc_inty, Kc_inty_vec);
}
void glds::ctrl_t::setKc_inty(armaMat& Kc_inty) {
  if (controlType & CONTROL_TYPE_INTY) reassign(this->Kc_inty, Kc_inty);
}

void glds::ctrl_t::setTauAntiWindup(data_t& tau) {
  tauAntiWindup = tau;
  kAntiWindup = dt / tauAntiWindup;
}

void glds::ctrl_t::calc_ssSetPt() {
  // Linearly-constrained least squares (ls).
  //
  // Reference:
  //  Boyd & Vandenberghe (2018) Introduction to Applied Linear Algebra
  //
  armaMat A_ls = join_horiz(C, armaMat(nY, nU, fill::zeros));
  armaVec b_ls = yRef - d;
  armaMat C_ls =
      join_horiz(A - armaMat(nX, nX, fill::eye), B * arma::diagmat(g));
  armaVec d_ls = -m0;
  if (controlType & CONTROL_TYPE_ADAPT_M)
    d_ls = -m;  // adapt setpoint calc with disturbance?

  armaMat A_ls_t = A_ls.t();  // TODO(mfbolus): not sure why but causes seg
                              // fault if I do not do this.
  armaMat phi_ls = join_vert(join_horiz(2 * A_ls_t * A_ls, C_ls.t()),
                             join_horiz(C_ls, armaMat(nX, nX, fill::zeros)));
  armaMat inv_phi = pinv(
      phi_ls);  // TODO(mfbolus): should be actual inverse, rather than pseudo-
  armaVec xulam = inv_phi * join_vert(2 * A_ls_t * b_ls, d_ls);
  xRef = xulam.subvec(0, nX - 1);
  uRef = xulam.subvec(nX, nX + nU - 1);
  yRef = C * xRef + d;  // the least-squares solution
}

void glds::ctrl_t::antiWindup() {
  uSaturated = false;
  uSat = u;

  // limit u and flag whether saturated
  for (size_t k = 0; k < u.n_elem; k++) {
    if (u[k] < uLB) {
      uSat[k] = uLB;
      uSaturated = true;
    }

    if (u[k] > uUB) {
      uSat[k] = uUB;
      uSaturated = true;
    }
  }

  if ((controlType & CONTROL_TYPE_INTY) && (tauAntiWindup < lds::inf)) {
    // one-step back-calculation (calculate intE for u=u_sat)
    // (Astroem, Rundqwist 1989 warn against using this...)
    // intE_awuAdjust =
    //     solve(Kc_inty, (u - uSat));  // pinv(Kc_inty) * (u-uSat);

    // gradual: see Astroem, Rundqwist 1989
    // this is a fudge for doing MIMO gradual
    // n.b., went ahead and multiplied 1/T by dt so don't have to do that here.
    intE_awuAdjust = kAntiWindup * (sign(Kc_inty).t() / nU) * (u - uSat);
    // intE_awuAdjust = kAntiWindup * (u-uSat);

    intE += intE_awuAdjust;
  }

  // set u to saturated version
  u = uSat;
}

void glds::ctrl_t::reset() {
  glds::sys_t::reset();
  uRef.zeros();
  uRef_prev.zeros();
  intE.zeros();
  intE_awuAdjust.zeros();
  uSat.zeros();
  uSaturated = false;
  t_since_ctrl_onset = 0.0;
}

void glds::ctrl_t::printSys() {
  glds::sys_t::printSys();
  std::cout << "gDesign : " << gDesign << "\n";
  std::cout << "uLB : " << uLB << "\n";
  std::cout << "uUB : " << uUB << "\n";
}

glds::ctrl_t& glds::ctrl_t::operator=(const glds::ctrl_t& sys) {
  // TODO(mfbolus): would love to be able to re-use the lds code:
  //
  // (*this) = lds::sys_t::operator=(sys);
  //
  // but this does not work bc the input is glds::sys_t which is a subclass of
  // lds::sys_t. Need to figure out if there is a way to write functions to
  // apply to all subclasses (e.g., <lds::sys_t& sys)

  // FROM LDS
  this->A = sys.A;
  this->B = sys.B;
  this->g = sys.g;
  this->Q = sys.Q;
  this->x0 = sys.x0;
  this->P0 = sys.P0;

  this->Q_m = sys.Q_m;
  this->m0 = sys.m0;
  this->P0_m = sys.P0_m;

  this->u = sys.u;
  this->x = sys.x;
  this->P = sys.P;
  this->m = sys.m;
  this->P_m = sys.P_m;

  this->dt = sys.dt;
  this->p0 = sys.p0;
  this->q0 = sys.q0;

  this->nX = sys.nX;
  this->nU = sys.nU;
  this->szChanged = sys.szChanged;
  // END FROM LDS

  // from GLDS
  this->C = sys.C;
  this->d = sys.d;
  this->R = sys.R;
  this->r0 = sys.r0;

  this->Ke = sys.Ke;
  this->Ke_m = sys.Ke_m;
  this->adaptM = sys.adaptM;

  this->y = sys.y;
  this->z = sys.z;
  this->nY = sys.nY;
  // end from GLDS

  // additional ctrl_t stuff
  this->gDesign = sys.gDesign;
  this->uRef = sys.uRef;
  this->uRef_prev = sys.uRef_prev;
  this->xRef = sys.xRef;
  this->yRef = sys.yRef;
  this->Kc_x = sys.Kc_x;
  this->Kc_u = sys.Kc_u;
  this->Kc_inty = sys.Kc_inty;
  this->duRef = sys.duRef;
  this->dvRef = sys.dvRef;
  this->vRef = sys.vRef;
  this->dv = sys.dv;
  this->v = sys.v;
  this->intE = sys.intE;
  this->intE_awuAdjust = sys.intE_awuAdjust;
  this->uLB = sys.uLB;
  this->uUB = sys.uUB;
  this->uSat = sys.uSat;
  this->uSaturated = sys.uSaturated;
  this->t_since_ctrl_onset = sys.t_since_ctrl_onset;
  this->controlType = sys.controlType;
  return *this;
}

void glds::ctrl_t::setDims(size_t& nU, size_t& nX, size_t& nY) {
  // If dimensions are changed, need to resize object and reset.

  if (nU != this->nU) {
    szChanged = true;
    this->nU = nU;
  }

  if (nX != this->nX) {
    szChanged = true;
    this->nX = nX;
  }

  if (nY != this->nY) {
    szChanged = true;
    this->nY = nY;
  }

  // this seems a bit heavy-handed, but if any of the dimensions are changed,
  // reset everything.
  if (szChanged) {
    cout << "System dimensions were changed. Resetting object.\n";
    (*this) = glds::ctrl_t(nU, nX, nY, uLB, uUB, dt, p0, q0, r0, controlType);
    szChanged = false;
  }
}
