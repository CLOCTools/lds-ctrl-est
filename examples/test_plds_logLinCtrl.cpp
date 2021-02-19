#include <matio.h>

#include <chrono>
#include <iostream>
#include <ldsCtrlEst>

using namespace std;
using namespace plds;

auto armaVec2stdVec(armaVec x) -> stdVec {
  return arma::conv_to<stdVec>::from(x);
};

auto main(void) -> int {
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

  // Going to hard-code values from a real fit (pooled 9/16/16 + 9/20/16 rat
  // data)
  armaMat aArma_true = armaMat(nX, nX, fill::eye);
  aArma_true[0] = 0.986;  // exp(-dt/0.075);
  armaMat bArma_true = armaMat(nX, nU, fill::eye);
  bArma_true[0] = 0.027 * 2;

  // simulate switching fr
  size_t frState = 0;
  data_t mLo_true = log(1 * dt) * (1 - aArma_true[0]);
  data_t prLo2Hi = 1e-3;
  data_t mHi_true = log(20 * dt) * (1 - aArma_true[0]);
  data_t prHi2Lo = prLo2Hi / 2;

  stdVec mVec_true = stdVec(nY, mLo_true);
  stdVec gVec_true = stdVec(nY, 1.0);
  size_t K = (size_t)30.0 / dt;  // time steps for simulation.

  // true system...
  sys_t sys_true(nU, nX, nY, dt, p0, q0_true);

  // test the re-assignment method..
  sys_true = sys_t(nU, nX, nY, dt, p0, q0_true);

  // Assign params.
  sys_true.setA(aArma_true);
  sys_true.setB(bArma_true);
  sys_true.setM(mVec_true);
  sys_true.setG(gVec_true);

  // armaVec x0_true = inv(armaMat(nX,nX,fill::eye) -
  // sys_true.getA())*sys_true.getM();
  armaVec x0_true(nX);
  x0_true.fill(log(20 * dt));
  sys_true.setX0(x0_true);

  sys_true.reset();

  // control vars
  // Going to take the steady-state vals...
  stdVec Kc_xVec = stdVec(nU * nX, 1);
  stdVec Kc_intyVec = stdVec(nU * nY, 25);
  stdVec xRefVec = stdVec(nX, log(20.0 * dt));
  stdVec yRefVec = stdVec(nY, 20.0 * dt);

  // the more uncorrelated noise, the less biased parameter estiarmaMates will
  // be...
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
  armaVec gSys = armaVec(nU, fill::zeros).fill(gVec_true[0]);
  armaVec mSys = armaVec(nY, fill::zeros).fill(mLo_true);

  data_t uLB = 0.0;
  data_t uUB = 5.0;
  ctrl_t sys(nU, nX, nY, uLB, uUB, dt, p0, q0);
  // go ahead and set it to the wrong vals...
  sys.setG(gSys);
  sys.setM(mSys);

  size_t controlType = CONTROL_TYPE_INTY;
  sys.setControlType(controlType);

  sys.adaptM = true;
  armaMat Q = sys.getQ();
  Q.diag().fill(1e-8);
  sys.setQ(Q);

  armaMat Q_m = sys.getQ_m();
  Q_m.diag().fill(1e-6);
  sys.setQ_m(Q_m);

  // Create vectors
  stdVec aVec = armaVec2stdVec(vectorise(sys_true.getA()));
  stdVec bVec = armaVec2stdVec(vectorise(sys_true.getB()));
  stdVec dVec = armaVec2stdVec(vectorise(sys_true.getD()));
  stdVec qVec = armaVec2stdVec(vectorise(Q));
  stdVec x0Vec = armaVec2stdVec(x0_true);
  stdVec gVec = armaVec2stdVec(sys.getG());
  stdVec mVec = armaVec2stdVec(sys.getM());

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

  // create armaMatrix to save outputs in...
  armaMat lambda_hat = armaMat(nY, K, fill::zeros);
  armaMat lambda_true = armaMat(nY, K, fill::zeros);
  armaMat lambda_ref = armaMat(nY, K, fill::ones) * yRefVec[0];

  // states and gain/disturbance params
  armaMat x_hat = armaMat(nX, K, fill::zeros);
  armaMat m_hat = armaMat(nY, K, fill::zeros);

  armaMat x_true = armaMat(nX, K, fill::zeros);
  armaMat m_true = armaMat(nY, K, fill::zeros);

  sys_true.reset();
  sys.reset();

  // set initial val
  lambda_hat.submat(0, 0, nY - 1, 0) = sys.getY();
  lambda_true.submat(0, 0, nY - 1, 0) = sys_true.getY();

  x_hat.submat(0, 0, nX - 1, 0) = sys.getX();
  x_true.submat(0, 0, nX - 1, 0) = sys_true.getX();

  m_hat.submat(0, 0, nX - 1, 0) = sys.getM();
  m_true.submat(0, 0, nX - 1, 0) = sys_true.getM();

  bool gateCtrl = true;
  bool gateLock = false;

  // test constructor pass by ref:
  // make uLB something else...
  uLB = 0.01;

  // always keep estimator enabled...
  bool gateEst = true;

  // tau antiWindup
  data_t tauAntiWindup = 1e6;
  sys.setTauAntiWindup(tauAntiWindup);

  data_t sigma_softStart = 0.1;

  cout << "Starting " << K * dt << " sec simulation ... \n";
  auto start = chrono::high_resolution_clock::now();
  for (size_t k = 1; k < K; k++) {
    armaVec chance(1, fill::randu);
    bool doSwitch = false;
    if (frState == 0)  // low state
    {
      if (chance[0] < prLo2Hi) {
        mVec_true = stdVec(nY, mHi_true);
        frState = 1;
        // cout << "Switching low -> high FR...\n";
      }
    } else {  // high state
      if (chance[0] < prHi2Lo) {
        mVec_true = stdVec(nY, mLo_true);
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
    sys.setXRef(xRefVec);
    sys.setYRef(yRefVec);
    sys.setKc_x(Kc_xVec);
    sys.setKc_inty(Kc_intyVec);
    // */

    // input
    armaVec u_km1 = armaVec(u.colptr(k - 1), u.n_rows, false, false);

    // Simulate the true system.
    sys_true.setU(u_km1);
    sys_true.simPredict();
    armaVec z_k = armaVec(z.colptr(k), z.n_rows, false, false);
    sys_true.simMeasurement(z_k);

    // update prev. prediction
    sys.steadyState_logLin_fbCtrl(z_k, gateCtrl, gateEst, gateLock,
                                  sigma_softStart, uSigma);

    lambda_ref.submat(0, k, nY - 1, k) = armaMat(yRefVec.data(), nY, 1);
    lambda_true.submat(0, k, nY - 1, k) = sys_true.getY();
    x_true.submat(0, k, nX - 1, k) = sys_true.getX();
    m_true.submat(0, k, nY - 1, k) = sys_true.getM();

    lambda_hat.submat(0, k, nY - 1, k) = sys.getY();
    x_hat.submat(0, k, nX - 1, k) = sys.getX();
    m_hat.submat(0, k, nY - 1, k) = sys.getM();

    // write control to u
    armaVec u_k = armaVec(u.colptr(k), u.n_rows, false, false);
    u_k = sys.getU();
  }

  auto finish = chrono::high_resolution_clock::now();
  chrono::duration<data_t, milli> simTime_ms = finish - start;
  cout << "Finished simulation in " << simTime_ms.count() << " ms.\n";
  cout << "(app. " << (simTime_ms.count() / K) * 1e3 << " us/time-step)\n";

  cout << ".....................................\n";
  cout << "sys_hat (post-sim): \n";
  cout << ".....................................\n";
  sys.printSys();
  cout << ".....................................\n";

  // saved variables: dt, lambdaHat, xHat, mHat, z, u, lambdaRef, lambdaTrue,
  // xTrue, mTrue saving with hdf5 via armadillo
  arma::hdf5_opts::opts replace = arma::hdf5_opts::replace;

  auto dt_vec = arma::vec(1).fill(dt);
  dt_vec.save(arma::hdf5_name("test_plds_ctrl.h5", "dt"));
  lambda_ref.save(arma::hdf5_name("test_plds_ctrl.h5", "lambdaRef", replace));
  u.save(arma::hdf5_name("test_plds_ctrl.h5", "u", replace));
  z.save(arma::hdf5_name("test_plds_ctrl.h5", "z", replace));
  x_true.save(arma::hdf5_name("test_plds_ctrl.h5", "xTrue", replace));
  m_true.save(arma::hdf5_name("test_plds_ctrl.h5", "mTrue", replace));
  lambda_true.save(arma::hdf5_name("test_plds_ctrl.h5", "lambdaTrue", replace));
  x_hat.save(arma::hdf5_name("test_plds_ctrl.h5", "xHat", replace));
  m_hat.save(arma::hdf5_name("test_plds_ctrl.h5", "mHat", replace));
  lambda_hat.save(arma::hdf5_name("test_plds_ctrl.h5", "lambdaHat", replace));

  return 0;
}
