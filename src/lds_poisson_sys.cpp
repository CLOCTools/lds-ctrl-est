#include <ldsCtrlEst>

using namespace std;
using namespace plds;

// ******************* SYS_T *******************
/* Constructor(s) for sys class... */
plds::sys_t::sys_t(size_t nU, size_t nX, size_t nY, data_t& dt, data_t& p0,
                   data_t& q0)
    : lds::sys_t(nU, nX, dt, p0, q0) {
  this->nY = nY;

  y = armaVec(nY, fill::zeros);
  logy = armaVec(nY, fill::zeros);
  z = armaVec(nY, fill::zeros);
  d = armaVec(nY, fill::zeros);

  C = armaMat(nY, nX, fill::eye);  // each state will map to an output by
                                   // default (as many as possible)
  diag_y = diagmat(y);

  // TODO: for some reason, on my mac, initializing with randu results in bus
  // error! chance = armaVec(nY, fill::randu);
  chance = armaVec(nY, fill::zeros);
};

/*
predict: Given input, predict the state, covar
*/
void plds::sys_t::predict() {
  lds::sys_t::predict();
  h();
}

/*
Correct: Given measurement (z) and current input (u), update estimate of the
state, covar, output. Eden et al. 2004
*/
void plds::sys_t::filter(armaVec& z) {
  predict();
  diag_y.diag() = y;

  // assign the measurement internal variable.
  setZ(z);

  // predict covariance (took this out of generic predict step...)
  P = A * P * A.t() + Q;

  // update cov
  P = pinv(pinv(P) + C.t() * diag_y * C);
  x = x + P * C.t() * (z - y);  // posterior
  if (adaptM) {
    P_m += Q_m;  // A_m = I
    P_m = pinv(pinv(P_m) + C.t() * diag_y * C);
    m = m + P_m * C.t() * (z - y);
  }
  h();  // posterior
}

/*
Output: y_{k} = h(x_{k})
*/
void plds::sys_t::h() {
  logy = C * x + d;
  y = exp(logy);
}

/*
Measurement: z ~ Poiss(y)
n.b., In reality, this is Poisson where lim_{y->0}.
If either of those is violated, results will be innacurate.
*/
void plds::sys_t::simMeasurement(armaVec& z) {
  h();

  // roll the dice.
  chance.randu(y.n_elem);

  // Compare.
  z.zeros();
  for (std::size_t k = 0; k < y.n_elem; k++) {
    if ((y[k]) > chance[k]) z[k] = 1.0;
  }
}

void plds::sys_t::reset() {
  lds::sys_t::reset();
  h();
  diag_y = diagmat(y);
}

void plds::sys_t::setDims(size_t& nU, size_t& nX, size_t& nY) {
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
    (*this) = plds::sys_t(nU, nX, nY, dt, p0, q0);
    szChanged = false;
  }
}

void plds::sys_t::setC(stdVec& cVec) { reassign(C, cVec); }
void plds::sys_t::setC(armaMat& C) { reassign(this->C, C); }

void plds::sys_t::setD(stdVec& dVec) { reassign(d, dVec); }
void plds::sys_t::setD(armaVec& d) { reassign(this->d, d); }

void plds::sys_t::setZ(stdVec& zVec) { reassign(z, zVec); }
void plds::sys_t::setZ(armaVec& z) { reassign(this->z, z); }

void plds::sys_t::printSys() {
  lds::sys_t::printSys();
  cout << "d: \n" << d << endl;
  cout << "C: \n" << C << endl;
  cout << "y: \n" << y << endl;
}

plds::sys_t& plds::sys_t::operator=(const plds::sys_t& sys) {
  // // would love to be able to re-use the lds code:
  // (*this) = lds::sys_t::operator=(sys);
  // // but this does not work bc the input is plds::sys_t which is a subclass
  // of lds::sys_t
  // // Need to figure out if there is a way to write functions to apply to all
  // subclasses (e.g., <lds::sys_t& sys)

  // Just going to copy code for now...
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

  // PLDS
  this->C = sys.C;
  this->d = sys.d;
  this->logy = sys.logy;
  this->y = sys.y;
  this->z = sys.z;
  this->nY = sys.nY;
  this->diag_y = sys.diag_y;
  this->chance = sys.chance;
  // END FROM PLDS

  return *this;
}
// ******************* SYS_T *******************
