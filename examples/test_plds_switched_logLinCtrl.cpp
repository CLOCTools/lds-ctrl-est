#include <matio.h>

#include <chrono>
#include <iostream>
#include <ldsCtrlEst>

using namespace std;
using namespace plds;

auto main(void) -> int {
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

  armaMat A_true(nX, nX, fill::eye);
  A_true[0] = 0.98;
  armaMat B_true2 = armaMat(nX, nU).fill(0.03);
  armaMat B_true1 = armaMat(nX, nU).fill(scale_sysB * B_true2[0]);
  armaMat C_true(nY, nX, fill::eye);
  armaVec d_true = armaVec(nY, fill::zeros).fill(log(1 * dt));

  size_t T = (size_t)30.0 / dt;  // time steps for simulation.

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
  armaMat Kc_x2 = scale_sysB * Kc_x1;  // system2 is x-times less sensitive.
  armaVec yRef = armaVec(nY).fill(20.0 * dt);

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
  sys.setSystem(0, sys1);
  sys.setSystem(1, sys2);
  sys.switchSystem(0);
  sys.reset();

  // Fake measurements
  armaMat z(nY, T, fill::zeros);

  // Will later contain control.
  armaMat u(nU, T, fill::zeros);

  // create armaMatrix to save outputs in...
  armaMat lambda_hat = armaMat(nY, T, fill::zeros);
  armaMat lambda_true = armaMat(nY, T, fill::zeros);
  armaMat lambda_ref = armaMat(nY, T, fill::ones) * yRef[0];

  // states and gain/disturbance params
  armaMat x_hat = armaMat(nX, T, fill::zeros);
  armaMat x_true = armaMat(nX, T, fill::zeros);
  armaMat state = armaMat(1, T, fill::zeros);

  // set initial val
  lambda_hat.submat(0, 0, nY - 1, 0) = sys1.getY();
  lambda_true.submat(0, 0, nY - 1, 0) = sys_true.getY();
  x_hat.submat(0, 0, nX - 1, 0) = sys1.getX();
  x_true.submat(0, 0, nX - 1, 0) = sys_true.getX();

  // some params.
  uLB = 0.01;
  bool gateEst = true;
  bool gateCtrl = true;

  cout << "Starting " << T * dt << " sec simulation ... \n";
  auto start = chrono::high_resolution_clock::now();
  for (size_t t = 1; t < T; t++) {
    armaVec chance(1, fill::randu);
    if (gainState == 0)  // state1
    {
      if (chance[0] < pr21) {
        gainState = 1;
        sys_true.setB(B_true2);
        if (doSwitchCtrl) sys.switchSystem(1);
      }
    } else {  // state2
      if (chance[0] < pr12) {
        gainState = 0;
        sys_true.setB(B_true1);
        if (doSwitchCtrl) sys.switchSystem(0);
      }
    }

    // input
    armaVec u_tm1 = armaVec(u.colptr(t - 1), u.n_rows, false, false);

    // Simulate the true system.
    sys_true.setU(u_tm1);
    sys_true.simPredict();
    armaVec z_k = armaVec(z.colptr(t), z.n_rows, false, false);
    sys_true.simMeasurement(z_k);

    // perform control
    sys.steadyState_logLin_fbCtrl(z_k, gateCtrl, gateEst);

    state.submat(0, t, 0, t) = gainState + 1;
    lambda_ref.submat(0, t, nY - 1, t) = yRef;
    lambda_true.submat(0, t, nY - 1, t) = sys_true.getY();
    x_true.submat(0, t, nX - 1, t) = sys_true.getX();
    lambda_hat.submat(0, t, nY - 1, t) = sys.getY();
    x_hat.submat(0, t, nX - 1, t) = sys.getX();

    // write control to u
    armaVec u_t = armaVec(u.colptr(t), u.n_rows, false, false);
    u_t = sys.getU();
  }

  auto finish = chrono::high_resolution_clock::now();
  chrono::duration<data_t, milli> simTime_ms = finish - start;
  cout << "Finished simulation in " << simTime_ms.count() << " ms.\n";
  cout << "(app. " << (simTime_ms.count() / T) * 1e3 << " us/time-step)\n";

  // saved variables: dt, lambdaHat, xHat, mHat, z, u, lambdaRef, lambdaTrue,
  // xTrue, mTrue saving with hdf5 via armadillo
  arma::hdf5_opts::opts replace = arma::hdf5_opts::replace;

  auto dt_vec = arma::vec(1).fill(dt);
  dt_vec.save(arma::hdf5_name("test_plds_switched_ctrl.h5", "dt"));
  lambda_ref.save(
      arma::hdf5_name("test_plds_switched_ctrl.h5", "lambdaRef", replace));
  u.save(arma::hdf5_name("test_plds_switched_ctrl.h5", "u", replace));
  z.save(arma::hdf5_name("test_plds_switched_ctrl.h5", "z", replace));
  x_true.save(arma::hdf5_name("test_plds_switched_ctrl.h5", "xTrue", replace));
  lambda_true.save(
      arma::hdf5_name("test_plds_switched_ctrl.h5", "lambdaTrue", replace));
  x_hat.save(arma::hdf5_name("test_plds_switched_ctrl.h5", "xHat", replace));
  lambda_hat.save(
      arma::hdf5_name("test_plds_switched_ctrl.h5", "lambdaHat", replace));
  state.save(arma::hdf5_name("test_plds_switched_ctrl.h5", "state", replace));

  return 0;
}
