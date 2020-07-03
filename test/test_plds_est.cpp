// #include <iostream>
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
	data_t q0_true = 1e-10;
	data_t q0 = 1e-10;
	data_t p0 = 1e-9;

	armaMat aArma_true = armaMat(nX,nX,fill::eye);
	aArma_true[0] = exp(-dt/0.075);
	armaMat bArma_true = armaMat(nX,nU,fill::eye);
	bArma_true[0] = 5e-2; //2.7e-2;
	armaMat cArma_true = armaMat(nY,nX,fill::zeros);
	cArma_true[0] = 1;

	data_t m = -7e-2;//-7.67e-2;
	armaVec mArma_true = armaVec(nX, fill::zeros);
	mArma_true.fill(m);

	size_t K = (size_t) 180.0/dt; //time steps for simulation.

	// true system...
	sys_t sys_true(nU, nX, nY, dt, p0, q0_true);

	//test the re-assignment method..
	sys_true = sys_t(nU, nX, nY, dt, p0, q0_true);

	// Assign params.
	sys_true.setA(aArma_true);
	sys_true.setB(bArma_true);
	sys_true.setC(cArma_true);
	sys_true.setM(mArma_true);
	sys_true.h();
	sys_true.reset();

	// augment sys_true to let params vary
	// sys_true.augment();

	// Fake measurements
	armaMat z(nY, K, fill::zeros);

	// Fake stimlus.
	armaMat u(nU, K, arma::fill::zeros);//randu);
	// u.randu(); //For some reason, bus error.
	u.randn(); //need to do this first.
	u.randu(nU,K);

	data_t tau_u = 0.002;
	u = smooth(u, tau_u, dt);

	sys_t sys(nU, nX, nY, dt, p0, q0);

	// Create *incorrect* model.
	armaVec gSys = sys_true.getG() * 2;
	armaVec mSys = sys_true.getM() / 2;
	sys.setG(gSys);
	sys.setM(mSys);

	sys.augment(AUGMENT_M | AUGMENT_G);
	armaMat Q = sys.getQ();
	Q.diag()[nX]=1e-7;
	Q.diag()[nX+nX]=1e-4;
	sys.setQ(Q);

	cout << ".....................................\n";
	cout << "sys_hat:\n";
	cout << ".....................................\n";
	sys.printSys();
	cout << ".....................................\n";

	// Create vectors
	stdVec aVec = armaVec2stdVec( vectorise(sys_true.getA()) );
	stdVec bVec = armaVec2stdVec( vectorise(sys_true.getB()) );
	stdVec cVec = armaVec2stdVec( vectorise(sys_true.getC()) );
	stdVec qVec = armaVec2stdVec( vectorise(Q) );
	stdVec x0Vec = armaVec2stdVec( sys_true.getX0() );
	stdVec gVec = armaVec2stdVec( sys.getG() );
	stdVec mVec = armaVec2stdVec( sys.getM() );

	// assign params.
	sys.setDims(nU, nX, nY);
	sys.setA(aVec);
	sys.setB(bVec);
	sys.setQ(qVec);
	sys.setX0(x0Vec);
	sys.setC(cVec);
	sys.setM(mVec);
	sys.setG(gVec);
	sys.reset();

	cout << ".....................................\n";
	cout << "sys_hat:\n";
	cout << ".....................................\n";
	sys.printSys();
	cout << ".....................................\n";

	bool isAugmented = sys.checkIfAugmented();
	cout << "Is sys_hat augmented? " << isAugmented << "\n";

	// create matrix to save outputs in...
	armaMat lambdaHat = armaMat(nY, K, fill::zeros);
	armaMat lambdaTrue = armaMat(nY, K, fill::zeros);

	// states and gain/bias params
	armaMat xHat = armaMat(nX, K, fill::zeros);
	armaMat mHat = armaMat(nX, K, fill::zeros);
	armaMat gHat = armaMat(nU, K, fill::zeros);

	armaMat xTrue = armaMat(nX, K, fill::zeros);
	armaMat mTrue = armaMat(nY, K, fill::zeros);
	armaMat gTrue = armaMat(nU, K, fill::zeros);

	cout << "Starting " << K*dt << " sec simlation ... \n";
	auto start = chrono::high_resolution_clock::now();
	for (size_t k=0; k<K; k++)
	{
		// Assign params (uncomment to simlate online conditions)
		// /*
		sys.setDims(nU, nX, nY);
		sys.setA(aVec);
		sys.setB(bVec);
		sys.setQ(qVec);
		sys.setX0(x0Vec);
		sys.setC(cVec);
		sys.setM(mVec);
		sys.setG(gVec);
		// */

		// input
		armaVec u_k = armaVec(u.colptr(k), u.n_rows, false, false);

		// Simlate the true system.
		sys_true.setU(u_k);
		sys_true.simPredict();
		sys_true.h();

		// generate a measurement
		armaVec z_k = armaVec(z.colptr(k), z.n_rows, false, false);
		sys_true.simMeasurement(z_k);

		// update prev. prediction
		sys.update(z_k);
		// cout << "sys.y[0]_" << k << " = " << sys.y[0] << "\n";

		lambdaHat.submat(0,k,nY-1,k) = sys.getY();
		lambdaTrue.submat(0,k,nY-1,k) = sys_true.getY();

		xTrue.submat(0,k,nX-1,k) = sys_true.getX();
		gTrue.submat(0,k,nU-1,k) = sys_true.getG();
		mTrue.submat(0,k,nY-1,k) = sys_true.getM();

		xHat.submat(0,k,nX-1,k) = sys.getX();
		gHat.submat(0,k,nU-1,k) = sys.getG();
		mHat.submat(0,k,nY-1,k) = sys.getM();

		// Predict
		sys.setU(u_k);
		sys.predict();
		// sys.h();
	}

	auto finish = chrono::high_resolution_clock::now();
	chrono::duration<data_t, milli> simTime_ms = finish - start;
	cout << "Finished simlation in " << simTime_ms.count() << " ms.\n";
	cout << "(app. " << (simTime_ms.count()/K)*1e3 << " us/time-step)\n";

	// cout << ".....................................\n";
	// cout << "sys_true (post-sim):\n";
	// cout << ".....................................\n";
	// sys_true.printSys();
	// cout << ".....................................\n";

	// cout << ".....................................\n";
	// cout << "sys_hat (post-sim): \n";
	// cout << ".....................................\n";
	// sys.printSys();
	// cout << ".....................................\n";

	// create matlab variables and save to armaMat file for plotting elsewhere...
	mat_t *matfile;
	matvar_t *matvar;

	matfile = Mat_CreateVer("test_plds_est.mat",NULL,MAT_FT_DEFAULT);
	matio_types matDataType = MAT_T_DOUBLE;
	matio_compression matCompression = MAT_COMPRESSION_NONE;

	if(matfile)
	{
		cout << "Saving measurements and true vs. estimated outputs... \n";
		// matvar_t  *Mat_VarCreate(const char *name,enum matio_classes class_type, enum matio_types data_type,int rank,size_t *dims, void *data,int opt);

		// lambdaHat
		size_t dims[2] = {lambdaHat.n_rows, lambdaHat.n_cols};
		matvar = Mat_VarCreate("lambdaHat", MAT_C_DOUBLE, matDataType, 2, dims, lambdaHat.memptr(), 0);
		Mat_VarWrite( matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// xHat
		dims[0] = xHat.n_rows; dims[1] = xHat.n_cols;
		matvar = Mat_VarCreate("xHat", MAT_C_DOUBLE, matDataType, 2, dims, xHat.memptr(), 0);
		Mat_VarWrite( matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// mHat
		dims[0] = mHat.n_rows; dims[1] = mHat.n_cols;
		matvar = Mat_VarCreate("mHat", MAT_C_DOUBLE, matDataType, 2, dims, mHat.memptr(), 0);
		Mat_VarWrite( matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// gHat
		dims[0] = gHat.n_rows; dims[1] = gHat.n_cols;
		matvar = Mat_VarCreate("gHat", MAT_C_DOUBLE, matDataType, 2, dims, gHat.memptr(), 0);
		Mat_VarWrite( matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// simlated measurements.
		dims[0] = z.n_rows; dims[1] = z.n_cols;
		matvar = Mat_VarCreate("z", MAT_C_DOUBLE, matDataType, 2, dims, z.memptr(), 0);
		Mat_VarWrite( matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// inputs.
		dims[0] = u.n_rows; dims[1] = u.n_cols;
		matvar = Mat_VarCreate("u", MAT_C_DOUBLE, matDataType, 2, dims, u.memptr(), 0);
		Mat_VarWrite(matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// true rate.
		dims[0] = lambdaTrue.n_rows; dims[1] = lambdaTrue.n_cols;
		matvar = Mat_VarCreate("lambdaTrue", MAT_C_DOUBLE, matDataType, 2, dims, lambdaTrue.memptr(), 0);
		Mat_VarWrite( matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// xTrue
		dims[0] = xTrue.n_rows; dims[1] = xTrue.n_cols;
		matvar = Mat_VarCreate("xTrue", MAT_C_DOUBLE, matDataType, 2, dims, xTrue.memptr(), 0);
		Mat_VarWrite( matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// gTrue
		dims[0] = gTrue.n_rows; dims[1] = gTrue.n_cols;
		matvar = Mat_VarCreate("gTrue", MAT_C_DOUBLE, matDataType, 2, dims, gTrue.memptr(), 0);
		Mat_VarWrite( matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// mTrue
		dims[0] = gTrue.n_rows; dims[1] = gTrue.n_cols;
		matvar = Mat_VarCreate("mTrue", MAT_C_DOUBLE, matDataType, 2, dims, mTrue.memptr(), 0);
		Mat_VarWrite( matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		// dt.
		data_t* dt_ptr = &dt;
		dims[0] = 1; dims[1] = 1;
		matvar = Mat_VarCreate("dt", MAT_C_DOUBLE, matDataType, 2, dims, dt_ptr, 0);
		Mat_VarWrite( matfile, matvar, matCompression);
		Mat_VarFree(matvar);

		Mat_Close(matfile);
		cout << "Finished saving. \n";
	}
	else
	cout << "Dammit. Failed to open file... \n";

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
