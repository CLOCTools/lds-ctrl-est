#include <ldsCtrlEst>

using namespace std;
using namespace glds;

// ******************* CTRL_T *******************
//Constructor(s) for sys class
glds::ctrl_t::ctrl_t(size_t nU, size_t nX, size_t nY, data_t& uLB, data_t& uUB, data_t& dt, data_t& p0, data_t& q0, data_t& r0, size_t augmentation) :
glds::sys_t(nU, nX, nY, dt, p0, q0, r0),
uLB(uLB), uUB(uUB),
gateCtrl_prev(false), gateLock_prev(false),
gInverted(false), uSaturated(false), yRefLB(0.1*dt),
tauEst(0.1), tauAntiWindup(1e6),
t_since_ctrl_onset(0.0)
{
	uRef = armaVec(nU, fill::zeros);
	uRef_prev = armaVec(nU, fill::zeros);
	xRef = armaVec(nX, fill::zeros);
	yRef = armaVec(nY, fill::zeros);

	uSat = armaVec(nU, fill::zeros);

	//might not need this, so will not give them elements until I know they need them.
	Kc_x = armaMat(nU,nX, fill::zeros);
	// Likewise, might not need these, so zero elements until later.
	Kc_u = armaMat(0,0, fill::zeros);
	// Kc_m = armaMat(0,0, fill::zeros);
	Kc_y = armaMat(nU,nY, fill::zeros);
	Kc_inty = armaMat(0,0, fill::zeros);
	Kc_dy = armaMat(0,0, fill::zeros);

	dv = armaVec(nU, fill::zeros);
	v = armaVec(nU, fill::zeros);
	duRef = armaVec(nU, fill::zeros);
	dvRef = armaVec(nU, fill::zeros);
	vRef = armaVec(nU, fill::zeros);

	gDesign = g;

	intE = armaVec(0, fill::zeros);
	intE_awuAdjust = armaVec(0, fill::zeros);

	kAntiWindup = dt/tauAntiWindup;

	// perform any augmentations.
	augment(augmentation);
}

// Proportional + integral control on output
void glds::ctrl_t::piCtrl(armaVec& z, bool& gateCtrl, bool& gateLock, data_t& sigma_softStart, data_t& sigma_uNoise, bool& resetAtCtrlOnset) {

	setZ(z);
	update_expFilt();

	if (gateCtrl) {
		//consider resetting estimates each control epoch...
		if (!gateCtrl_prev) {
			if (resetAtCtrlOnset) {
				reset();
			}
			t_since_ctrl_onset = 0.0;
		} else {
			t_since_ctrl_onset += dt;
		}

		// enforce softstart on control vars.
		data_t softStart_sf = 1 - exp(-pow(t_since_ctrl_onset,2)/(2*pow(sigma_softStart,2)));
		uRef *= softStart_sf;
		yRef *= softStart_sf;

		if (!gateLock) {
			// first do g inversion change of vars. (v = g.*u)
			vRef = gDesign % uRef;

			//calc. the control
			v = vRef; //nominally-optimal.

			v -= Kc_y * (y - yRef); //instantaneous state error

			if (augmentation & AUGMENT_INTY) {
				// if (!uSaturated)
				intE += (y - yRef)*dt; //integrated error
				v -= Kc_inty*intE;//control for integrated error
			}

			// // TODO: add derivative...
			// if (augmentation & AUGMENT_DY) {
			// }

			u = v / getG();
			// check to see whether g has inverted from ref val
			// this would be indicative of depol block if g was pos -> neg
			// it would be indicative of hyperpolarization --> excitation
			// checkGainInversion();
		} //else do nothing until lock is low
	} else { //if not control
		u = uRef % gDesign/getG(); // feed through uRef in open loop
		uRef.zeros();
		intE.zeros();
		intE_awuAdjust.zeros();
		uSat.zeros();

		// reset some other stuff as well.
		// x=x0;
		h();
	} //ends gateCtrl

	if (sigma_uNoise>0.0)
	u += sigma_uNoise * armaVec(nU,fill::randn);

	// enforce box constraints
	// Newman et al. 2015 limited to [0 1], then scaled to dynamic range
	// limit(u, DATA_T_ZERO, DATA_T_ONE);
	// u *= uUB;

	// Limit u and if saturated, back-calculate integral error.
	antiWindup();

	gateCtrl_prev = gateCtrl;
	gateLock_prev = gateLock;
}

// Integral action + penalty of deltaU
void glds::ctrl_t::fbCtrl(armaVec& z, bool& gateCtrl, bool& gateLock, data_t& sigma_softStart, data_t& sigma_uNoise, bool& resetAtCtrlOnset, bool& doRecurse_Ke) {
	//update state estimates, given latest measurement
	update(z,doRecurse_Ke);

	calc_ctrl(gateCtrl, gateLock, sigma_softStart, sigma_uNoise, resetAtCtrlOnset);
}

// Integral action + penalty of deltaU
void glds::ctrl_t::steadyState_fbCtrl(armaVec& z, bool& gateCtrl, bool& gateEst, bool& gateLock, data_t& sigma_softStart, data_t& sigma_uNoise, bool& resetAtCtrlOnset, bool& doRecurse_Ke) {
	//update state estimates, given latest measurement
	if (gateEst) {
		update(z,doRecurse_Ke);
	}

	// calculate the set point @ steady state.
	if (gateCtrl)
	calc_ssSetPt();

	calc_ctrl(gateCtrl, gateLock, sigma_softStart, sigma_uNoise, resetAtCtrlOnset);
}//ends steadyState_fbCtrl

// private method for meat of control logic
void glds::ctrl_t::calc_ctrl(bool& gateCtrl, bool& gateLock, data_t& sigma_softStart, data_t& sigma_uNoise, bool& resetAtCtrlOnset) {

	if (gateCtrl) {
		//consider resetting estimates each control epoch...
		if (!gateCtrl_prev) {
			if (resetAtCtrlOnset) {
				reset();
			}
			t_since_ctrl_onset = 0.0;
		} else {
			t_since_ctrl_onset += dt;
		}

		// enforce softstart on control vars.
		data_t softStart_sf = 1 - exp(-pow(t_since_ctrl_onset,2)/(2*pow(sigma_softStart,2)));
		uRef *= softStart_sf;
		xRef *= softStart_sf;
		yRef *= softStart_sf;

		if (!gateLock) {
			duRef = uRef - uRef_prev;

			// first do g inversion change of vars. (v = g.*u)
			// e.g., convert into physical units v[=]mW/mm2 rather than u[=]V
			vRef = gDesign % uRef;
			dvRef = gDesign % duRef;

			//Given FB, calc. the change in control
			if (augmentation & AUGMENT_U) {
				dv = dvRef; //nominally-optimal.
				dv -= Kc_x * (getX() - xRef); //instantaneous state error
				dv -= Kc_u * (v - vRef); //penalty on changes in u

				if (augmentation & AUGMENT_INTY) {
					// if (!uSaturated)
					intE += (y - yRef)*dt; //integrated error
					dv -= Kc_inty * intE;//control for integrated error
				}

				// update the control
				v += dv;
			} else {
				v = vRef; //nominally-optimal.
				v -= Kc_x * (getX() - xRef); //instantaneous state error

				if (augmentation & AUGMENT_INTY) {
					// if (!uSaturated)
					intE += (y - yRef)*dt; //integrated error
					v -= Kc_inty * intE; //control for integrated error
				}
			}

			u = v / getG();
			// check to see whether g has inverted from nominal val
			// this would be indicative of depol block if g was pos -> neg
			// it would be indicative of hyperpolarization --> excitation
			// checkGainInversion();
		} //else do nothing until lock is low

		// It may be desireable to make inputs more variable.
    if (sigma_uNoise>0.0)
 		u += sigma_uNoise * armaVec(nU,fill::randn);

	} else { //if not control
		u = uRef % gDesign/getG(); // feed through uRef in open loop
		uRef.zeros();
		intE.zeros();
		intE_awuAdjust.zeros();
		uSat.zeros();

		// reset some other stuff as well.
		// x=x0;// <-- this should be unnecessary, so commenting out for now

    h();
	} //ends gateCtrl

	// enforce box constraints
	antiWindup();

	// now that we have a new input, make a prediction
	predict();

	gateCtrl_prev = gateCtrl;
	gateLock_prev = gateLock;
}//ends calc_ctrl

void glds::ctrl_t::augment(size_t augmentation, bool gateCtrl) {
	if (this->augmentation == augmentation)
	{
		if (!(this->augmentation & AUGMENT_G))
		return;
	}

	//starting over to be safe...but will take more time.
	deaugment();

	// /*
	// Do not allow for g-augmentation for now.
	if (augmentation & AUGMENT_G) {
		size_t INV_AUGMENT_G = ~AUGMENT_G;
		augmentation = augmentation & INV_AUGMENT_G;
	}
	// */

	glds::sys_t::augment(augmentation);

	if (augmentation & AUGMENT_U) {
		Kc_u.zeros(nU, nU);
		this->augmentation = this->augmentation | AUGMENT_U;
	}

	if (augmentation & AUGMENT_INTY) {
		Kc_inty.zeros(nU, nY);
		intE.zeros(nY);
		intE_awuAdjust.zeros(nY);
		this->augmentation = this->augmentation | AUGMENT_INTY;
	}

	// TODO : add Kc_dy, eyprev, etc.
	// if (augmentation & AUGMENT_DY) {
	// 	Kc_dy.zeros(nU, nY);
	// 	this->augmentation = this->augmentation | AUGMENT_DY;
	// }
}

void glds::ctrl_t::deaugment() {
	glds::sys_t::deaugment();
	Kc_u.zeros(0,0);
	Kc_inty.zeros(0,0);
	Kc_dy.zeros(0,0);
	intE.zeros(0,0);
	intE_awuAdjust.zeros(0,0);
}

// set methods
// make sure cannot user-define value of u.
void glds::ctrl_t::setU(stdVec& uVec) {
	// cerr << "Cannot assign a value to u in glds::ctrl_t.\n";
}
void glds::ctrl_t::setU(armaVec& u) {
	// cerr << "Cannot assign a value to u in glds::ctrl_t.\n";
}

void glds::ctrl_t::setG(stdVec& gVec) {
	if (!gInverted)
	glds::sys_t::setG(gVec);
	// else
	// cout << "Gain is inverted currently. Ignoring setG() calls.\n";
}

void glds::ctrl_t::setG(armaVec& g) {
	if (!gInverted)
	glds::sys_t::setG(g);
	// else
	// cout << "Gain is inverted currently. Ignoring setG() calls.\n";
}

void glds::ctrl_t::setGDesign(stdVec& gVec) {
	reassign(gDesign, gVec);
}
void glds::ctrl_t::setGDesign(armaVec& g) {
	reassign(gDesign, g);
}

void glds::ctrl_t::setURef(stdVec& uRefVec) {
	uRef_prev = uRef;
	reassign(uRef, uRefVec);
}
void glds::ctrl_t::setURef(armaVec& uRef) {
	uRef_prev = this->uRef;
	reassign(this->uRef, uRef);
}

void glds::ctrl_t::setXRef(stdVec& xRefVec) {
	reassign(xRef, xRefVec);
}
void glds::ctrl_t::setXRef(armaVec& xRef) {
	reassign(this->xRef, xRef);
}

void glds::ctrl_t::setYRef(stdVec& yRefVec) {
	reassign(yRef, yRefVec);
}
void glds::ctrl_t::setYRef(armaVec& yRef) {
	reassign(this->yRef, yRef);
}

void glds::ctrl_t::setKc_x(stdVec& Kc_x_vec) {
	reassign(Kc_x, Kc_x_vec);
}
void glds::ctrl_t::setKc_x(armaVec& Kc_x) {
	reassign(this->Kc_x, Kc_x);
}

void glds::ctrl_t::setKc_u(stdVec& Kc_u_vec) {
	if (augmentation & AUGMENT_U)
	reassign(Kc_u, Kc_u_vec);
}
void glds::ctrl_t::setKc_u(armaVec& Kc_u) {
	if (augmentation & AUGMENT_U)
	reassign(this->Kc_u, Kc_u);
}

void glds::ctrl_t::setKc_y(stdVec& Kc_y_vec) {
	reassign(Kc_y, Kc_y_vec);
}
void glds::ctrl_t::setKc_y(armaVec& Kc_y) {
	reassign(this->Kc_y, Kc_y);
}

void glds::ctrl_t::setKc_inty(stdVec& Kc_inty_vec) {
	if (augmentation & AUGMENT_INTY)
	reassign(Kc_inty, Kc_inty_vec);
}
void glds::ctrl_t::setKc_inty(armaVec& Kc_inty) {
	if (augmentation & AUGMENT_INTY)
	reassign(this->Kc_inty, Kc_inty);
}

void glds::ctrl_t::setKc_dy(stdVec& Kc_dy_vec) {
	if (augmentation & AUGMENT_DY)
	reassign(Kc_dy, Kc_dy_vec);
}
void glds::ctrl_t::setKc_dy(armaVec& Kc_dy) {
	if (augmentation & AUGMENT_DY)
	reassign(this->Kc_dy, Kc_dy);
}

void glds::ctrl_t::setTauAntiWindup(data_t& tau) {
	tauAntiWindup = tau;
	kAntiWindup = dt/tauAntiWindup;
}

// check whether there was a g inversion indicative of depol block (or over-inhibition leading to excitation)
void glds::ctrl_t::checkGainInversion() {
	armaVec gSq = g % getG();
	for (size_t k=0; k<gSq.n_elem; k++) {
		if (gSq[k]<0) {
			// cerr << "DETECTED GAIN INVERSION! RESETTING TO 0.1x GAIN.\n";
			gInverted = true;
			g *= 0.1;
			setG(g);//just in case the reference to x0 was off
			x = x0;
			defaultQ();
			return;
		}
	}

	// if didn't get caught in any of those tests, not inverted.
	gInverted = false;
}

void glds::ctrl_t::calc_ssSetPt() {
	// TODO: this should be weighted least squares for MIMO case that puts high cost on violating dynamics (i.e., dx=0 at steady state).

	// This is a waste to do every timestep, but because I have separate set* methods for A,B,C, this is the only safe way to do it...
	// TODO: replace setA,setB, etc. with single setSysParams(A,B,g,m,C), and compute there so this can be avoided? setSysParams should be private to lds::sys_t so it doesn't get inherited for glds/glds
	armaMat phi_ss = join_horiz(getA()-armaMat(nX,nX,fill::eye), getB()*arma::diagmat(g));
	phi_ss = join_vert(phi_ss, join_horiz(getC(), armaMat(nY,nU,fill::zeros)));
	//TODO: consider pinv()? Would at least give least-sq solution if can't invert
	armaVec xu_ss = inv(phi_ss) * join_vert(-m, yRef-d);

	// WARNING: Do not do the below unless you are okay with the fact that since m is now time-varying, the steady-state solution is not appropriate.
	// This is essentially active disturbance rejection, assuming the plant is a static gain
	// xu_ss = inv(phi_ss) * join_vert(-getM(), yRef-d); //TODO: consider pinv()?

	xRef.subvec(0,nX-1) = xu_ss.subvec(0,nX-1);
	uRef = xu_ss.subvec(nX,nX+nU-1);
}

void glds::ctrl_t::update_expFilt() {
	// TODO: this seems unnecessary bc could be done in the tau set-method, but currently dt is a reference to externally-owned value which is free to change. So, this has to be allowed to change as dt changes.
	aEst = exp(-dt/tauEst);
	bEst = 1.0 - aEst;

	for (size_t k=0; k<z.size(); k++)
	y[k] = aEst*y[k] + bEst*z[k];
}

void glds::ctrl_t::antiWindup() {
	uSaturated = false;
	uSat = u;

	for (size_t k=0; k<u.n_elem; k++)
	{
		if (u[k]<uLB) {
			uSat[k] = uLB;
			uSaturated=true;
		}

		if (u[k]>uUB) {
			uSat[k] = uUB;
			uSaturated=true;
		}
	}

	if (augmentation & AUGMENT_INTY) {
		// one-step back-calculation (Astroem, Rundqwist 1989 warn against using this...)
		// armaVec delta_intE = solve(Kc_inty, (u-uSat)); //pinv(Kc_inty) * (u-uSat);
		// intE += delta_intE;

		// gradual: see Astroem, Rundqwist 1989
		// my fudge for doing MIMO gradual
		// n.b., went ahead and multiplied 1/T by dt so don't have to do that here.
		intE_awuAdjust += kAntiWindup * (sign(Kc_inty).t()/nU) * (u-uSat);
		intE += intE_awuAdjust;
	}

	u = uSat;
}

void glds::ctrl_t::reset() {
	glds::sys_t::reset();

	uRef.zeros();
	uRef_prev.zeros();
	xRef.zeros();
	yRef.zeros();
	intE.zeros();
	intE_awuAdjust.zeros();

	uSat.zeros();

	gInverted = false;
	uSaturated = false;

	t_since_ctrl_onset = 0.0;
}

void glds::ctrl_t::printSys() {
	glds::sys_t::printSys();
	std::cout << "uLB : " << uLB << "\n";
	std::cout << "uUB : " << uUB << "\n";
}

glds::ctrl_t& glds::ctrl_t::operator=(const glds::ctrl_t& sys)
{
	// // would love to be able to re-use the lds code:
	// (*this) = lds::sys_t::operator=(sys);
	// // but this does not work bc the input is glds::sys_t which is a subclass of lds::sys_t
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

	// from GLDS
	this->C = sys.C;
	this->d = sys.d;
	this->R = sys.R;
	this->r0 = sys.r0;

	this->Ke = sys.Ke;

	this->y = sys.y;
	this->z = sys.z;
	this->nY = sys.nY;
	// end from GLDS

	// additional ctrl_t stuff
	this->gDesign = sys.gDesign;
	this->uRef = sys.uRef;
	this->uRef_prev = sys.uRef_prev;
	this->xRef = sys.xRef;
	this->yRef = sys.yRef;
	this->Kc_x = sys.Kc_x;
	this->Kc_u = sys.Kc_u;
	// this->Kc_m = sys.Kc_m;
	this->Kc_inty = sys.Kc_inty;
	this->duRef = sys.duRef;
	this->dvRef = sys.dvRef;
	this->vRef = sys.vRef;
	this->dv = sys.dv;
	this->v = sys.v;
	this->intE = sys.intE;
	this->intE_awuAdjust = sys.intE_awuAdjust;
	this->uLB = sys.uLB;
	this->uUB = sys.uUB;

	this->tauEst = sys.tauEst;
	this->uSat = sys.uSat;

	this->gInverted = sys.gInverted;
	this->uSaturated = sys.uSaturated;

	this->t_since_ctrl_onset = sys.t_since_ctrl_onset;
	return *this;
}

void glds::ctrl_t::setDims(size_t& nU, size_t& nX, size_t& nY) {
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
		(*this)=glds::ctrl_t(nU,nX,nY,uLB,uUB,dt,p0,q0,r0,augmentation);
		szChanged=false;
	}
}
// ******************* CTRL_T *******************
