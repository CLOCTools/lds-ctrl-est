//===-- lds_gaussian_sys.cpp - GLDS ---------------------------------------===//
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
/// This file implements the type for state estimation (filtering) as well as
/// simulation of Gaussian-output linear dynamical systems
/// (`lds::gaussian::sys_t`). It inherits functionality from the underlying
/// linear dynamical system (`lds::sys_t`).
///
/// \brief GLDS base type
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>

using namespace std;
using namespace glds;

glds::sys_t::sys_t(size_t nU, size_t nX, size_t nY, data_t& dt, data_t& p0,
                   data_t& q0, data_t& r0)
    : lds::sys_t(nU, nX, dt, p0, q0), r0(r0) {
  this->nY = nY;

  y = armaVec(nY, fill::zeros);
  z = armaVec(nY, fill::zeros);
  d = armaVec(nY, fill::zeros);

  C = armaMat(nY, nX, fill::eye);       // each state will map to an output by
                                        // default (as many as possible)
  Ke = armaMat(nX, nY, fill::zeros);    // estimator gain.
  Ke_m = armaMat(nX, nY, fill::zeros);  // estimator gain for m adaptation.
  defaultR();
};

void glds::sys_t::predict() {
  lds::sys_t::predict();
  h();
}

// Filter: Given measurement (`z`) and previous input (`u_tm1`), predict state
// and update estimate of the state, covar, output using Kalman filter
void glds::sys_t::filter(armaVec& u_tm1, armaVec& z_t, bool doRecurse_Ke) {
  setU(u_tm1);  // set u
  filter(z_t, doRecurse_Ke);
}

// Filter: Given measurement (`z`) and previous input (`u_tm1`), predict state
// and update estimate of the state, covar, output using Kalman filter
// (assumes u was previously set or was set internall, e.g. during control)
void glds::sys_t::filter(armaVec& z, bool doRecurse_Ke) {
  // predict mean
  predict();

  // assign the measurement internal variable.
  setZ(z);

  // recursively calculate esimator gains (or just keep existing values)
  if (doRecurse_Ke) recurse_Ke();

  // update
  x += Ke * (this->z - y);
  if (adaptM) m += Ke_m * (this->z - y);  // adaptively estimating disturbance

  // With new state, estimate output.
  h();  // --> posterior
}

// recursively estimate Ke
void glds::sys_t::recurse_Ke() {
  // predict covariance
  P = A * P * A.t() + Q;

  // calc Kalman gain
  Ke = P * C.t() * inv_sympd(C * P * C.t() + R);

  // update covariance
  // Reference: Ghahramani et Hinton (1996)
  P = P - Ke * C * P;

  if (adaptM) {
    P_m += Q_m;  // A_m = I (i.e., random walk)
    Ke_m = P_m * C.t() * inv_sympd(C * P_m * C.t() + R);
    P_m = P_m - Ke_m * C * P_m;
  }
}

// Output: y_{k} = h(x_{k})
void glds::sys_t::h() { y = C * x + d; }

// Measurement: z ~ N(y,R)
void glds::sys_t::simMeasurement(armaVec& z) {
  h();
  z = y + arma::mvnrnd(armaVec(y.n_elem).fill(0), R);
}

void glds::sys_t::reset() {
  lds::sys_t::reset();
  h();
}

// Fall back to default value for R
void glds::sys_t::defaultR() {
  R.zeros(nY, nY);
  R.diag().fill(r0);
}

void glds::sys_t::setDims(size_t& nU, size_t& nX, size_t& nY) {
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
    (*this) = glds::sys_t(nU, nX, nY, dt, p0, q0, r0);
    szChanged = false;
  }
}

// output-specific set methods
void glds::sys_t::setC(stdVec& cVec) { reassign(C, cVec); }
void glds::sys_t::setC(armaMat& C) { reassign(this->C, C); }

void glds::sys_t::setR(stdVec& rVec) { reassign(R, rVec); }
void glds::sys_t::setR(armaMat& R) { reassign(this->R, R); }

void glds::sys_t::setD(stdVec& dVec) { reassign(d, dVec); }
void glds::sys_t::setD(armaVec& d) { reassign(this->d, d); }

void glds::sys_t::setZ(stdVec& zVec) { reassign(z, zVec); }
void glds::sys_t::setZ(armaVec& z) { reassign(this->z, z); }

void glds::sys_t::setKe(stdVec& keVec) { reassign(Ke, keVec); }
void glds::sys_t::setKe(armaMat& Ke) { reassign(this->Ke, Ke); }

void glds::sys_t::setKe_m(stdVec& kemVec) { reassign(Ke_m, kemVec); }
void glds::sys_t::setKe_m(armaMat& Ke_m) { reassign(this->Ke_m, Ke_m); }

void glds::sys_t::printSys() {
  lds::sys_t::printSys();
  cout << "d: \n" << d << endl;
  cout << "C: \n" << C << endl;
  cout << "R: \n" << R << endl;
  cout << "y: \n" << y << endl;
}

glds::sys_t& glds::sys_t::operator=(const glds::sys_t& sys) {
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

  return *this;
}
