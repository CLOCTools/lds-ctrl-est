#include <ldsCtrlEst>

using namespace std;
using namespace plds;

lds::poisson::fit_t::fit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m,
                           armaMat& Q, armaVec& x0, armaMat& P0, armaMat& C,
                           armaVec& d, data_t dt, vector<armaMat>& uTrain,
                           vector<armaMat>& zTrain)
    : lds::fit_t(A, B, g, m, Q, x0, P0, dt) {
  this->C = C;
  this->d = d;

  this->uTrain = uTrain;
  this->zTrain = zTrain;
}

lds::poisson::ssidFit_t::ssidFit_t(armaMat& A, armaMat& B, armaVec& g,
                                   armaVec& m, armaMat& Q, armaVec& x0,
                                   armaMat& P0, armaMat& C, armaVec& d,
                                   data_t dt, data_t t_startSSID,
                                   data_t t_stopSSID, armaVec& singVals,
                                   vector<data_t>& t0, vector<armaMat>& uTrain,
                                   vector<armaMat>& zTrain)
    : lds::poisson::fit_t(A, B, g, m, Q, x0, P0, C, d, dt, uTrain, zTrain) {
  this->t0 = t0;
  this->t_startSSID = t_startSSID;
  this->t_stopSSID = t_stopSSID;
  this->singVals = singVals;
}
