//===-- eg_glds_ctrl.cpp - Example GLDS Control ---------------------------===//
//
// Copyright 2021 [name of copyright owner]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//===----------------------------------------------------------------------===//
/// \brief Example GLDS Control
///
/// \example eg_glds_ctrl.cpp
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>

using lds::armaMat;
using lds::armaVec;
using lds::data_t;
using std::cout;
using std::vector;

auto main() -> int {
  cout << " ********** Example Gaussian LDS Control ********** \n\n";

  // Make 1st-order SISO system, sampled at 1kHz
  data_t dt = 1e-3;
  size_t n_u = 1;
  size_t n_x = 1;
  size_t n_y = 1;

  // no time steps for simulation.
  size_t n_t = static_cast<size_t>(5.0 / dt);

  // construct ground truth system to be controlled...
  // initializes to random walk model with top-most n_y state observed
  lds::gaussian::sys_t controlled_system(n_u, n_x, n_y, dt);

  // Ground-truth parameters for the controlled system
  // (stand-in for physical system to be controlled)
  armaMat a_true(n_x, n_x, arma::fill::eye);
  a_true[0] = exp(-dt / 0.01);
  armaMat b_true = armaMat(n_x, n_u).fill(2e-4);
  // control signal to model input unit conversion e.g., V -> mW/mm2:
  armaVec g_true = armaVec(n_y).fill(10.0);

  // output noise covariance
  armaMat r_true = armaMat(n_y, n_y, arma::fill::eye) * 1e-4;

  /// Going to simulate a switching disturbance (m) acting on system
  size_t which_m = 0;  // whether low or high disturbance (0, 1)
  data_t m_low = 5 * dt * (1 - a_true[0]);
  data_t pr_lo2hi = 1e-3;  // probability of going from low to high disturb.
  data_t m_high = 20 * dt * (1 - a_true[0]);
  data_t pr_hi2lo = pr_lo2hi;

  // initially let m be low
  armaVec m0_true = armaVec(n_y).fill(m_low);

  // Assign params.
  controlled_system.setA(a_true);
  controlled_system.setB(b_true);
  controlled_system.setM(m0_true);
  controlled_system.setG(g_true);
  controlled_system.setR(r_true);

  cout << ".....................................\n";
  cout << "controlled_system:\n";
  cout << ".....................................\n";
  controlled_system.printSys();
  cout << ".....................................\n";

  // create controller
  // lower and upper bounds on control signal (e.g., in Volts)
  data_t u_lb = 0.0;  // [=] V
  data_t u_ub = 5.0;  // [=] V
  glds::ctrl_t controller(n_u, n_x, n_y, u_lb, u_ub, dt);

  // Create **incorrect** model used for control.
  // (e.g., imperfect model fitting)
  armaMat b_controller = armaMat(n_x, n_u).fill(1e-4);

  // Let's say these controller gains were designed assuming g was 9 V/(mW/mm2):
  armaVec g_design = armaVec(n_u).fill(9);

  // let's assume zero process disturbance initially
  // (will be re-estimating)
  armaVec m_controller = armaVec(n_x, arma::fill::zeros);

  // Control variables:

  // if following enabled, adapts set point with re-estimated process
  // disturbance n.b., should not need integral action if this is enabled as the
  // adaptive estimator minimizes DC error
  bool do_adaptive_set_point = false;

  // Reference/target output, controller gains
  // n.b., Can either use armaVec (arma::Col) or std::vector
  armaVec y_ref0 = armaVec(n_y).fill(20.0 * dt);
  // (Can either use arma::Mat or mats flattened into std::vector)
  armaMat k_x = armaMat(n_u, n_x).fill(100);     // gains on state error
  armaMat k_inty = armaMat(n_u, n_y).fill(1e3);  // gains on integrated err

  // setting initial state to target to avoid error at onset:
  armaVec x0 = armaVec(n_x).fill(y_ref0[0]);

  // set up controller type bit mask so controller knows how to proceed
  size_t control_type = 0;
  if (do_adaptive_set_point) {
    // adapt set point with estimated disturbance
    control_type = control_type | lds::CONTROL_TYPE_ADAPT_M;
  } else {
    // use integral action to minimize DC error
    control_type = control_type | lds::CONTROL_TYPE_INTY;
  }

  // Start configuring controller:
  // Adaptively re-estimate process disturance (m)
  controller.adaptM = true;

  // set controller type
  controller.setControlType(control_type);

  // Set params.
  // **n.b. using arbitrary defaults for Q, R in this example. Really, these
  // should be set by users, as they tune characteristics of Kalman filter.
  // Users can also choose not to recursively calculate the estimator gain and
  // supply it (setKe) instead of covariances.**
  controller.setYRef(y_ref0);
  controller.setKc_x(k_x);
  controller.setKc_inty(k_inty);
  controller.setA(a_true);
  controller.setB(b_controller);
  controller.setM(m_controller);
  controller.setG(g_true);
  controller.setGDesign(g_design);
  controller.setX0(x0);
  controller.reset();  // reset to new initial condition

  // set adaptation rate by changing covariance of assumed process noise acting
  // on random-walk evolution of m
  armaMat q_m = armaMat(n_x, n_x, arma::fill::eye) * 1e-6;
  controller.setQ_m(q_m);

  cout << ".....................................\n";
  cout << "control system:\n";
  cout << ".....................................\n";
  controller.printSys();
  cout << ".....................................\n";

  // set up variables for simulation
  // create armaMatrix to save outputs in...
  armaMat y_ref = armaMat(n_y, n_t, arma::fill::ones) * y_ref0[0];

  // Simulated measurements
  armaMat z(n_y, n_t, arma::fill::zeros);

  // simulated control signal ([=] V)
  armaMat u(n_u, n_t, arma::fill::zeros);

  // outputs, states and gain/disturbance params
  // *_hat indicates online estimates
  armaMat y_hat(n_y, n_t, arma::fill::zeros);
  armaMat x_hat(n_x, n_t, arma::fill::zeros);
  armaMat m_hat(n_y, n_t, arma::fill::zeros);

  // *_true indicates ground truth (system being controlled)
  armaMat y_true(n_y, n_t, arma::fill::zeros);
  armaMat x_true(n_x, n_t, arma::fill::zeros);
  armaMat m_true(n_y, n_t, arma::fill::zeros);

  // get initial val
  y_hat.submat(0, 0, n_y - 1, 0) = controller.getY();
  y_true.submat(0, 0, n_y - 1, 0) = controlled_system.getY();

  x_hat.submat(0, 0, n_x - 1, 0) = controller.getX();
  x_true.submat(0, 0, n_x - 1, 0) = controlled_system.getX();

  m_hat.submat(0, 0, n_x - 1, 0) = controller.getM();
  m_true.submat(0, 0, n_x - 1, 0) = controlled_system.getM();

  cout << "Starting " << n_t * dt << " sec simulation ... \n";
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t t = 1; t < n_t; t++) {
    // simulate a stochastically switched disturbance
    armaVec chance = arma::randu<arma::vec>(1);
    if (which_m == 0)  // low disturbance
    {
      if (chance[0] < pr_lo2hi) {  // switches low -> high disturbance
        m0_true = vector<data_t>(n_y, m_high);
        which_m = 1;
      }
    } else {                       // high disturbance
      if (chance[0] < pr_hi2lo) {  // swithces high -> low disturbance
        m0_true = vector<data_t>(n_y, m_low);
        which_m = 0;
      }
    }
    controlled_system.setM(m0_true);

    // input
    armaVec u_tm1(u.colptr(t - 1), u.n_rows, false, true);

    // Simulate the true system.
    controlled_system.setU(u_tm1);
    controlled_system.simPredict();
    armaVec z_t(z.colptr(t), z.n_rows, false, true);
    controlled_system.simMeasurement(z_t);

    // This method uses a steady-state solution to control problem to calculate
    // xRef, uRef from reference output yRef. Therefore, it is only applicable
    // to regulation problems or cases where reference trajectory changes slowly
    // compared to system dynamics.
    controller.steadyState_fbCtrl(z_t);

    // save the signals
    y_true.col(t) = controlled_system.getY();
    x_true.col(t) = controlled_system.getX();
    m_true.col(t) = controlled_system.getM();

    y_hat.col(t) = controller.getY();
    x_hat.col(t) = controller.getX();
    m_hat.col(t) = controller.getM();

    u.col(t) = controller.getU();
  }

  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<data_t, std::milli> sim_time_ms = finish - start;
  cout << "Finished simulation in " << sim_time_ms.count() << " ms.\n";
  cout << "(app. " << (sim_time_ms.count() / n_t) * 1e3 << " us/time-step)\n";

  cout << "Saving simulation data to disk.\n";

  // saved variables: dt, lambdaHat, xHat, mHat, z, u, lambdaRef, lambdaTrue,
  // xTrue, mTrue saving with hdf5 via armadillo
  arma::hdf5_opts::opts replace = arma::hdf5_opts::replace;

  auto dt_vec = arma::vec(1).fill(dt);
  dt_vec.save(arma::hdf5_name("eg_glds_ctrl.h5", "dt"));
  y_ref.save(arma::hdf5_name("eg_glds_ctrl.h5", "y_ref", replace));
  u.save(arma::hdf5_name("eg_glds_ctrl.h5", "u", replace));
  z.save(arma::hdf5_name("eg_glds_ctrl.h5", "z", replace));
  x_true.save(arma::hdf5_name("eg_glds_ctrl.h5", "x_true", replace));
  m_true.save(arma::hdf5_name("eg_glds_ctrl.h5", "m_true", replace));
  y_true.save(arma::hdf5_name("eg_glds_ctrl.h5", "y_true", replace));
  x_hat.save(arma::hdf5_name("eg_glds_ctrl.h5", "x_hat", replace));
  m_hat.save(arma::hdf5_name("eg_glds_ctrl.h5", "m_hat", replace));
  y_hat.save(arma::hdf5_name("eg_glds_ctrl.h5", "y_hat", replace));

  cout << "fin.\n";
  return 0;
}
