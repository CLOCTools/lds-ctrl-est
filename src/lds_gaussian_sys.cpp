#include <ldsCtrlEst>

using namespace std;
using namespace glds;

// ******************* SYS_T *******************
/* Constructor(s) for sys class... */
glds::sys_t::sys_t(size_t nU, size_t nX, size_t nY, data_t& dt, data_t& p0, data_t& q0, data_t& r0, size_t augmentation) :
lds::sys_t(nU, nX, dt, p0, q0),
r0(r0)
{
	this->nY = nY;

	y = armaVec(nY, fill::zeros);
	z = armaVec(nY, fill::zeros);
	d = armaVec(nY, fill::zeros);

	C = armaMat(nY, nX, fill::eye); //each state will map to an output by default (as many as possible)
	Ke = armaMat(nX, nY, fill::zeros);//estimator gain.
	defaultR();

	// perform any augmentations.
	augment(augmentation);
};

/*
Correct: Given measurement (z) and predicted state, update estimate of the state, covar, output using Kalman filter
*/
void glds::sys_t::update(armaVec& z, bool doRecurse_Ke)
{
	// assign the measurement internal variable.
	setZ(z);

	// Given *predicted* state, calc predicted output.
	h();//predicted y

	if (doRecurse_Ke)
	recurse_Ke();

	// update
	x = x - Ke * (y-this->z); //posterior

	// With new update, estimate output.
	h();//posterior
}

void glds::sys_t::recurse_Ke() //recursively estimate Ke
{
	// predict covariance
	P = A*P*A.t() + Q;

	// calc Kalman gain
	Ke = P * C.t() * inv_sympd(C*P*C.t() + R);

	// update covariance
	P = P - Ke*C*P;//Hinton, Gaghramani 1995
}

/*
Output: y_{k} = h(x_{k})
*/
void glds::sys_t::h()
{
	y = C*x + d;
}

/*
Measurement: z ~ N(y,R)
*/
void glds::sys_t::simMeasurement(armaVec& z)
{
	z = y + sqrtmat_sympd(R)*armaVec(y.n_elem,fill::randn);
}

void glds::sys_t::reset()
{
	lds::sys_t::reset();
	h();
}

// Fall back to default value for R
void glds::sys_t::defaultR() {
	R.zeros(nY,nY);
	R.diag().fill(r0);
}

void glds::sys_t::setDims(size_t& nU, size_t& nX, size_t& nY) {
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

	if (nY != this->nY)
	{
		szChanged=true;
		this->nY = nY;
	}

	// this seems a bit heavy-handed, but if any of the dimensions are changed, reset everything.
	if (szChanged) {
		cout << "System dimensions were changed. Resetting object.\n";
		(*this)=glds::sys_t(nU,nX,nY,dt,p0,q0,r0,augmentation);
		szChanged=false;
	}
}

void glds::sys_t::setC(stdVec& cVec) {
	if (augmentation && (cVec.size() != C.n_elem)) {
		auto temp = C.submat(0,0,nY-1,nX-1);
		reassign(temp, cVec);
	} else {
		reassign(C, cVec);
	}
}
void glds::sys_t::setC(armaMat& C) {
	if (augmentation && (C.n_elem != this->C.n_elem)) {
		auto temp = this->C.submat(0,0,nY-1,nX-1);
		reassign(temp, C);
	} else {
		reassign(this->C, C);
	}
}

void glds::sys_t::setR(stdVec& rVec) {
	reassign(R, rVec);
}
void glds::sys_t::setR(armaMat& R) {
	reassign(this->R, R);
}

void glds::sys_t::setD(stdVec& dVec) {
	reassign(d, dVec);
}
void glds::sys_t::setD(armaVec& d) {
	reassign(this->d, d);
}

void glds::sys_t::setZ(stdVec& zVec) {
	reassign(z, zVec);
}
void glds::sys_t::setZ(armaVec& z) {
	reassign(this->z, z);
}
void glds::sys_t::setKe(stdVec& keVec) {
	reassign(Ke, keVec);
}
void glds::sys_t::setKe(armaMat& Ke) {
	reassign(this->Ke, Ke);
}

void glds::sys_t::augment(size_t augmentation) {
	// call the parent's method
	lds::sys_t::augment(augmentation);

	// now just to the C augmentations
	if (augmentation & AUGMENT_M) {
		C = join_horiz(getC(), armaMat(nY,nX,fill::zeros));
		Ke = join_horiz(Ke, armaMat(nY,nX,fill::zeros));
	}

	if (augmentation & AUGMENT_G) {
		C = join_horiz( C, armaMat(nY,nU,fill::zeros) );
		Ke = join_horiz(Ke, armaMat(nY,nU,fill::zeros));
	}
}

void glds::sys_t::printSys() {
	lds::sys_t::printSys();
	cout << "d: \n" << d << endl;
	cout << "C: \n" << C << endl;
	cout << "R: \n" << R << endl;
	cout << "y: \n" << y << endl;
}

glds::sys_t& glds::sys_t::operator=(const glds::sys_t& sys)
{
	// FROM LDS
	this->A = sys.A;
	this->B = sys.B;
	this->Q = sys.Q;

	this->x0 = sys.x0;
	this->P0 = sys.P0;
	this->m = sys.m;
	this->g = sys.g;

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
	// END FROM LDS

	this->C = sys.C;
	this->d = sys.d;
	this->R = sys.R;
	this->r0 = sys.r0;

	this->Ke = sys.Ke;

	this->y = sys.y;
	this->z = sys.z;
	this->nY = sys.nY;

	return *this;
}
// ******************* SYS_T *******************
