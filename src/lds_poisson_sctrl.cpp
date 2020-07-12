#include <ldsCtrlEst>

using namespace std;
using namespace plds;

// ******************* SCTRL_T *******************
//Constructor(s) for sys class
plds::sctrl_t::sctrl_t(size_t nSys, size_t nU, size_t nX, size_t nY, data_t& uLB, data_t& uUB, data_t& dt, data_t& p0, data_t& q0, size_t controlType) :
plds::ctrl_t(nU, nX, nY, uLB, uUB, dt, p0, q0, controlType),
nSys(nSys), sysIdx(0)
{
	ctrl_t sys0(nU,nX,nY,uLB,uUB,dt,p0,q0,controlType);
	systems = std::vector<ctrl_t>(nSys,sys0);
	adaptM = false;
}

void plds::sctrl_t::setSystem(size_t sysIdx, ctrl_t& sys)
{
	// Make sure system Idx in bounds
	if ((sysIdx+1) > nSys)
	{
		cerr << "Provided system index is not consistent with switched system! Skipping." << endl;
		return;
	}

	// check to make sure dimensions correct.
	if ((sys.getNx()==nX) & (sys.getNu()==nU) & (sys.getNy()==nY))
	systems[sysIdx] = sys;
	else
	cerr << "System dimensions are not consistent with switched system! Skipping." << endl;

	// assign to parent
	this->sysIdx = sysIdx+1;
	switchSystem(sysIdx);
}

void plds::sctrl_t::switchSystem(size_t sysIdx)
{
	// Make sure system Idx in bounds
	if ((sysIdx+1) > nSys)
	{
		cerr << "Provided system index is not consistent with switched system! Skipping." << endl;
		return;
	}

	if (sysIdx==this->sysIdx)
	return;//already there.

	if (adaptM)
	{
		// Need to recalc equivalent disturbance for new dynamics [A]
		// TODO: This is a fudge. It is the disturbance such that the next time step prediction should be the same for both systems.
		m += (A - systems[sysIdx].getA())*x;
	}
	else
	m = systems[sysIdx].getM0();

	// Necessary params.
	A = systems[sysIdx].getA();
	B = systems[sysIdx].getB();
	Q = systems[sysIdx].getQ();
	Q_m = systems[sysIdx].getQ_m();
	C = systems[sysIdx].getC();
	d = systems[sysIdx].getD();
	x0 = systems[sysIdx].getX0();
	m0 = systems[sysIdx].getM0();
	P0 = systems[sysIdx].getP0();
	P0_m = systems[sysIdx].getP0_m();

	Kc_u = systems[sysIdx].getKc_u();
	Kc_x = systems[sysIdx].getKc_x();
	Kc_inty = systems[sysIdx].getKc_inty();

	// g = systems[sysIdx].getG();
	// gDesign = systems[sysIdx].getGDesign();

	this->sysIdx = sysIdx;
}

// ******************* SCTRL_T *******************
