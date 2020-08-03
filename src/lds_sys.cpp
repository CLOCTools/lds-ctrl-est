#include <ldsCtrlEst>

using namespace std;
using namespace lds;

//******************* SYS_T *******************
// Constructor(s) for sys_t class
lds::sys_t::sys_t(size_t nU, size_t nX, data_t& dt, data_t& p0, data_t& q0): dt(dt), p0(p0), q0(q0)
{
	this->nU = nU;
	this->nX = nX;

	u = armaVec(nU, fill::zeros);

	// initial conditions.
	x0 = armaVec(nX, fill::zeros); // includes bias (nY) and g (nU)
	x = x0;
	P0 = armaMat(nX, nX, fill::zeros);
	// P0 = p0*armaMat(nX, nX, fill::eye);
	P = P0;

	m0 = x0;
	m = m0;
	P0_m = P0;
	P_m = P0_m;

	g = armaVec(nU, fill::ones);

	// By default, random walk where each state is independent
	// In this way, provides independent estimates of rate per channel of output.
	A = armaMat(nX, nX, fill::eye);
	B = armaMat(nX, nU, fill::zeros);
	Q = q0 * armaMat(nX, nX, fill::eye);
	Q_m = Q;

	adaptM = false;
	szChanged = false;
};

/*
predict: Given input, predict the state, covar
*/
void lds::sys_t::predict()
{
	// Dynamics: x_{k+1} = f(x_{k},u_{k},w_{k})
	x = A*x + B*(g%u) + m;
	// P = A*P*A.t() + Q;//going to do this only if calculating estimator gain, Ke
}

/*
predict: Given input, predict the state, covar
*/
void lds::sys_t::simPredict()
{
	// Dynamics: x_{k+1} = f(x_{k},u_{k},w_{k})
	x = A*x + B*(g%u) + m;
	x += sqrtmat_sympd(Q) * armaVec(nX, fill::randn);
}

void lds::sys_t::reset()
{
	// reset to initial conditions
	x = x0; //mean
	P = P0; //cov
	m = m0;
	P_m = P0_m;
	szChanged = false;
}

void lds::sys_t::setDims(size_t& nU, size_t& nX) {
	if (nU != this->nU)
	{
		szChanged=true;
		this->nU = nU;
	}

	if (nX != this->nX)
	{
		szChanged=true;
		this->nX = nX;
	}

	// this seems a bit heavy-handed, but if any of the dimensions are changed, reset everything.
	if (szChanged) {
		cout << "System dimensions were changed. Resetting object.\n";
		(*this)=lds::sys_t(nU,nX,dt,p0,q0);
		szChanged=false;
	}
}

// setting input
void lds::sys_t::setU(stdVec& uVec) {
	reassign(u, uVec);
}
void lds::sys_t::setU(armaVec& u) {
	reassign(this->u, u);
}

// Fall back to default value for Q and reset P
void lds::sys_t::defaultQ() {
	Q.zeros();
	Q.diag().fill(q0);

	Q_m.zeros();
	Q_m.diag().fill(q0);

	// for good measure...
	P = P0;
	P_m = P0;
}

// Setting parameter values...
void lds::sys_t::setA(stdVec& aVec) {
	reassign(A, aVec);
}
void lds::sys_t::setA(armaMat& A) {
	reassign(this->A, A);
}

void lds::sys_t::setB(stdVec& bVec) {
	reassign(B, bVec);
}

void lds::sys_t::setB(armaMat& B) {
	reassign(this->B,B);
}

void lds::sys_t::setQ(stdVec& qVec) {
	reassign(Q,qVec);
}

void lds::sys_t::setQ(armaMat& Q) {
	reassign(this->Q, Q);
}

void lds::sys_t::setP0(stdVec& p0Vec) {
		reassign(P0,p0Vec);
}
void lds::sys_t::setP0(armaMat& P0) {
		reassign(this->P0, P0);
		P = this->P0;
}

void lds::sys_t::setX0(stdVec& x0Vec) {
	reassign(x0, x0Vec);
}
void lds::sys_t::setX0(armaVec& x0) {
	reassign(this->x0, x0);
}

void lds::sys_t::setG(stdVec& gVec) {
	reassign(g,gVec);
}
void lds::sys_t::setG(armaVec& g) {
	reassign(this->g, g);
}

void lds::sys_t::setM(stdVec& mVec) {
	reassign(m0, mVec);
	if (!adaptM)
	m = m0;
}
void lds::sys_t::setM(armaVec& m) {
	reassign(m0, m);
	if (!adaptM)
	this->m = m0;
}

void lds::sys_t::setP0_m(stdVec& p0mVec) {
		reassign(P0_m,p0mVec);
}
void lds::sys_t::setP0_m(armaMat& P0_m) {
		reassign(this->P0_m, P0_m);
}

void lds::sys_t::setQ_m(stdVec& qmVec) {
	reassign(Q_m,qmVec);
}
void lds::sys_t::setQ_m(armaMat& Q_m) {
	reassign(this->Q_m, Q_m);
}

// Generic functions for re-assigning elements.
void lds::sys_t::reassign(armaVec& oldVar, armaVec& newVar, data_t defaultVal) {
	for (size_t k=0; k<oldVar.n_elem; k++) {
		if (newVar.n_elem>k)
		oldVar[k] = newVar[k];
		else
		oldVar[k] = defaultVal;
	}
}
void lds::sys_t::reassign(armaVec& oldVar, stdVec& newVar, data_t defaultVal) {
	for (size_t k=0; k<oldVar.n_elem; k++) {
		if (newVar.size()>k)
		oldVar[k] = newVar[k];
		else
		oldVar[k] = defaultVal;
	}
}
void lds::sys_t::reassign(armaMat& oldVar, armaMat& newVar, data_t defaultVal) {
	for (size_t k=0; k<oldVar.n_elem; k++) {
		if (newVar.n_elem>k)
		oldVar[k] = newVar[k];
		else
		oldVar[k] = defaultVal;
	}
}
void lds::sys_t::reassign(armaMat& oldVar, stdVec& newVar, data_t defaultVal) {
	for (size_t k=0; k<oldVar.n_elem; k++) {
		if (newVar.size()>k)
		oldVar[k] = newVar[k];
		else
		oldVar[k] = defaultVal;
	}
}

// subviews..
void lds::sys_t::reassign(armaSubVec& oldVar, armaVec& newVar, data_t defaultVal) {
	for (size_t k=0; k<oldVar.n_elem; k++) {
		if (newVar.n_elem>k)
		oldVar[k] = newVar[k];
		else
		oldVar[k] = defaultVal;
	}
}
void lds::sys_t::reassign(armaSubVec& oldVar, stdVec& newVar, data_t defaultVal) {
	for (size_t k=0; k<oldVar.n_elem; k++) {
		if (newVar.size()>k)
		oldVar[k] = newVar[k];
		else
		oldVar[k] = defaultVal;
	}
}
void lds::sys_t::reassign(armaSubMat& oldVar, armaMat& newVar, data_t defaultVal) {
	for (size_t k=0; k<oldVar.n_elem; k++) {
		if (newVar.n_elem>k)
		oldVar[k] = newVar[k];
		else
		oldVar[k] = defaultVal;
	}
}
void lds::sys_t::reassign(armaSubMat& oldVar, stdVec& newVar, data_t defaultVal) {
	for (size_t k=0; k<oldVar.n_elem; k++) {
		if (newVar.size()>k)
		oldVar[k] = newVar[k];
		else
		oldVar[k] = defaultVal;
	}
}

void lds::sys_t::printSys() {
	cout << "\n ********** SYSTEM ********** \n";
	cout << "x: \n" << x << endl;
	cout << "P: \n" << P << endl;
	cout << "A: \n" << A << endl;
	cout << "B: \n" << B << endl;
	cout << "Q: \n" << Q << endl;
	cout << "m: \n" << m << endl;
	cout << "g: \n" << g << endl;
}

lds::sys_t& lds::sys_t::operator=(const lds::sys_t& sys)
{
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

	return *this;
}

void lds::sys_t::limit(stdVec& x, data_t& lb, data_t& ub) {
	for (size_t k=0; k<x.size(); k++)
	{
		x[k] = x[k] < lb ? lb : x[k];
		x[k] = x[k] > ub ? ub : x[k];
	}
}

void lds::sys_t::limit(armaVec& x, data_t& lb, data_t& ub) {
	for (size_t k=0; k<x.n_elem; k++)
	{
		x[k] = x[k] < lb ? lb : x[k];
		x[k] = x[k] > ub ? ub : x[k];
	}
}

bool lds::sys_t::limitReset(armaVec& x, data_t& lb, data_t& ub) {
	bool didReset = false;

	for (size_t k=0; k<x.n_elem; k++)
	{
		if ((x[k]>ub) || (x[k]<lb)) {
			x[k] = lb;
			didReset=true;
		}
	}

	return didReset;
}

void lds::sys_t::limit(armaMat& x, data_t& lb, data_t& ub) {
	for (size_t k=0; k<x.n_elem; k++)
	{
		x[k] = x[k] < lb ? lb : x[k];
		x[k] = x[k] > ub ? ub : x[k];
	}
}

void lds::sys_t::checkP() {
	for (size_t k=0; k<P.n_elem; k++)
	{
		if (abs(P[k]) > plim) {
			cerr << "\n\n P GOT HIGHER THAN PLIM! RESETTING TO P0... \n\n";
			P = P0;
			return;
		}
	}

	for (size_t k=0; k<P_m.n_elem; k++)
	{
		if (abs(P_m[k]) > plim) {
			cerr << "\n\n P_m GOT HIGHER THAN PLIM! RESETTING TO P0... \n\n";
			P_m = P0_m;
			return;
		}
	}
}
//******************* SYS_T *******************
