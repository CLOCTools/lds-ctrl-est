// #include <iostream>
#include <ldsCtrlEst>

using namespace std;
using namespace plds;

// for smoothing things..
auto smooth(armaMat& x, data_t tau, data_t dt) -> armaMat;

auto armaVec2stdVec(armaVec x) -> stdVec {
  return arma::conv_to<stdVec>::from(x);
};

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

  armaMat aArma_true = armaMat(nX, nX, fill::eye);
  aArma_true[0] = exp(-dt / 0.075);
  armaMat bArma_true = armaMat(nX, nU, fill::eye);
  bArma_true[0] = 5e-2;
  armaMat cArma_true = armaMat(nY, nX, fill::zeros);
  cArma_true[0] = 1;

  armaVec x0Arma_true = armaVec(nX, fill::zeros);
  x0Arma_true.fill(-7);

  data_t m = -7e-2;
  armaVec mArma_true = armaVec(nX, fill::zeros);
  mArma_true.fill(m);

  size_t K = (size_t)180.0 / dt;  // time steps for simulation.

  // true system...
  sys_t sys_true(nU, nX, nY, dt, p0, q0_true);

  // test the re-assignment method..
  sys_true = sys_t(nU, nX, nY, dt, p0, q0_true);

  // Assign params.
  sys_true.setA(aArma_true);
  sys_true.setB(bArma_true);
  sys_true.setC(cArma_true);
  sys_true.setX0(x0Arma_true);
  sys_true.setM(mArma_true);
  sys_true.reset();

  // augment sys_true to let params vary
  // sys_true.augment();

  // Fake measurements
  armaMat z(nY, K, fill::zeros);

  // Fake stimlus.
  armaMat u(nU, K, arma::fill::zeros);  // randu);
  // u.randu(); //For some reason, bus error.
  u.randn();  // need to do this first.
  u.randu(nU, K);

  data_t tau_u = 0.002;
  u = smooth(u, tau_u, dt);

  sys_t sys(nU, nX, nY, dt, p0, q0);

  // Create *incorrect* model.
  armaVec gSys = sys_true.getG();
  armaVec mSys = sys_true.getM() * 1.25;
  sys.setG(gSys);
  sys.setM(mSys);
  sys.reset();

  sys.adaptM = true;
  armaMat Q = sys.getQ();
  Q.diag().fill(1e-8);
  sys.setQ(Q);

  armaMat Q_m = sys.getQ_m();
  Q_m.diag().fill(1e-8);
  sys.setQ_m(Q_m);

  cout << ".....................................\n";
  cout << "sys_hat:\n";
  cout << ".....................................\n";
  sys.printSys();
  cout << ".....................................\n";

  // Create vectors
  stdVec aVec = armaVec2stdVec(vectorise(sys_true.getA()));
  stdVec bVec = armaVec2stdVec(vectorise(sys_true.getB()));
  stdVec cVec = armaVec2stdVec(vectorise(sys_true.getC()));
  stdVec qVec = armaVec2stdVec(vectorise(Q));
  stdVec x0Vec = armaVec2stdVec(sys_true.getX0());
  stdVec gVec = armaVec2stdVec(sys.getG());
  stdVec mVec = armaVec2stdVec(sys.getM());

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

  // create matrix to save outputs in...
  armaMat lambda_hat = armaMat(nY, K, fill::zeros);
  armaMat lambda_true = armaMat(nY, K, fill::zeros);

  // states and gain/bias params
  armaMat x_hat = armaMat(nX, K, fill::zeros);
  armaMat m_hat = armaMat(nX, K, fill::zeros);

  armaMat x_true = armaMat(nX, K, fill::zeros);
  armaMat m_true = armaMat(nY, K, fill::zeros);

  cout << "Starting " << K * dt << " sec simlation ... \n";
  auto start = chrono::high_resolution_clock::now();
  for (size_t k = 0; k < K; k++) {
    // Assign params (uncomment to simlate online conditions)
    sys.setDims(nU, nX, nY);
    sys.setA(aVec);
    sys.setB(bVec);
    sys.setQ(qVec);
    sys.setX0(x0Vec);
    sys.setC(cVec);
    sys.setM(mVec);
    sys.setG(gVec);
    //

    // input
    armaVec u_k = armaVec(u.colptr(k), u.n_rows, false, false);

    // Simlate the true system.
    sys_true.setU(u_k);
    sys_true.simPredict();

    // generate a measurement
    armaVec z_k = armaVec(z.colptr(k), z.n_rows, false, false);
    sys_true.simMeasurement(z_k);

    // filter (predict -> update)
    sys.filter(z_k);

    lambda_hat.submat(0, k, nY - 1, k) = sys.getY();
    lambda_true.submat(0, k, nY - 1, k) = sys_true.getY();

    x_true.submat(0, k, nX - 1, k) = sys_true.getX();
    m_true.submat(0, k, nY - 1, k) = sys_true.getM();

    x_hat.submat(0, k, nX - 1, k) = sys.getX();
    m_hat.submat(0, k, nY - 1, k) = sys.getM();

    // for next time.
    sys.setU(u_k);
  }

  auto finish = chrono::high_resolution_clock::now();
  chrono::duration<data_t, milli> simTime_ms = finish - start;
  cout << "Finished simlation in " << simTime_ms.count() << " ms.\n";
  cout << "(app. " << (simTime_ms.count() / K) * 1e3 << " us/time-step)\n";

  // saved variables: dt, lambdaHat, xHat, mHat, z, u, lambdaTrue,
  // xTrue, mTrue saving with hdf5 via armadillo
  arma::hdf5_opts::opts replace = arma::hdf5_opts::replace;

  auto dt_vec = arma::vec(1).fill(dt);
  dt_vec.save(arma::hdf5_name("test_plds_est.h5", "dt"));
  u.save(arma::hdf5_name("test_plds_est.h5", "u", replace));
  z.save(arma::hdf5_name("test_plds_est.h5", "z", replace));
  x_true.save(arma::hdf5_name("test_plds_est.h5", "xTrue", replace));
  m_true.save(arma::hdf5_name("test_plds_est.h5", "mTrue", replace));
  lambda_true.save(arma::hdf5_name("test_plds_est.h5", "lambdaTrue", replace));
  x_hat.save(arma::hdf5_name("test_plds_est.h5", "xHat", replace));
  m_hat.save(arma::hdf5_name("test_plds_est.h5", "mHat", replace));
  lambda_hat.save(arma::hdf5_name("test_plds_est.h5", "lambdaHat", replace));

  return 0;
}

auto smooth(armaMat& x, data_t tau, data_t dt) -> armaMat {
  size_t K = x.n_cols;
  data_t a = exp(-dt / tau);
  data_t b = 1.0 - a;

  armaMat y = armaMat(size(x), fill::zeros);
  y.col(K - 1) = x.col(K - 1);
  for (size_t k = (K - 2); k == 0; k--) {
    y.col(k) = a * y.col(k + 1) + b * x.col(k);
  }

  for (size_t k = 1; k < K; k++) {
    y.col(k) = a * y.col(k - 1) + b * x.col(k);
  }

  return y;
}
