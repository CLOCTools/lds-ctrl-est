#include <ldsCtrlEst>

using namespace std;
using namespace plds;

// ******************* CTRL_T *******************
//Constructor(s) for sys class
plds::ctrl_t::ctrl_t(size_t nU, size_t nX, size_t nY, data_t& uLB, data_t& uUB, data_t& dt, data_t& p0, data_t& q0, size_t augmentation) :
plds::sys_t(nU, nX, nY, dt, p0, q0),
uLB(uLB), uUB(uUB),
gateCtrl_prev(false), gateLock_prev(false),
gInverted(false), uSaturated(false), yRefLB(0.1*dt),
tauEst(0.1), tauAntiWindup(1e6),
t_since_ctrl_onset(0.0)
{
	uRef = armaVec(nU, fill::zeros);
	uRef_prev = armaVec(nU, fill::zeros);
	xRef = armaVec(nX, fill::zeros);
	logyRef = armaVec(nY, fill::zeros);
	yRef = armaVec(nY, fill::zeros);

	uSat = armaVec(nU, fill::zeros);

	//might not need this, so will not give them elements until I know they need them.
	Kc_x = armaMat(nU,nX, fill::zeros);
	// Likewise, might not need these, so zero elements until later.
	Kc_u = armaMat(0,0, fill::zeros);
	Kc_y = armaMat(nU,nY, fill::zeros);
	Kc_inty = armaMat(0,0, fill::zeros);
	Kc_dy = armaMat(0,0, fill::zeros);

	gDesign = g;

	dv = armaVec(nU, fill::zeros);
	v = armaVec(nU, fill::zeros);
	duRef = armaVec(nU, fill::zeros);
	dvRef = armaVec(nU, fill::zeros);
	vRef = armaVec(nU, fill::zeros);

	intE = armaVec(0, fill::zeros);

	kAntiWindup = dt/tauAntiWindup;

	// perform any augmentations.
	augment(augmentation);
}

// Proportional + integral control on output
void plds::ctrl_t::piCtrl(armaVec& z, bool& gateCtrl, bool& gateLock, data_t& sigma_softStart, data_t& sigma_uNoise, bool& resetAtCtrlOnset) {
	//update state estimates, given latest measurement
	setZ(z);
	update_expFilt();

	if (gateCtrl) {
		//consider resetting estimate each control epoch...
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
			v -= Kc_y * (y - yRef); //instantaneous error

			if (augmentation & AUGMENT_INTY) {
				// if (!uSaturated)
				intE += (y - yRef)*dt; //integrated error
				v -= Kc_inty*intE;//control for integrated error
			}

			// // TODO: add derivative...
			// if (augmentation & AUGMENT_DY) {
			// }

			// update the control
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
		// x=x0; TODO: Is this necessary?
		h();
	} //ends gateCtrl

	if (sigma_uNoise>0.0)
	u += sigma_uNoise * armaVec(nU,fill::randn);

	// enforce box constraints
	// Newman et al. 2015 limited to +/- 1, then scaled to dynamic range
	// limit(u, DATA_T_ZERO, DATA_T_ONE);
	// u *= uUB;

	// Limit u and if saturated, back-calculate integral error.
	antiWindup();

	gateCtrl_prev = gateCtrl;
	gateLock_prev = gateLock;
}

// Most generic
void plds::ctrl_t::fbCtrl(armaVec& z, bool& gateCtrl, bool& gateLock, data_t& sigma_softStart, data_t& sigma_uNoise, bool& resetAtCtrlOnset) {
	//update state estiarmaMates, given latest measurement
	update(z);

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
			duRef = uRef - uRef_prev;

			// first do g inversion change of vars. (v = g.*u)
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

			// undo the change of variables
			// if gain is augmented, this is a nonlinear control policy
			u = v / getG();

			// check to see whether g has inverted from nominal val
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
		// x=x0; TODO: Is this necessary?
		h();
	} //ends gateCtrl

	// n.b., control creates inputs that are highly correlated with state/params. Since this is not accounted for in the estiarmaMation process, leads to biased estiarmaMates!
	// There are more principled ways to go about this (that I don't know), but ...
	// Adding uncorrelated noise helps the estiarmaMator from converging on wrong params
	// Besides, it may be desireable to make inputs more variable.
	if (sigma_uNoise>0.0)
	u += sigma_uNoise * armaVec(nU,fill::randn);

	// enforce box constraints
	// because gradient for input disappears when there is no input, consider adding a small light level at all times...
	antiWindup();

	// now that we have a new input, make a prediction
	predict();

	gateCtrl_prev = gateCtrl;
	gateLock_prev = gateLock;
}//ends fbCtrl

// private meat of ctrl around log-linear system
void plds::ctrl_t::calc_logLinCtrl(bool& gateCtrl, bool& gateLock, data_t& sigma_softStart, data_t& sigma_uNoise, bool& resetAtCtrlOnset) {

	// estimator update called in parent fn

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
		logyRef *= softStart_sf;

		duRef = uRef - uRef_prev;
		uRef_prev = uRef;

		if (!gateLock) {
			// first do g inversion change of vars. (v = g.*u)
			vRef = gDesign % uRef;
			dvRef = gDesign % duRef;

			//Given FB, calc. the change in control
			if (augmentation & AUGMENT_U) {
				dv = dvRef; //nominally-optimal.
				dv -= Kc_x * (getX() - xRef); //instantaneous state error
				dv -= Kc_u * (v - vRef); //penalty on changes in u

				// if (augmentation & AUGMENT_M)
				// dv -= Km * (getM() - d); //changes in d

				if (augmentation & AUGMENT_INTY) {
					// if (!uSaturated)
					intE += (logy - logyRef)*dt; //integrated error
					dv -= Kc_inty * intE;//control for integrated error
				}

				// update the control
				v += dv;
			} else {
				v = vRef; //nominally-optimal.
				v -= Kc_x * (getX() - xRef); //instantaneous state error

				if (augmentation & AUGMENT_INTY) {
					// if (!uSaturated)
					intE += (logy - logyRef)*dt; //integrated error
					v -= Kc_inty * intE; //control for integrated error
				}
			}

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
		// x=x0; TODO: Is this necessary?
		h();
	} //ends gateCtrl

	// it may be desireable to make inputs more variable.
	if (sigma_uNoise>0.0)
	u += sigma_uNoise * armaVec(nU,fill::randn);

	// enforce box constraints
	antiWindup();
	// limit(u, uLB, uUB);

	// now that we have a new input, make a prediction
	predict();

	gateCtrl_prev = gateCtrl;
	gateLock_prev = gateLock;
}//ends calc_logLinCtrl

void plds::ctrl_t::logLin_fbCtrl(armaVec& z, bool& gateCtrl, bool& gateLock, data_t& sigma_softStart, data_t& sigma_uNoise, bool& resetAtCtrlOnset) {

	update(z);

	calc_logLinCtrl(gateCtrl, gateLock, sigma_softStart, sigma_uNoise, resetAtCtrlOnset);
}

void plds::ctrl_t::steadyState_logLin_fbCtrl(armaVec& z, bool& gateCtrl, bool& gateEst, bool& gateLock, data_t& sigma_softStart, data_t& sigma_uNoise, bool& resetAtCtrlOnset) {

	if (gateEst) {
		update(z);
	} else {
		// Need to make sure that P doesn't grow with repeated calls of predict without calls to update()...
		defaultQ();
	}

	if (gateCtrl)
	{
		//given reference & params, calculate steady-state set-point [xRef,uRef]
		calc_ssSetPt();
	}

	calc_logLinCtrl(gateCtrl, gateLock, sigma_softStart, sigma_uNoise, resetAtCtrlOnset);
}

void plds::ctrl_t::augment(size_t augmentation, bool gateCtrl) {
	if (this->augmentation == augmentation)
	{
		if (!(this->augmentation & AUGMENT_G))
		return;
	}

	//starting over to be safe...but will take more time
	deaugment();

	// /*
	// Do not allow for g-augmentation for now.
	if (augmentation & AUGMENT_G) {
		size_t INV_AUGMENT_G = ~AUGMENT_G;
		augmentation = augmentation & INV_AUGMENT_G;
	}
	// */

	plds::sys_t::augment(augmentation);

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

void plds::ctrl_t::deaugment() {
	plds::sys_t::deaugment();
	Kc_u.zeros(0,0);
	Kc_inty.zeros(0,0);
	Kc_dy.zeros(0,0);
	intE.zeros(0,0);
	intE_awuAdjust.zeros(0,0);
}

// set methods
// make sure cannot user-define value of u.
void plds::ctrl_t::setU(stdVec& uVec) {
	// cerr << "Cannot assign a value to u in plds::ctrl_t.\n";
}
void plds::ctrl_t::setU(armaVec& u) {
	// cerr << "Cannot assign a value to u in plds::ctrl_t.\n";
}

void plds::ctrl_t::setG(stdVec& gVec) {
	if (!gInverted)
	plds::sys_t::setG(gVec);
	// else
	// cout << "Gain is inverted currently. Ignoring setG() calls.\n";
}
void plds::ctrl_t::setG(armaVec& g) {
	if (!gInverted)
	plds::sys_t::setG(g);
	// else
	// cout << "Gain is inverted currently. Ignoring setG() calls.\n";
}

void plds::ctrl_t::setGDesign(stdVec& gVec) {
	reassign(gDesign, gVec);
}
void plds::ctrl_t::setGDesign(armaVec& g) {
	reassign(gDesign, g);
}

void plds::ctrl_t::setURef(stdVec& uRefVec) {
	uRef_prev = uRef;
	reassign(uRef, uRefVec);
}
void plds::ctrl_t::setURef(armaVec& uRef) {
	uRef_prev = this->uRef;
	reassign(this->uRef, uRef);
}

void plds::ctrl_t::setXRef(stdVec& xRefVec) {
	reassign(xRef, xRefVec);
}
void plds::ctrl_t::setXRef(armaVec& xRef) {
	reassign(this->xRef, xRef);
}

void plds::ctrl_t::setYRef(stdVec& yRefVec) {
	reassign(yRef, yRefVec);

	//make sure to enforce lower limit on yRef
	limit(yRef, yRefLB, inf);
	logyRef = log(yRef);
}
void plds::ctrl_t::setYRef(armaVec& yRef) {
	reassign(this->yRef, yRef);

	//make sure to enforce lower limit on yRef
	limit(this->yRef, yRefLB, inf);
	logyRef = log(this->yRef);
}

void plds::ctrl_t::setKc_x(stdVec& Kc_xVec) {
	reassign(Kc_x, Kc_xVec);
}
void plds::ctrl_t::setKc_x(armaVec& Kc_x) {
	reassign(this->Kc_x, Kc_x);
}

void plds::ctrl_t::setKc_u(stdVec& Kc_uVec) {
	if (augmentation & AUGMENT_U)
	reassign(Kc_u, Kc_uVec);
}
void plds::ctrl_t::setKc_u(armaVec& Kc_u) {
	if (augmentation & AUGMENT_U)
	reassign(this->Kc_u, Kc_u);
}

void plds::ctrl_t::setKc_y(stdVec& Kc_yVec) {
	reassign(Kc_y, Kc_yVec);
}
void plds::ctrl_t::setKc_y(armaVec& Kc_y) {
	reassign(this->Kc_y, Kc_y);
}

void plds::ctrl_t::setKc_inty(stdVec& Kc_intyVec) {
	if (augmentation & AUGMENT_INTY)
	reassign(Kc_inty, Kc_intyVec);
}
void plds::ctrl_t::setKc_inty(armaVec& Kc_inty) {
	if (augmentation & AUGMENT_INTY)
	reassign(this->Kc_inty, Kc_inty);
}

void plds::ctrl_t::setKc_dy(stdVec& Kc_dyVec) {
	if (augmentation & AUGMENT_DY)
	reassign(Kc_dy, Kc_dyVec);
}
void plds::ctrl_t::setKc_dy(armaVec& Kc_dy) {
	if (augmentation & AUGMENT_DY)
	reassign(this->Kc_dy, Kc_dy);
}

void plds::ctrl_t::setTauAntiWindup(data_t& tau) {
	tauAntiWindup = tau;
	kAntiWindup = dt/tauAntiWindup;
}

// check whether there was a g inversion indicative of depol block (or over-inhibition leading to excitation)
void plds::ctrl_t::checkGainInversion() {
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

void plds::ctrl_t::calc_ssSetPt() {
	// This is a waste to do every timestep, but because I have separate set* methods for A,B,C, this is the only safe way to do it...
	// TODO: replace setA,setB, etc. with single setSysParams(A,B,g,m,C), and compute there so this can be avoided? setSysParams should be private to lds::sys_t so it doesn't get inherited for plds/glds
	armaMat phi_ss = join_horiz(getA()-armaMat(nX,nX,fill::eye), getB()*arma::diagmat(g));
	phi_ss = join_vert(phi_ss, join_horiz(getC(), armaMat(nY,nU,fill::zeros)));
	//TODO: consider pinv()? Would at least give least-sq solution if can't invert
	armaVec xu_ss = inv(phi_ss) * join_vert(-m, logyRef-d);

	// WARNING: Do not do the below unless you are okay with the fact that since m is now time-varying, the steady-state solution is not appropriate.
	// xu_ss = inv(phi_ss) * join_vert(-getM(), yRef-d); //TODO: consider pinv()?

	xRef.subvec(0,nX-1) = xu_ss.subvec(0,nX-1);
	uRef = xu_ss.subvec(nX,nX+nU-1);
}

void plds::ctrl_t::update_expFilt() {
	aEst = exp(-dt/tauEst);
	bEst = 1.0 - aEst;

	for (size_t k=0; k<z.size(); k++)
	y[k] = aEst*y[k] + bEst*z[k];
}

void plds::ctrl_t::antiWindup() {
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
		// n.b., went ahead and multipled 1/T by dt so don't have to do that here.
		intE_awuAdjust += kAntiWindup * (sign(Kc_inty).t()/nU) * (u-uSat);
		intE += intE_awuAdjust;
	}

	u = uSat;
}

void plds::ctrl_t::reset() {
	plds::sys_t::reset();

	uRef.zeros();
	uRef_prev.zeros();
	// xRef.zeros();
	// logyRef.zeros();
	// yRef.zeros();
	intE.zeros();
	intE_awuAdjust.zeros();

	uSat.zeros();

	gInverted = false;
	uSaturated = false;

	t_since_ctrl_onset = 0.0;
}

void plds::ctrl_t::printSys() {
	plds::sys_t::printSys();
	std::cout << "gDesign : " << gDesign << "\n";
	std::cout << "uLB : " << uLB << "\n";
	std::cout << "uUB : " << uUB << "\n";
}

plds::ctrl_t& plds::ctrl_t::operator=(const plds::ctrl_t& sys)
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
	this->m = sys.m;
	this->g = sys.g;

	this->x = sys.x;
	this->P = sys.P;

	this->dt = sys.dt;
	this->q0 = sys.q0;

	this->nX = sys.nX;
	this->nU = sys.nU;
	this->nXaug = sys.nXaug;

	this->augmentation = sys.augmentation;
	this->diag_u = sys.diag_u;
	// END FROM LDS

	// from PDLS
	this->C = sys.C;
	this->d = sys.d;

	this->y = sys.y;
	this->z = sys.z;
	this->nY = sys.nY;

	this->nlType = sys.nlType;
	this->diag_y = sys.diag_y;
	this->chance = sys.chance;
	// end from PLDS

	// additional ctrl_t stuff
	this->gDesign = sys.gDesign;
	this->uRef = sys.uRef;
	this->uRef_prev = sys.uRef_prev;
	this->xRef = sys.xRef;
	this->logyRef = sys.logyRef;
	this->yRef = sys.yRef;
	this->Kc_x = sys.Kc_x;
	this->Kc_u = sys.Kc_u;
	// this->Km = sys.Km;
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
// ******************* CTRL_T *******************
