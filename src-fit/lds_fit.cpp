#include <ldsCtrlEst>

using namespace std;
using namespace lds;

lds::fit_t::fit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q,
                  armaVec& x0, armaMat& P0, data_t& dt) {
  this->A = A;
  this->B = B;
  this->g = g;
  this->m = m;
  this->Q = Q;

  this->x0 = x0;

  if (P0.is_sympd()) {
    this->P0 = P0;
  } else {
    cerr << "Warning: P0 was not symmetric/positive-def. Going to set P0 to "
            "diag mat with default value...\n";
    this->P0 = DEFAULT_P0 * armaMat(A.n_rows, A.n_cols, fill::eye);
  }

  this->dt = dt;
}
