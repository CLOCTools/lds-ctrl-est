//===-- eg_plds_logLinCtrl.cpp - Example PLDS Control ---------------------===//
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
/// \brief Example PLDS Control
///
/// \example eg_plds_logLinCtrl.cpp
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>

using lds::armaMat;
using lds::armaVec;
using lds::data_t;
using std::cout;
using std::vector;

auto main(void) -> int {
  cout << " ********** Example Poisson LDS Control ********** \n\n";

  // Make SISO system sampled at 1kHz
  data_t dt = 1e-3;
  size_t n_u = 1;
  size_t n_x = 1;
  size_t n_y = 1;

  // no time steps for simulation.
  size_t n_t = static_cast<size_t>(10.0 / dt);

  // Control variables: _reference/target output, controller gains
  // n.b., Can either use armaVec (arma::Col) or std::vector
  armaVec y_ref0 = armaVec(n_y, arma::fill::ones) * 30.0 * dt;
  armaMat k_x =
      armaMat(n_u, n_x, arma::fill::zeros) + 1;  // gains on state error
  armaMat k_inty = armaMat(n_u, n_y, arma::fill::zeros) +
                   10;  // gains on integrated output err

  // Set control type bit mask, so controller knows what to do
  size_t control_type = lds::CONTROL_TYPE_INTY;  // integral action

  // // uncomment to use control that was designed to minimize change in control
  // // signal, rather than instantaneous amplitude
  // control_type = control_type | lds::CONTROL_TYPE_U;
  // if (control_type & lds::CONTROL_TYPE_U) {
  //   k_x *= dt*10;
  //   k_inty *= 0;
  // }

  // Ground-truth parameters for the controlled system
  // (stand-in for physical system to be controlled)
  armaMat a_true(n_x, n_x, arma::fill::eye);
  a_true[0] = 0.986;
  armaMat b_true(n_x, n_u, arma::fill::zeros);
  b_true[0] = 0.054;
  armaVec x0_true = armaVec(n_x, arma::fill::ones) * log(1 * dt);

  /// Going to simulate a switching disturbance (m) acting on system
  size_t which_m = 0;
  data_t m_low = log(1 * dt) * (1 - a_true[0]);
  data_t pr_lo2hi = 1e-3;
  data_t m_high = log(20 * dt) * (1 - a_true[0]);
  data_t pr_hi2lo = pr_lo2hi;

  armaVec m0_true = armaVec(n_y, arma::fill::ones) * m_low;
  // construct ground truth system to be controlled...
  lds::poisson::sys_t controlled_system(n_u, n_x, n_y, dt);

  // Assign params.
  controlled_system.setA(a_true);
  controlled_system.setB(b_true);
  controlled_system.setM(m0_true);
  controlled_system.setX0(x0_true);
  // reset to initial conditions
  controlled_system.reset();

  cout << ".....................................\n";
  cout << "controlled_system:\n";
  cout << ".....................................\n";
  controlled_system.printSys();
  cout << ".....................................\n";

  // Create model used for control.
  // for this example, assume model correct, except disturbance
  armaVec m0_controller = armaVec(n_y, arma::fill::ones) * m_low;

  data_t uLB = 0.0;
  data_t uUB = 5.0;
  lds::poisson::ctrl_t controller(n_u, n_x, n_y, uLB, uUB, dt);

  // adaptively re-estimate process disturbance (m)
  controller.adaptM = true;

  // set controller type
  controller.setControlType(control_type);

  // Assign parameters
  // **n.b. using arbitrary default for Q in this example. Really these should
  // be set by users.**
  controller.setA(a_true);
  controller.setB(b_true);
  controller.setM(m0_controller);
  // setting initial state to target to avoid large error at onset:
  armaVec x0_controller = arma::log(y_ref0);
  controller.setX0(x0_controller);
  // reset to new initial conditions
  controller.reset();

  // set adaptation rate by changing covariance of assumed process noise acting
  // on random-walk evolution of m
  armaMat q_m = armaMat(n_x, n_x, arma::fill::eye) * 1e-5;
  controller.setQ_m(q_m);

  // set controller gains
  controller.setKc_x(k_x);
  controller.setKc_inty(k_inty);

  // to protect against integral windup when output is consistently above
  // target:
  data_t tau_awu(0.1);
  controller.setTauAntiWindup(tau_awu);

  cout << ".....................................\n";
  cout << "controller:\n";
  cout << ".....................................\n";
  controller.printSys();
  cout << ".....................................\n";

  // create armaMatrix to save outputs in...
  armaMat y_ref = armaMat(n_y, n_t, arma::fill::zeros);
  y_ref.each_col() += y_ref0;

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

  // set initial val
  y_hat.col(0) = controller.getY();
  y_true.col(0) = controlled_system.getY();

  x_hat.col(0) = controller.getX();
  x_true.col(0) = controlled_system.getX();

  m_hat.col(0) = controller.getM();
  m_true.col(0) = controlled_system.getM();

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

    // e.g., use sinusoidal reference
    data_t f = 0.5;  // freq [=] Hz
    armaVec t_vec = armaVec(n_y, arma::fill::ones) * t;
    y_ref.col(t) +=
        y_ref0 % arma::sin(f * 2 * lds::pi * dt * t_vec - lds::pi / 4);

    armaVec y_ref_t(y_ref.colptr(t), n_y, false, true);

    // input
    armaVec u_km1 = armaVec(u.colptr(t - 1), u.n_rows, false, true);

    // Simulate the true system.
    controlled_system.setU(u_km1);
    controlled_system.simPredict();
    armaVec z_k = armaVec(z.colptr(t), z.n_rows, false, true);
    controlled_system.simMeasurement(z_k);

    // This method uses a steady-state solution to control problem to calculate
    // x_ref, u_ref from reference output y_ref. Notably, it does this in the
    // log-linear space (i.e., log(y)).
    //
    // Therefore, it is only applicable to regulation problems or cases where
    // reference trajectory changes slowly compared to system dynamics.
    controller.setYRef(y_ref_t);
    controller.steadyState_logLin_fbCtrl(z_k);

    y_true.col(t) = controlled_system.getY();
    x_true.col(t) = controlled_system.getX();
    m_true.col(t) = controlled_system.getM();

    y_hat.col(t) = controller.getY();
    x_hat.col(t) = controller.getX();
    m_hat.col(t) = controller.getM();

    // write control to u
    armaVec u_k = armaVec(u.colptr(t), u.n_rows, false, true);
    u_k = controller.getU();
  }

  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<data_t, std::milli> simTime_ms = finish - start;
  cout << "Finished simulation in " << simTime_ms.count() << " ms.\n";
  cout << "(app. " << (simTime_ms.count() / n_t) * 1e3 << " us/time-step)\n";

  // saved variables: dt, y_hat, x_hat, m_hat, z, u, y_ref, y_true,
  // x_true, m_true saving with hdf5 via armadillo
  arma::hdf5_opts::opts replace = arma::hdf5_opts::replace;

  auto dt_vec = arma::vec(1).fill(dt);
  dt_vec.save(arma::hdf5_name("eg_plds_ctrl.h5", "dt"));
  y_ref.save(arma::hdf5_name("eg_plds_ctrl.h5", "y_ref", replace));
  u.save(arma::hdf5_name("eg_plds_ctrl.h5", "u", replace));
  z.save(arma::hdf5_name("eg_plds_ctrl.h5", "z", replace));
  x_true.save(arma::hdf5_name("eg_plds_ctrl.h5", "x_true", replace));
  m_true.save(arma::hdf5_name("eg_plds_ctrl.h5", "m_true", replace));
  y_true.save(arma::hdf5_name("eg_plds_ctrl.h5", "y_true", replace));
  x_hat.save(arma::hdf5_name("eg_plds_ctrl.h5", "x_hat", replace));
  m_hat.save(arma::hdf5_name("eg_plds_ctrl.h5", "m_hat", replace));
  y_hat.save(arma::hdf5_name("eg_plds_ctrl.h5", "y_hat", replace));

  return 0;
}
