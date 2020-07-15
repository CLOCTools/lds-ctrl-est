#include <iostream>
#include <chrono>
#include <matio.h>
#include <ldsCtrlEst>

using namespace std;
using namespace plds;

int main(void) {
	cout << "********** Testing LDS C++ library. **********  \n\n";
	cout << "********** Switched Poisson **********  \n\n";

	bool doSwitchCtrl = true;

	// Make SISO system sampled at 1kHz
	data_t dt = 1e-3;
	size_t nU = 1;
	size_t nX = 1;
	size_t nY = 1;

	// Scale factor between switched systems.
	// System 1 should be more sensitive than system 2 for AAW study.
	data_t scale_sysB = 2;

	armaMat A_true(nX,nX,fill::eye);
	A_true[0] = 0.98;
	armaMat B_true2 = armaMat(nX,nU).fill(0.03);
	armaMat B_true1 = armaMat(nX,nU).fill(scale_sysB*B_true2[0]);
	armaMat C_true(nY,nX,fill::eye);
	armaVec d_true = armaVec(nY,fill::zeros).fill(log(1*dt));

	size_t T = (size_t) 30.0/dt; //time steps for simulation.

	// simulate switching
	size_t gainState = 0;
	data_t pr21 = 1e-3;
	data_t pr12 = pr21;

	// true system...
	sys_t sys_true(nU, nX, nY, dt);
	sys_true.setA(A_true);
	sys_true.setB(B_true1);
	sys_true.setC(C_true);
	sys_true.setD(d_true);
	sys_true.reset();

	// control vars
	// Going to take the steady-state vals...
	armaMat Kc_x1(nU, nX, fill::ones);
	armaMat Kc_x2 = scale_sysB*Kc_x1;//system2 is x-times less sensitive.
	armaVec yRef = armaVec(nY).fill(20.0*dt);

	// AAW does not use integral action
	// size_t controlType = CONTROL_TYPE_INTY;
	// sys1.setControlType(controlType);
	// stdVec Kc_intyVec = stdVec(nU*nY, 25);

	data_t uLB = 0.0;
	data_t uUB = 5.0;
	ctrl_t sys1(nU, nX, nY, uLB, uUB, dt);
	sys1.setA(A_true);
	sys1.setB(B_true1);
	sys1.setC(C_true);
	sys1.setD(d_true);
	sys1.setKc_x(Kc_x1);
	sys1.reset();

	// set Q for filter...
	armaMat Q = sys1.getQ();
	Q.diag().fill(1e-2);
	sys1.setQ(Q);
	sys1.reset();

	cout << ".....................................\n";
	cout << "sys1:\n";
	cout << ".....................................\n";
	sys1.printSys();
	cout << ".....................................\n";

	ctrl_t sys2 = sys1;
	sys2.setB(B_true2);
	sys2.setKc_x(Kc_x2);
	cout << ".....................................\n";
	cout << "sys2:\n";
	cout << ".....................................\n";
	sys2.printSys();
	cout << ".....................................\n";

	// create 2-system composite switched-state controller.
	sctrl_t sys(2, nU, nX, nY, uLB, uUB, dt);
	sys.setYRef(yRef);
	sys.setSystem(0,sys1);
	sys.setSystem(1,sys2);
	sys.switchSystem(0);
	sys.reset();

	// Fake measurements
	armaMat z(nY, T, fill::zeros);

	// Will later contain control.
	armaMat u(nU, T, fill::zeros);

	// create armaMatrix to save outputs in...
	armaMat lambdaHat = armaMat(nY, T, fill::zeros);
	armaMat lambdaTrue = armaMat(nY, T, fill::zeros);
	armaMat lambdaRef = armaMat(nY, T, fill::ones)*yRef[0];

	// states and gain/disturbance params
	armaMat xHat = armaMat(nX, T, fill::zeros);
	armaMat xTrue = armaMat(nX, T, fill::zeros);
	armaMat state = armaMat(1, T, fill::zeros);

	// set initial val
	lambdaHat.submat(0,0,nY-1,0) = sys1.getY();
	lambdaTrue.submat(0,0,nY-1,0) = sys_true.getY();
	xHat.submat(0,0,nX-1,0) = sys1.getX();
	xTrue.submat(0,0,nX-1,0) = sys_true.getX();

	// some params.
	uLB = 0.01;
	bool gateEst = true;
	bool gateCtrl = true;

	cout << "Starting " << T*dt << " sec simulation ... \n";
	auto start = chrono::high_resolution_clock::now();
	for (size_t t=1; t<T; t++)
	{
		armaVec chance(1,fill::randu);
		if (gainState == 0) //state1
		{
				if (chance[0] < pr21) {
					gainState = 1;
					sys_true.setB(B_true2);
					if (doSwitchCtrl)
					sys.switchSystem(1);
				}
		} else { //state2
			if (chance[0] < pr12) {
				gainState = 0;
				sys_true.setB(B_true1);
				if (doSwitchCtrl)
				sys.switchSystem(0);
			}
		}

		// input
		armaVec u_tm1 = armaVec(u.colptr(t-1), u.n_rows, false, false);

		// Simulate the true system.
		sys_true.setU(u_tm1);
		sys_true.simPredict();
		armaVec z_k = armaVec(z.colptr(t), z.n_rows, false, false);
		sys_true.simMeasurement(z_k);

		// perform control
		sys.steadyState_logLin_fbCtrl(z_k, gateCtrl, gateEst);

		state.submat(0,t,0,t) = gainState+1;
		lambdaRef.submat(0,t,nY-1,t) = yRef;
		lambdaTrue.submat(0,t,nY-1,t) = sys_true.getY();
		xTrue.submat(0,t,nX-1,t) = sys_true.getX();
		lambdaHat.submat(0,t,nY-1,t) = sys.getY();
		xHat.submat(0,t,nX-1,t) = sys.getX();

		// write control to u
		armaVec u_t = armaVec(u.colptr(t), u.n_rows, false, false);
		u_t = sys.getU();
	}

	auto finish = chrono::high_resolution_clock::now();
	chrono::duration<data_t, milli> simTime_ms = finish - start;
	cout << "Finished simulation in " << simTime_ms.count() << " ms.\n";
	cout << "(app. " << (simTime_ms.count()/T)*1e3 << " us/time-step)\n";

	// create armaMatlab variables and save to armaMat file for plotting elsewhere...
	mat_t *matfile;
	matvar_t *matvar;

	matfile = Mat_CreateVer("test_plds_sctrl.mat",NULL,MAT_FT_DEFAULT);
	matio_types matDataType = MAT_T_DOUBLE;
	matio_compression matCompression = MAT_COMPRESSION_NONE;

	if(matfile)
	{
		std::cout << "Saving measurements and true vs. estimated outputs... \n";
		// matvar_t  *Mat_VarCreate(const char *name,enum matio_classes class_type, enum matio_types data_type,int rank,size_t *dims, void *data,int opt);

		// lambdaHat
		std::size_t dims[2] = {lambdaHat.n_rows, lambdaHat.n_cols};
		matvar = Mat_VarCreate("lambdaHat", MAT_C_DOUBLE, matDataType, 2, dims, lambdaHat.memptr(), 0);
		Mat_VarWrite(matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// xHat
		dims[0] = xHat.n_rows; dims[1] = xHat.n_cols;
		matvar = Mat_VarCreate("xHat", MAT_C_DOUBLE, matDataType, 2, dims, xHat.memptr(), 0);
		Mat_VarWrite(matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// simulated measurements.
		dims[0] = z.n_rows; dims[1] = z.n_cols;
		matvar = Mat_VarCreate("z", MAT_C_DOUBLE, matDataType, 2, dims, z.memptr(), 0);
		Mat_VarWrite(matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// inputs.
		dims[0] = u.n_rows; dims[1] = u.n_cols;
		matvar = Mat_VarCreate("u", MAT_C_DOUBLE, matDataType, 2, dims, u.memptr(), 0);
		Mat_VarWrite(matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// ref rate.
		dims[0] = lambdaRef.n_rows; dims[1] = lambdaRef.n_cols;
		matvar = Mat_VarCreate("lambdaRef", MAT_C_DOUBLE, matDataType, 2, dims, lambdaRef.memptr(), 0);
		Mat_VarWrite(matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// true rate.
		dims[0] = lambdaTrue.n_rows; dims[1] = lambdaTrue.n_cols;
		matvar = Mat_VarCreate("lambdaTrue", MAT_C_DOUBLE, matDataType, 2, dims, lambdaTrue.memptr(), 0);
		Mat_VarWrite(matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// xTrue
		dims[0] = xTrue.n_rows; dims[1] = xTrue.n_cols;
		matvar = Mat_VarCreate("xTrue", MAT_C_DOUBLE, matDataType, 2, dims, xTrue.memptr(), 0);
		Mat_VarWrite(matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// state
		dims[0] = state.n_rows; dims[1] = state.n_cols;
		matvar = Mat_VarCreate("state", MAT_C_DOUBLE, matDataType, 2, dims, state.memptr(), 0);
		Mat_VarWrite(matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// dt.
		double* dt_ptr = &dt;
		dims[0] = 1; dims[1] = 1;
		matvar = Mat_VarCreate("dt", MAT_C_DOUBLE, matDataType, 2, dims, dt_ptr, 0);
		Mat_VarWrite(matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		Mat_Close(matfile);
		std::cout << "Finished saving. \n";
	}
	else
	std::cout << "Dammit. Failed to open file... \n";

	return 0;
}
