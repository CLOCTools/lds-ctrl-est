#include <iostream>
#include <chrono>
#include <matio.h>
#include <ldsCtrlEst>

using namespace std;
using namespace plds;

// for smoothing things..
armaMat smooth(armaMat& x, data_t tau, data_t dt);

stdVec armaVec2stdVec(armaVec x) { return arma::conv_to<stdVec>::from(x); };

int main(void) {
	cout << "********** Testing LDS C++ library. **********  \n\n";
	cout << "********** Poisson **********  \n\n";

	// Make SISO system sampled at 1kHz
	data_t dt = 1e-3;
	size_t nU = 1;
	size_t nX = 1;
	size_t nY = 1;

	data_t q0_true = 1e-12;
	data_t q0 = 1e-10;
	data_t p0 = 1e-9;

	// Going to hard-code values from a real fit (pooled 9/16/16 + 9/20/16 rat data)
	armaMat aArma_true = armaMat(nX,nX,fill::eye);
	aArma_true[0] = 0.986;//exp(-dt/0.075);
	armaMat bArma_true = armaMat(nX,nU,fill::eye);
	bArma_true[0] = 0.027;

	// simulate switching fr
	size_t frState = 0;
	data_t mLo_true = log(5*dt) * (1-aArma_true[0]);
	data_t prLo2Hi = 1e-3;
	data_t mHi_true = log(20*dt) * (1-aArma_true[0]);
	data_t prHi2Lo = prLo2Hi;

	stdVec mVec_true = stdVec(nY,mLo_true);
	stdVec gVec_true = stdVec(nY,2.0);
	size_t K = (size_t) 30.0/dt; //time steps for simulation.

	// true system...
	sys_t sys_true(nU, nX, nY, dt, p0, q0_true);

	//test the re-assignment method..
	sys_true = sys_t(nU, nX, nY, dt, p0, q0_true);

	// Assign params.
	sys_true.setA(aArma_true);
	sys_true.setB(bArma_true);
	sys_true.setM(mVec_true);
	sys_true.setG(gVec_true);

	// armaVec x0_true = inv(armaMat(nX,nX,fill::eye) - sys_true.getA())*sys_true.getM();
	armaVec x0_true(nX);
	x0_true.fill(log(20*dt));
	sys_true.setX0(x0_true);

	sys_true.reset();

	// control vars
	// Going to take the steady-state vals...
	stdVec Kc_xVec = stdVec(nU*nX, 1);
	stdVec Kc_intyVec = stdVec(nU*nY, 250);
	stdVec uRefVec = stdVec(nU, 0.45);
	stdVec xRefVec = stdVec(nX, log(20.0*dt));
	stdVec yRefVec = stdVec(nY, 20.0*dt);

	// the more uncorrelated noise, the less biased parameter estiarmaMates will be...
	data_t uSigma = 0e-3;

	// Fake measurements
	armaMat z(nY, K, fill::zeros);

	// Will later contain control.
	armaMat u(nU, K, fill::zeros);

	cout << ".....................................\n";
	cout << "sys_true:\n";
	cout << ".....................................\n";
	sys_true.printSys();
	cout << ".....................................\n";

	// Create *incorrect* model.
	armaVec gSys = armaVec(nU,fill::ones);
	armaVec mSys = armaVec(nY,fill::zeros).fill(mLo_true);

	data_t uLB = 0.0;
	data_t uUB = 5.0;
	ctrl_t sys(nU, nX, nY, uLB, uUB, dt, p0, q0);
	// go ahead and set it to the wrong vals...
	sys.setG(gSys);
	sys.setM(mSys);

	// augment
	size_t augmentation = AUGMENT_M;
	// augmentation = augmentation | AUGMENT_G;
	augmentation = augmentation | AUGMENT_INTY;
	sys.augment(augmentation);

	auto Q = sys.getQ();
	Q.diag()[nX]=1e-6;//disturbance
	if (augmentation & AUGMENT_G)
	Q.diag()[nX+nX]=1e-3;
	sys.setQ(Q);

	// Create vectors
	stdVec aVec = armaVec2stdVec( vectorise(sys_true.getA()) );
	stdVec bVec = armaVec2stdVec( vectorise(sys_true.getB()) );
	stdVec dVec = armaVec2stdVec( vectorise(sys_true.getD()) );
	stdVec qVec = armaVec2stdVec( vectorise(Q) );

	stdVec x0Vec = armaVec2stdVec( x0_true );
	stdVec gVec = armaVec2stdVec( sys.getG() );
	stdVec mVec = armaVec2stdVec( sys.getM() );

	// assign params.
	sys.setDims(nU, nX, nY);
	sys.setA(aVec);
	sys.setB(bVec);
	sys.setQ(qVec);
	sys.setD(dVec);
	sys.setM(mVec);
	sys.setG(gVec);
	sys.setX0(x0Vec);
	sys.reset();

	cout << ".....................................\n";
	cout << "sys_hat:\n";
	cout << ".....................................\n";
	sys.printSys();
	cout << ".....................................\n";

	bool isAugmented = sys.checkIfAugmented();
	cout << "Is sys_hat augmented? " << isAugmented << "\n";

	// create armaMatrix to save outputs in...
	armaMat lambdaHat = armaMat(nY, K, fill::zeros);
	armaMat lambdaTrue = armaMat(nY, K, fill::zeros);
	armaMat lambdaRef = armaMat(nY, K, fill::ones)*yRefVec[0];

	// states and gain/disturbance params
	armaMat xHat = armaMat(nX, K, fill::zeros);
	armaMat mHat = armaMat(nY, K, fill::zeros);
	armaMat gHat = armaMat(nU, K, fill::zeros);

	armaMat xTrue = armaMat(nX, K, fill::zeros);
	armaMat mTrue = armaMat(nY, K, fill::zeros);
	armaMat gTrue = armaMat(nU, K, fill::zeros);

	sys_true.reset();
	sys.reset();

	// set initial val
	lambdaHat.submat(0,0,nY-1,0) = sys.getY();
	lambdaTrue.submat(0,0,nY-1,0) = sys_true.getY();

	xHat.submat(0,0,nX-1,0) = sys.getX();
	xTrue.submat(0,0,nX-1,0) = sys_true.getX();

	mHat.submat(0,0,nX-1,0) = sys.getM();
	mTrue.submat(0,0,nX-1,0) = sys_true.getM();

	gHat.submat(0,0,nX-1,0) = sys.getG();
	gTrue.submat(0,0,nX-1,0) = sys_true.getG();

	bool gateCtrl = true;
	bool gateLock = false;

	// test constructor pass by ref:
	// make uLB something else...
	uLB = 0.01;

	// tau antiWindup
	data_t tauAntiWindup = 1e6;
	sys.setTauAntiWindup(tauAntiWindup);

	data_t sigma_softStart = 0.1;

	cout << "Starting " << K*dt << " sec simulation ... \n";
	auto start = chrono::high_resolution_clock::now();
	for (size_t k=1; k<K; k++)
	{
		armaVec chance(1,fill::randu);
		bool doSwitch = false;
		if (frState == 0) //low state
		{
				if (chance[0] < prLo2Hi) {
					mVec_true = stdVec(nY,mHi_true);
					frState = 1;
					// cout << "Switching low -> high FR...\n";
				}
		} else { //high state
			if (chance[0] < prHi2Lo) {
				mVec_true = stdVec(nY,mLo_true);
				frState = 0;
				// cout << "Switching high -> low FR...\n";
			}
		}
		sys_true.setM(mVec_true);

		// Assign params (uncomment to simulate online conditions)
		// /*
		sys.setDims(nU, nX, nY);
		sys.setA(aVec);
		sys.setB(bVec);
		sys.setQ(qVec);
		sys.setD(dVec);
		sys.setM(mVec);
		sys.setG(gVec);
		sys.setX0(x0Vec);
		// */

		// Assign control params
		// /*
		sys.setURef(uRefVec);
		sys.setXRef(xRefVec);
		sys.setYRef(yRefVec);
		sys.setKc_x(Kc_xVec);
		sys.setKc_inty(Kc_intyVec);
		// */

		// input
		armaVec u_km1 = armaVec(u.colptr(k-1), u.n_rows, false, false);

		// Simulate the true system.
		sys_true.setU(u_km1);
		sys_true.simPredict();
		sys_true.h();

		// generate a measurement
		armaVec z_k = armaVec(z.colptr(k), z.n_rows, false, false);
		sys_true.simMeasurement(z_k);

		// update prev. prediction
		sys.fbCtrl(z_k, gateCtrl, gateLock, sigma_softStart, uSigma);

		lambdaRef.submat(0,k,nY-1,k) = armaMat(yRefVec.data(),nY,1);
		lambdaTrue.submat(0,k,nY-1,k) = sys_true.getY();
		xTrue.submat(0,k,nX-1,k) = sys_true.getX();
		gTrue.submat(0,k,nU-1,k) = sys_true.getG();
		mTrue.submat(0,k,nY-1,k) = sys_true.getM();

		lambdaHat.submat(0,k,nY-1,k) = sys.getY();
		xHat.submat(0,k,nX-1,k) = sys.getX();
		gHat.submat(0,k,nU-1,k) = sys.getG();
		mHat.submat(0,k,nY-1,k) = sys.getM();

		// write control to u
		armaVec u_k = armaVec(u.colptr(k), u.n_rows, false, false);
		u_k = sys.getU();
	}

	cout << "E[u] = " << mean(u, 1) << endl;

	auto finish = chrono::high_resolution_clock::now();
	chrono::duration<data_t, milli> simTime_ms = finish - start;
	cout << "Finished simulation in " << simTime_ms.count() << " ms.\n";
	cout << "(app. " << (simTime_ms.count()/K)*1e3 << " us/time-step)\n";

	cout << ".....................................\n";
	cout << "sys_hat (post-sim): \n";
	cout << ".....................................\n";
	sys.printSys();
	cout << ".....................................\n";

	// create armaMatlab variables and save to armaMat file for plotting elsewhere...
	mat_t *matfile;
	matvar_t *matvar;

	matfile = Mat_CreateVer("test_plds_ctrl.mat",NULL,MAT_FT_DEFAULT);
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

		// mHat
		dims[0] = mHat.n_rows; dims[1] = mHat.n_cols;
		matvar = Mat_VarCreate("mHat", MAT_C_DOUBLE, matDataType, 2, dims, mHat.memptr(), 0);
		Mat_VarWrite(matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// gHat
		dims[0] = gHat.n_rows; dims[1] = gHat.n_cols;
		matvar = Mat_VarCreate("gHat", MAT_C_DOUBLE, matDataType, 2, dims, gHat.memptr(), 0);
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

		// gTrue
		dims[0] = gTrue.n_rows; dims[1] = gTrue.n_cols;
		matvar = Mat_VarCreate("gTrue", MAT_C_DOUBLE, matDataType, 2, dims, gTrue.memptr(), 0);
		Mat_VarWrite(matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// mTrue
		dims[0] = gTrue.n_rows; dims[1] = gTrue.n_cols;
		matvar = Mat_VarCreate("mTrue", MAT_C_DOUBLE, matDataType, 2, dims, mTrue.memptr(), 0);
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

armaMat smooth(armaMat& x, data_t tau, data_t dt) {

	size_t K = x.n_cols;
	data_t a = exp(-dt/tau);
	data_t b = 1.0-a;

	armaMat y = armaMat(size(x), fill::zeros);
	y.col(K-1) = x.col(K-1);
	for (size_t k=(K-2); k==0; k--)
	{
		y.col(k) = a*y.col(k+1) + b*x.col(k);
	}

	for (size_t k=1; k<K; k++)
	{
		y.col(k) = a*y.col(k-1) + b*x.col(k);
	}

	return y;
}
