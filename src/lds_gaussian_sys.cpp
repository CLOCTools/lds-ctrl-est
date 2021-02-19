#include <ldsCtrlEst>

using namespace std;
using namespace glds;

// ******************* SYS_T *******************
/* Constructor(s) for sys class... */
glds::sys_t::sys_t(size_t nU, size_t nX, size_t nY, data_t& dt, data_t& p0,
                   data_t& q0, data_t& r0)
    : lds::sys_t(nU, nX, dt, p0, q0), r0(r0) {
  this->nY = nY;

  y = armaVec(nY, fill::zeros);
  z = armaVec(nY, fill::zeros);
  d = armaVec(nY, fill::zeros);

  C = armaMat(nY, nX, fill::eye);  // each state will map to an output by
                                   // default (as many as possible)
  Ke = armaMat(nX, nY, fill::zeros);    // estimator gain.
  Ke_m = armaMat(nX, nY, fill::zeros);  // estimator gain for m adaptation.
  defaultR();
};

void glds::sys_t::predict() {
  lds::sys_t::predict();
  h();
}

/*
Correct: Given measurement (z) and predicted state, update estimate of the
state, covar, output using Kalman filter
*/
void glds::sys_t::filter(armaVec& u_tm1, armaVec& z_t, bool doRecurse_Ke) {
  setU(u_tm1);
  filter(z_t, doRecurse_Ke);
}

void glds::sys_t::filter(armaVec& z, bool doRecurse_Ke) {
  // predict mean
  predict();

  // assign the measurement internal variable.
  setZ(z);

  if (doRecurse_Ke) recurse_Ke();

  // update
  x += Ke * (this->z - y);
  if (adaptM) m += Ke_m * (this->z - y);

  // With new state, estimate output.
  h();  // posterior
}

// recursively estimate Ke
void glds::sys_t::recurse_Ke() {
  // predict covariance
  P = A * P * A.t() + Q;

  // calc Kalman gain
  Ke = P * C.t() * inv_sympd(C * P * C.t() + R);

  // update covariance
  P = P - Ke * C * P;  // e.g., Ghahramani+Hinton 1996

  if (adaptM) {
    P_m += Q_m;  // A_m = I
    Ke_m = P_m * C.t() * inv_sympd(C * P_m * C.t() + R);
    P_m = P_m - Ke_m * C * P_m;
  }
}

/*
Output: y_{k} = h(x_{k})
*/
void glds::sys_t::h() { y = C * x + d; }

/*
Measurement: z ~ N(y,R)
*/
void glds::sys_t::simMeasurement(armaVec& z) {
  h();
  // z = y + sqrtmat_sympd(R) * armaVec(y.n_elem, fill::randn);
  z = y + arma::mvnrnd( armaVec(y.n_elem).fill(0), R );
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
// ******************* SYS_T *******************
