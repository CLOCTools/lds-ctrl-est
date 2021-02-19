#include <matio.h>

#include <chrono>
#include <iostream>
#include <ldsCtrlEst>

#include "ldsCtrlEst_h/lds_gaussian.hpp"

using namespace std;
using namespace glds;

auto armaVec2stdVec(armaVec const& x) -> stdVec {
  return arma::conv_to<stdVec>::from(x);
};

auto main() -> int {
  cout << "********** Testing LDS C++ library. **********  \n\n";
  cout << "********** Gaussian **********  \n\n";

  bool do_adaptive_set_point = false;

  // Make SISO system sampled at 1kHz
  data_t dt = 1e-3;
  size_t n_u = 1;
  size_t n_x = 1;
  size_t n_y = 1;

  data_t q0_true = 1e-12;
  data_t q0 = 1e-8;
  data_t q_m = q0;
  data_t r0 = 1e-2;
  data_t p0 = 0;

  // Going to hard-code values from a real fit (pooled 9/16/16 + 9/20/16 rat
  // data)
  armaMat a_arma_true = armaMat(n_x, n_x, fill::eye);
  a_arma_true[0] = exp(-dt / 0.01);
  armaMat b_arma_true = armaMat(n_x, n_u, fill::eye);
  b_arma_true[0] = 4.25e-4;

  // simulate switching fr
  size_t fr_state = 0;
  data_t m_lo_true = log(5 * dt) * (1 - a_arma_true[0]);
  data_t pr_lo2hi = 1e-3;
  data_t m_hi_true = log(20 * dt) * (1 - a_arma_true[0]);
  data_t pr_hi2lo = pr_lo2hi;  // /2;

  stdVec m_vec_true = stdVec(n_y, m_lo_true);
  stdVec g_vec_true = stdVec(n_y, 9.3 / 0.03);
  size_t k_samples =
      static_cast<size_t>(30.0) / dt;  // time steps for simulation.

  // true system...
  plds::sys_t sys_true(n_u, n_x, n_y, dt, p0, q0_true);

  // Assign params.
  sys_true.setA(a_arma_true);
  sys_true.setB(b_arma_true);
  sys_true.setM(m_vec_true);
  sys_true.setG(g_vec_true);

  armaVec x0_true =
      inv(armaMat(n_x, n_x, fill::eye) - sys_true.getA()) * sys_true.getM();
  sys_true.setX0(x0_true);

  sys_true.reset();

  // control vars
  // Going to take the steady-state vals...
  stdVec kx_vec = stdVec(n_u * n_x, 25);
  stdVec kinty_vec =
      stdVec(n_u * n_y, 1e3 * static_cast<double>(!do_adaptive_set_point));
  stdVec x_ref_vec = stdVec(n_x, log(20.0 * dt));
  stdVec y_ref_vec = stdVec(n_y, 20.0 * dt);

  // the more uncorrelated noise, the less biased parameter estiarmaMates will
  // be...
  data_t u_sigma = 0e-3;

  // Fake measurements
  armaMat z(n_y, k_samples, fill::zeros);

  // Will later contain control.
  armaMat u(n_u, k_samples, fill::zeros);

  cout << ".....................................\n";
  cout << "sys_true:\n";
  cout << ".....................................\n";
  sys_true.printSys();
  cout << ".....................................\n";

  // Create *incorrect* model.
  armaVec g_sys = armaVec(n_u, fill::ones).fill(9.3);  // V -> mW/mm2
  armaVec g_design =
      armaVec(n_u, fill::ones)
          .fill(9.3);  // the input gain of the model used for controller design
  armaVec m_sys = armaVec(n_x, fill::zeros);
  armaMat b_sys = armaMat(n_x, n_u, fill::zeros).fill(4.25e-4);
  armaVec x0_sys = m_sys;
  armaVec d_sys = armaVec(n_y, fill::zeros).fill(5 * dt);

  data_t u_lb = 0.0;
  data_t u_ub = 5.0;
  glds::ctrl_t sys(n_u, n_x, n_y, u_lb, u_ub, dt, p0, q0, r0);

  // Assign params.
  sys.setA(a_arma_true);
  sys.setB(b_sys);
  sys.setM(m_sys);
  sys.setG(g_sys);
  sys.setGDesign(g_design);
  sys.setD(d_sys);
  sys.reset();

  size_t control_type = CONTROL_TYPE_INTY;
  if (do_adaptive_set_point) {
    control_type = control_type | CONTROL_TYPE_ADAPT_M;
  }
  sys.setControlType(control_type);

  sys.adaptM = true;
  armaMat q = sys.getQ();
  q.diag().fill(1e-10);
  sys.setQ(q);

  armaMat q_mat_m = sys.getQ_m();
  q_mat_m.diag().fill(1e-8);
  sys.setQ_m(q_mat_m);

  // Create vectors
  stdVec a_vec = armaVec2stdVec(vectorise(sys.getA()));
  stdVec b_vec = armaVec2stdVec(vectorise(sys.getB()));
  stdVec d_vec = armaVec2stdVec(vectorise(sys.getD()));
  stdVec q_vec = armaVec2stdVec(vectorise(q));

  stdVec x0_vec = armaVec2stdVec(x0_sys);
  stdVec g_vec = armaVec2stdVec(sys.getG());
  stdVec m_vec = armaVec2stdVec(sys.getM());

  // test assign params.
  sys.setDims(n_u, n_x, n_y);
  sys.setA(a_vec);
  sys.setB(b_vec);
  sys.setQ(q_vec);
  sys.setD(d_vec);
  sys.setM(m_vec);
  sys.setG(g_vec);
  sys.setX0(x0_vec);
  sys.reset();

  cout << ".....................................\n";
  cout << "sys_hat:\n";
  cout << ".....................................\n";
  sys.printSys();
  cout << ".....................................\n";

  // create armaMatrix to save outputs in...
  armaMat lambda_hat = armaMat(n_y, k_samples, fill::zeros);
  armaMat lambda_true = armaMat(n_y, k_samples, fill::zeros);
  armaMat lambda_ref = armaMat(n_y, k_samples, fill::ones) * y_ref_vec[0];

  // states and gain/disturbance params
  armaMat x_hat = armaMat(n_x, k_samples, fill::zeros);
  armaMat m_hat = armaMat(n_y, k_samples, fill::zeros);

  armaMat x_true = armaMat(n_x, k_samples, fill::zeros);
  armaMat m_true = armaMat(n_y, k_samples, fill::zeros);

  sys_true.reset();
  sys.reset();

  // set initial val
  lambda_hat.submat(0, 0, n_y - 1, 0) = sys.getY();
  lambda_true.submat(0, 0, n_y - 1, 0) = sys_true.getY();

  x_hat.submat(0, 0, n_x - 1, 0) = sys.getX();
  x_true.submat(0, 0, n_x - 1, 0) = sys_true.getX();

  m_hat.submat(0, 0, n_x - 1, 0) = sys.getM();
  m_true.submat(0, 0, n_x - 1, 0) = sys_true.getM();

  bool gate_ctrl = true;
  bool gate_lock = false;

  // test constructor pass by ref:
  // make uLB something else...
  u_lb = 0.01;

  // always keep estimator enabled...
  bool gate_est = true;

  // tau antiWindup
  data_t tau_anti_windup = 1e6;
  sys.setTauAntiWindup(tau_anti_windup);

  // data_t sigma_softStart = 0.1;

  cout << "Starting " << k_samples * dt << " sec simulation ... \n";
  auto start = chrono::high_resolution_clock::now();
  for (size_t k = 1; k < k_samples; k++) {
    armaVec chance = arma::randu<arma::vec>(1);
    if (fr_state == 0)  // low state
    {
      if (chance[0] < pr_lo2hi) {
        m_vec_true = stdVec(n_y, m_hi_true);
        fr_state = 1;
        // cout << "Switching low -> high FR...\n";
      }
    } else {  // high state
      if (chance[0] < pr_hi2lo) {
        m_vec_true = stdVec(n_y, m_lo_true);
        fr_state = 0;
        // cout << "Switching high -> low FR...\n";
      }
    }
    sys_true.setM(m_vec_true);

    // Assign params (uncomment to simulate online conditions)
    // /*
    sys.setDims(n_u, n_x, n_y);
    sys.setA(a_vec);
    sys.setB(b_vec);
    sys.setQ(q_vec);
    sys.setD(d_vec);
    sys.setM(m_vec);
    sys.setG(g_vec);
    sys.setX0(x0_vec);
    // */

    // Assign control params
    // /*
    sys.setXRef(x_ref_vec);
    sys.setYRef(y_ref_vec);
    sys.setKc_x(kx_vec);
    sys.setKc_inty(kinty_vec);
    // */

    // input
    armaVec u_km1 = armaVec(u.colptr(k - 1), u.n_rows, false, false);

    // Simulate the true system.
    sys_true.setU(u_km1);
    sys_true.simPredict();
    armaVec z_k = armaVec(z.colptr(k), z.n_rows, false, false);
    sys_true.simMeasurement(z_k);

    // sys.steadyState_fbCtrl(z_k, gateCtrl, gateEst, gateLock, sigma_softStart,
    // uSigma);
    sys.steadyState_fbCtrl(z_k, gate_ctrl, gate_est);

    lambda_ref.submat(0, k, n_y - 1, k) = armaMat(y_ref_vec.data(), n_y, 1);
    lambda_true.submat(0, k, n_y - 1, k) = sys_true.getY();
    x_true.submat(0, k, n_x - 1, k) = sys_true.getX();
    m_true.submat(0, k, n_y - 1, k) = sys_true.getM();

    lambda_hat.submat(0, k, n_y - 1, k) = sys.getY();
    x_hat.submat(0, k, n_x - 1, k) = sys.getX();
    m_hat.submat(0, k, n_y - 1, k) = sys.getM();

    // write control to u
    armaVec u_k = armaVec(u.colptr(k), u.n_rows, false, false);
    u_k = sys.getU();
  }

  auto finish = chrono::high_resolution_clock::now();
  chrono::duration<data_t, milli> sim_time_ms = finish - start;
  cout << "Finished simulation in " << sim_time_ms.count() << " ms.\n";
  cout << "(app. " << (sim_time_ms.count() / k_samples) * 1e3
       << " us/time-step)\n";

  cout << ".....................................\n";
  cout << "sys_hat (post-sim): \n";
  cout << ".....................................\n";
  sys.printSys();
  cout << ".....................................\n";

  // saved variables: dt, lambdaHat, xHat, mHat, z, u, lambdaRef, lambdaTrue,
  // xTrue, mTrue saving with hdf5 via armadillo
  arma::hdf5_opts::opts replace = arma::hdf5_opts::replace;

  auto dt_vec = arma::vec(1).fill(dt);
  dt_vec.save(arma::hdf5_name("test_glds_ctrl.h5", "dt"));
  lambda_ref.save(arma::hdf5_name("test_glds_ctrl.h5", "lambdaRef", replace));
  u.save(arma::hdf5_name("test_glds_ctrl.h5", "u", replace));
  z.save(arma::hdf5_name("test_glds_ctrl.h5", "z", replace));
  x_true.save(arma::hdf5_name("test_glds_ctrl.h5", "xTrue", replace));
  m_true.save(arma::hdf5_name("test_glds_ctrl.h5", "mTrue", replace));
  lambda_true.save(arma::hdf5_name("test_glds_ctrl.h5", "lambdaTrue", replace));
  x_hat.save(arma::hdf5_name("test_glds_ctrl.h5", "xHat", replace));
  m_hat.save(arma::hdf5_name("test_glds_ctrl.h5", "mHat", replace));
  lambda_hat.save(arma::hdf5_name("test_glds_ctrl.h5", "lambdaHat", replace));

  return 0;
}
