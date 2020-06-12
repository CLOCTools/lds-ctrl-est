#include <ldsCtrlEst>

using namespace std;
using namespace glds;

lds::gaussian::fit_t::fit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q, armaVec& x0, armaMat& P0, armaMat& C, armaMat& D, armaVec& d, armaMat& R, data_t dt, vector<data_t>& t0, vector<armaMat>& uTrain, vector<armaMat>& zTrain) : lds::fit_t(A,B,g,m,Q,x0,P0,dt)
{
	//output
	this->C = C;
	this->D = D;
	this->d = d;
	this->R = R;

	this->t0 = t0;
	this->uTrain = uTrain;
	this->zTrain = zTrain;
}

lds::gaussian::fit_t::fit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q, armaVec& x0, armaMat& P0, armaMat& C, armaVec& d, armaMat& R, data_t dt, vector<data_t>& t0, vector<armaMat>& uTrain, vector<armaMat>& zTrain) : lds::fit_t(A,B,g,m,Q,x0,P0,dt)
{
	//output
	this->C = C;
	this->D = armaMat(C.n_rows, B.n_cols, fill::zeros);
	this->d = d;
	this->R = R;

	this->t0 = t0;
	this->uTrain = uTrain;
	this->zTrain = zTrain;
}
