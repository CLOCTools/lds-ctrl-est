#include <ldsCtrlEst>

using namespace std;
using namespace lds;

//******************* SYS_T *******************
// Constructor(s) for sys_t class
lds::sys_t::sys_t(size_t nU, size_t nX, data_t& dt, data_t& p0, data_t& q0): dt(dt), p0(p0), q0(q0)
{
	this->nU = nU;
	this->nX = nX;
	nXaug = nX;//until augmented...

	// Initially make this not augmented
	augmentation = 0;
	szChanged = false;

	u = armaVec(nU, fill::zeros);
	diag_u = armaMat(nU, nU, fill::zeros);

	// initial conditions.
	x0 = armaVec(nX, fill::zeros); // includes bias (nY) and g (nU)
	x = x0;
	P0 = p0*armaMat(nX, nX, fill::zeros);//fill::eye);
	P = P0;

	g = armaVec(nU, fill::ones);
	m = armaVec(nX, fill::zeros);

	// By default, random walk where each state is independent
	// In this way, provides independent estimates of rate per channel of output.
	A = armaMat(nX, nX, fill::eye);
	B = armaMat(nX, nU, fill::zeros);
	Q = q0 * armaMat(nX, nX, fill::eye);
};

/*
predict: Given input, predict the state, covar
*/
void lds::sys_t::predict()
{
	// Dynamics: x_{k+1} = f(x_{k},u_{k},w_{k})
	if (augmentation & AUGMENT_G) {
		// Assumes A is augmented to included Bu term that multiplies with g.
		diag_u.diag() = u;
		A.submat(0, A.n_cols-nU, nX-1, A.n_cols-1) = B.submat(0,0,nX-1,nU-1)*diag_u;
		x = A*x;
	} else {
		// TODO: make diag_u diag_gu because this does not need
		// to be matrix mult
		x = A*x + B*(g%u);
	}

	if (!(augmentation & AUGMENT_M))
	x.subvec(0,nX-1) += m;
	// P = A*P*A.t() + Q;//going to do this only if calculating Kest.
}

/*
predict: Given input, predict the state, covar
*/
void lds::sys_t::simPredict()
{
	// Dynamics: x_{k+1} = f(x_{k},u_{k},w_{k})
	if (augmentation & AUGMENT_G) {
		// Assumes A is augmented to included Bu term that multiplies with g.
		diag_u.diag() = u;
		A.submat(0, A.n_cols-nU, nX-1, A.n_cols-1) = B.submat(0,0,nX-1,nU-1)*diag_u;
		x = A*x;
	} else {
		x = A*x + B*(g%u);
	}

	if (!(augmentation & AUGMENT_M))
	x.subvec(0,nX-1) += m;

	if (augmentation)
	x += sqrtmat_sympd(Q) * armaVec(nXaug, fill::randn);
	else
	x += sqrtmat_sympd(Q) * armaVec(nX, fill::randn);
}

// report if there is any form of augmentation
bool lds::sys_t::checkIfAugmented()
{
	bool isAugmented(augmentation);
	return isAugmented;
}

// check if *any* of the augmentations match the type
bool lds::sys_t::checkIfAugmented(size_t augmentationType)
{
	bool isAugmented((augmentation & augmentationType));
	return isAugmented;
}

void lds::sys_t::augment(size_t augmentation) {
	if (this->augmentation == augmentation)
	return;

	// this is a fudge. Would like to have conditions that selectively take it off.
	deaugment();

	nXaug = nX;
	if (augmentation & AUGMENT_M) {
		x = join_vert(x, m);
		x0 = join_vert(x0, m);
		m.zeros();

		Q = join_horiz( Q, armaMat(nX,nX,fill::zeros) );
		Q = join_vert( Q, join_horiz(armaMat(nX,nX,fill::zeros), q0*armaMat(nX, nX, fill::eye)) );

		P0 = join_horiz( P0, armaMat(nX,nX,fill::zeros) );
		P0 = join_vert( P0, join_horiz(armaMat(nX,nX,fill::zeros), p0*armaMat(nX, nX, fill::eye)) );
		P = P0;

		A = join_horiz( A, armaMat(nX,nX,fill::eye) );
		A = join_vert( A, join_horiz(armaMat(nX,nX,fill::zeros), armaMat(nX, nX, fill::eye)) );

		B = join_vert( B, armaMat(nX,nU,fill::zeros) );

		this->augmentation = this->augmentation | AUGMENT_M;
		nXaug += nX;
	}

	if (augmentation & AUGMENT_G) {
		x = join_vert(x, g);
		x0 = join_vert(x0, g);

		Q = join_horiz( Q, armaMat(nXaug,nU,fill::zeros) );
		Q = join_vert( Q, join_horiz(armaMat(nU,nXaug,fill::zeros), q0*armaMat(nU, nU, fill::eye)) );

		P0 = join_horiz( P0, armaMat(nXaug,nU,fill::zeros) );
		P0 = join_vert( P0, join_horiz(armaMat(nU,nXaug,fill::zeros), p0*armaMat(nU, nU, fill::eye)) );
		P = P0;

		A = join_horiz( A, armaMat(nXaug,nU,fill::zeros) );
		A = join_vert( A, join_horiz(armaMat(nU,nXaug,fill::zeros), armaMat(nU, nU, fill::eye)) );

		B = join_vert( B, armaMat(nU,nU,fill::zeros) );

		this->augmentation = this->augmentation | AUGMENT_G;
		nXaug += nU;
	}
}

void lds::sys_t::deaugment()
{
	if (augmentation==0)
	{
		return;
	}

	if (augmentation & AUGMENT_M)
	m = x.subvec(nX,nX+nX-1);

	if (augmentation & AUGMENT_G)
	g = x.subvec(nXaug-nU,nXaug-1);

	x = x.subvec(0, nX-1);
	x0 = x0.subvec(0, nX-1);

	Q = Q.submat(0,0,nX-1,nX-1);

	P0 = P0.submat(0,0,nX-1,nX-1);
	P = P0;

	A = A.submat(0,0,nX-1,nX-1);
	B = B.submat(0,0,nX-1,nU-1);

	augmentation = 0;
	nXaug = nX;
}

void lds::sys_t::reset()
{
	x = x0; //mean
	P = P0; //cov
	g = getG();//might be unnecessary
	m = getM();//might be unnecessary

	diag_u.zeros();
	szChanged=false;//turn it back to false to reset...
}

armaVec lds::sys_t::getG() const {
	if (lds::AUGMENT_G & augmentation)
	return x.subvec(nXaug-nU, nXaug-1);
	else
	return g;
}

armaVec lds::sys_t::getM() const {
	if (augmentation & AUGMENT_M)
	return x.subvec(nX, nX+nX-1);
	else
	return m;
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

	// for good measure...
	P = P0;
}

// Setting parameter values...
void lds::sys_t::setA(stdVec& aVec) {
	if (augmentation && (aVec.size() != A.n_elem)) {
		auto temp = A.submat(0,0,nX-1,nX-1);
		reassign(temp, aVec);
	} else {
		reassign(A, aVec);
	}
}
void lds::sys_t::setA(armaMat& A) {
	if (augmentation && (A.n_elem != this->A.n_elem)) {
		auto temp = this->A.submat(0,0,nX-1,nX-1);
		reassign(temp, A);
	} else {
		reassign(this->A, A);
	}
}

void lds::sys_t::setB(stdVec& bVec) {
	if (bVec.size() != B.n_elem) {
		auto temp = B.submat(0,0,nX-1,nU-1);
		reassign(temp, bVec);
	} else {
		reassign(B, bVec);
	}
}

void lds::sys_t::setB(armaMat& B) {
	if (B.n_elem != this->B.n_elem) {
		auto temp = this->B.submat(0,0,nX-1,nU-1);
		reassign(temp,B);
	} else {
		reassign(this->B,B);
	}
}

void lds::sys_t::setQ(stdVec& qVec) {
	if (augmentation && (qVec.size() != Q.n_elem)) {
		auto temp = Q.submat(0,0,nX-1,nX-1);
		reassign(temp, qVec);
	} else {
		reassign(Q,qVec);
	}
}
void lds::sys_t::setQ(armaMat& Q) {
	if (augmentation && (Q.n_elem != this->Q.n_elem)) {
		auto temp = this->Q.submat(0,0,nX-1,nX-1);
		reassign(temp, Q);
	} else {
		reassign(this->Q, Q);
	}
}

void lds::sys_t::setP0(stdVec& p0Vec) {
	if (augmentation && (p0Vec.size() != P0.n_elem)) {
		auto temp = P0.submat(0,0,nX-1,nX-1);
		reassign(temp, p0Vec);
	} else {
		reassign(P0,p0Vec);
	}
}
void lds::sys_t::setP0(armaMat& P0) {
	if (augmentation && (P0.n_elem != this->P0.n_elem)) {
		auto temp = this->P0.submat(0,0,nX-1,nX-1);
		reassign(temp, P0);
	} else {
		reassign(this->P0, P0);
	}
}

void lds::sys_t::setX0(stdVec& x0Vec) {
	if (augmentation && (x0Vec.size() != x0.n_elem)) {
		auto temp = x0.subvec(0,nX-1);
		reassign(temp, x0Vec);
	} else {
		reassign(x0, x0Vec);
	}
}
void lds::sys_t::setX0(armaVec& x0) {
	if (augmentation && (x0.n_elem != this->x0.n_elem)) {
		auto temp = this->x0.subvec(0,nX-1);
		reassign(temp, x0);
	} else {
		reassign(this->x0, x0);
	}
}

void lds::sys_t::setG(stdVec& gVec) {
	if (augmentation & AUGMENT_G) {
		auto temp = x0.subvec(nXaug-nU,nXaug-1);
		reassign(temp,gVec);
	} else {
		reassign(g,gVec);
	}
}
void lds::sys_t::setG(armaVec& g) {
	if (augmentation & AUGMENT_G) {
		auto temp = x0.subvec(nXaug-nU,nXaug-1);
		reassign(temp, g);
	} else {
		reassign(this->g, g);
	}
}

void lds::sys_t::setM(stdVec& mVec) {
	if (augmentation & AUGMENT_M) {
		auto temp = x0.subvec(nX,nX+nX-1);
		reassign(temp, mVec);
	} else {
		reassign(m, mVec);
	}
}
void lds::sys_t::setM(armaVec& m) {
	if (augmentation & AUGMENT_M) {
		auto temp = x0.subvec(nX,nX+nX-1);
		reassign(temp, m);
	} else {
		reassign(this->m, m);
	}
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
	cout << "p0: " << p0 << endl;
	cout << "q0: " << q0 << endl;
	if (!(augmentation & AUGMENT_M)) {
		cout << "m: \n" << m << endl;
	}
	if (!(augmentation & AUGMENT_G)) {
		cout << "g: \n" << g << endl;
	}
}

lds::sys_t& lds::sys_t::operator=(const lds::sys_t& sys)
{
	this->A = sys.A;
	this->B = sys.B;
	this->Q = sys.Q;

	this->x0 = sys.x0;
	this->P0 = sys.P0;
	this->m = sys.m;
	this->g = sys.g;

	this->u = sys.u;
	this->x = sys.x;
	this->P = sys.P;

	this->dt = sys.dt;
	this->p0 = sys.p0;
	this->q0 = sys.q0;

	this->nX = sys.nX;
	this->nU = sys.nU;
	this->nXaug = sys.nXaug;

	this->augmentation = sys.augmentation;
	this->diag_u = sys.diag_u;
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
}
//******************* SYS_T *******************
