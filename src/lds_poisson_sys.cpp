#include <ldsCtrlEst>

using namespace std;
using namespace plds;

// ******************* SYS_T *******************
/* Constructor(s) for sys class... */
plds::sys_t::sys_t(size_t nU, size_t nX, size_t nY, data_t& dt, data_t& p0, data_t& q0, size_t augmentation) :
lds::sys_t(nU, nX, dt, p0, q0)
{
	this->nY = nY;

	y = armaVec(nY, fill::zeros);
	logy = armaVec(nY, fill::zeros);
	z = armaVec(nY, fill::zeros);
	d = armaVec(nY, fill::zeros);

	C = armaMat(nY, nX, fill::eye); //each state will map to an output by default (as many as possible)

	diag_y = diagmat(y);

	// TODO: for some reason, on my mac, initializing with randu results in bus error!
	// chance = armaVec(nY, fill::randu);
	chance = armaVec(nY, fill::zeros);

	nlType = EXP;

	// perform any augmentations.
	augment(augmentation);
};

/*
Correct: Given measurement (z) and current input (u), update estimate of the state, covar, output.
Eden et al. 2004
*/
void plds::sys_t::update(armaVec& z)
{
	// assign the measurement internal variable.
	setZ(z);

	// Given *current* input, make a prediction of the output.
	h();
	diag_y.diag() = y;

	// predict covariance (took this out of generic predict step...)
	P = A*P*A.t() + Q;

	// update cov
	P = inv_sympd( inv_sympd(P) + C.t() * diag_y * C ); //posterior
	x = x + P * C.t() * (z - y); //posterior

	// armaMat Ke = P*C.t();
	// cout << "Ke = " << Ke[0] << endl;

	// With new update, estimate output.
	h();//posterior
}

/*
Output: y_{k} = h(x_{k})
*/
void plds::sys_t::h()
{
	logy = C*x + d;
	y = exp( logy );
}

/*
Measurement: z ~ Poiss(y)
n.b., In reality, this is Poisson where lim_{y->0}.
If either of those is violated, results will be innacurate.
*/
void plds::sys_t::simMeasurement(armaVec& z)
{
	// roll the dice.
	chance.randu(y.n_elem);

	// Compare.
	for (std::size_t k=0; k<y.n_elem; k++)
	{
		if ((y[k]) > chance[k])
		z[k] = 1.0;
		else
		z[k] = 0.0;
	}
}

void plds::sys_t::reset()
{
	lds::sys_t::reset();
	h();
	diag_y = diagmat(y);
}

void plds::sys_t::setDims(size_t& nU, size_t& nX, size_t& nY) {
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
		(*this)=plds::sys_t(nU,nX,nY,dt,p0,q0,augmentation);
		szChanged=false;
	}
}

void plds::sys_t::setC(stdVec& cVec) {
	if (augmentation && (cVec.size() != C.n_elem)) {
		auto temp = C.submat(0,0,nY-1,nX-1);
		reassign(temp, cVec);
	} else {
		reassign(C, cVec);
	}
}
void plds::sys_t::setC(armaMat& C) {
	if (augmentation && (C.n_elem != this->C.n_elem)) {
		auto temp = this->C.submat(0,0,nY-1,nX-1);
		reassign(temp, C);
	} else {
		reassign(this->C, C);
	}
}

void plds::sys_t::setD(stdVec& dVec) {
	reassign(d, dVec);
}
void plds::sys_t::setD(armaVec& d) {
	reassign(this->d, d);
}

void plds::sys_t::setZ(stdVec& zVec) {
	reassign(z, zVec);
}
void plds::sys_t::setZ(armaVec& z) {
	reassign(this->z, z);
}

void plds::sys_t::augment(size_t augmentation) {
	// call the parent's method
	lds::sys_t::augment(augmentation);

	// now just to the C augmentations
	if (augmentation & AUGMENT_M) {
		C = join_horiz(getC(), armaMat(nY,nX,fill::zeros));
	}

	if (augmentation & AUGMENT_G) {
		C = join_horiz( C, armaMat(nY,nU,fill::zeros) );
	}
}

void plds::sys_t::printSys() {
	lds::sys_t::printSys();
	cout << "d: \n" << d << endl;
	cout << "C: \n" << C << endl;
	cout << "y: \n" << y << endl;
}

plds::sys_t& plds::sys_t::operator=(const plds::sys_t& sys)
{
	// // would love to be able to re-use the lds code:
	// (*this) = lds::sys_t::operator=(sys);
	// // but this does not work bc the input is plds::sys_t which is a subclass of lds::sys_t
	// // Need to figure out if there is a way to write functions to apply to all subclasses (e.g., <lds::sys_t& sys)

	// Just going to copy code for now...
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

	this->logy = sys.logy;
	this->y = sys.y;
	this->z = sys.z;
	this->nY = sys.nY;

	this->nlType = sys.nlType;
	this->diag_y = sys.diag_y;
	this->chance = sys.chance;

	return *this;
}
// ******************* SYS_T *******************
