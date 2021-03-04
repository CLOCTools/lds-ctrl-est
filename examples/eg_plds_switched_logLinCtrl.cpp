//===-- eg_plds_switched_logLinCtrl.cpp - Example Switched PLDS Control ---===//
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
/// \brief Example Switched PLDS Control
///
/// \example eg_plds_switched_logLinCtrl.cpp
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>

using lds::armaMat;
using lds::armaVec;
using lds::data_t;
using std::cout;
using std::vector;

auto main(void) -> int {
  cout << " ********** Example Switched Poisson LDS Control ********** \n\n";

  // whether to do switched control
  bool do_switch_ctrl = true;

  // Make SISO system sampled at 1kHz
  data_t dt = 1e-3;
  size_t n_u = 1;
  size_t n_x = 1;
  size_t n_y = 1;

  // no time steps for simulation.
  size_t n_t = static_cast<size_t>(30.0 / dt);

  // for simulating switching
  size_t which_mode = 1;
  data_t pr_21 = 1e-3;   // prob mode 1 -> 2
  data_t pr_12 = pr_21;  // prob mode 2 -> 1

  // simulated system being controlled
  lds::poisson::sys_t controlled_system(n_u, n_x, n_y, dt);

  // **Assume the system is not well characterized by one LDS, but is well
  // characterized by two LDS models with different input matrices.**

  armaMat a(n_x, n_x, arma::fill::eye);
  a[0] = 0.985;
  armaMat b1 = armaMat(n_x, n_u).fill(0.05);
  armaVec d = armaVec(n_y, arma::fill::zeros).fill(log(1 * dt));

  controlled_system.setA(a);
  controlled_system.setB(b1);
  controlled_system.setD(d);
  controlled_system.reset();  // reset to initial conditions

  // create switched controller sub-systems
  // system 1
  data_t u_lb = 0.0;
  data_t u_ub = 5.0;
  lds::poisson::ctrl_t sys1(n_u, n_x, n_y, u_lb, u_ub, dt);

  // Control variables
  armaVec y_ref0 = armaVec(n_y, arma::fill::zeros).fill(25.0 * dt);
  // controller gains for underlying system 1:
  armaMat k_x1(n_u, n_x, arma::fill::ones);
  size_t control_type = 0;  // no integral action, etc.

  // setting initial mode to target to avoid large error at onset:
  armaVec x0_controller = arma::log(y_ref0) - d;

  armaMat q_controller = armaMat(n_x, n_x, arma::fill::eye) * 5e-3;

  sys1.setControlType(control_type);
  sys1.setA(a);
  sys1.setB(b1);
  sys1.setD(d);
  sys1.setX0(x0_controller);
  sys1.setKc_x(k_x1);
  sys1.setQ(q_controller);  // set process noise covariance

  cout << ".....................................\n";
  cout << "sys1:\n";
  cout << ".....................................\n";
  sys1.printSys();
  cout << ".....................................\n";

  // system 2
  lds::poisson::ctrl_t sys2 = sys1;

  // Let underlying system 1 be more sensitive than system 2
  data_t scale_sys_b = 2;
  armaMat b2 = armaMat(n_x, n_u).fill(b1[0] / scale_sys_b);
  armaMat k_x2 = scale_sys_b * k_x1;  // system2 is x-times less sensitive.

  // set parameters
  sys2.setB(b2);
  sys2.setKc_x(k_x2);

  cout << ".....................................\n";
  cout << "sys2:\n";
  cout << ".....................................\n";
  sys2.printSys();
  cout << ".....................................\n";

  // create 2-system composite switched-mode controller.
  lds::poisson::sctrl_t switched_controller(2, n_u, n_x, n_y, u_lb, u_ub, dt);
  // assign the parameters of the sub-systems 1 and 2
  switched_controller.setSystem(0, sys1);
  switched_controller.setSystem(1, sys2);
  switched_controller.switchSystem(0);  // start with system 1
  switched_controller.reset();          // reset to initial conditions

  // reference belongs to super system. Set here.
  switched_controller.setYRef(y_ref0);

  // adaptively estimate process disturbance (m)
  // n.b. using arbitrary default value for process noise if enabled.
  switched_controller.adaptM = true;

  cout << ".....................................\n";
  cout << "switched_controller:\n";
  cout << ".....................................\n";
  switched_controller.printSys();
  cout << ".....................................\n";

  // Fake measurements
  armaMat z(n_y, n_t, arma::fill::zeros);

  // Will later contain control.
  armaMat u(n_u, n_t, arma::fill::zeros);

  // create armaMatrix to save outputs in...
  armaMat y_hat(n_y, n_t, arma::fill::zeros);
  armaMat y_true(n_y, n_t, arma::fill::zeros);
  armaMat y_ref = armaMat(n_y, n_t).fill(y_ref0[0]);

  // modes and gain/disturbance params
  armaMat x_hat(n_x, n_t, arma::fill::zeros);
  armaMat x_true(n_x, n_t, arma::fill::zeros);
  armaMat mode(1, n_t, arma::fill::ones);

  // set initial val
  y_hat.col(0) = switched_controller.getY();
  y_true.col(0) = controlled_system.getY();
  x_hat.col(0) = switched_controller.getX();
  x_true.col(0) = controlled_system.getX();

  cout << "Starting " << n_t * dt << " sec simulation ... \n";
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t t = 1; t < n_t; t++) {
    // Let the controlled system stachastically change gain
    // Assume another algorithm decodes this mode change and signals the
    // switched_controller
    //
    armaVec chance(1, arma::fill::randu);
    if (which_mode == 1)  // mode1
    {
      if (chance[0] < pr_21) {
        which_mode = 2;
        controlled_system.setB(b2);
        if (do_switch_ctrl) switched_controller.switchSystem(1);
      }
    } else {  // mode2
      if (chance[0] < pr_12) {
        which_mode = 1;
        controlled_system.setB(b1);
        if (do_switch_ctrl) switched_controller.switchSystem(0);
      }
    }

    // input
    armaVec u_tm1 = armaVec(u.colptr(t - 1), u.n_rows, false, true);

    // Simulate the true system.
    controlled_system.setU(u_tm1);
    controlled_system.simPredict();
    armaVec z_k = armaVec(z.colptr(t), z.n_rows, false, true);
    controlled_system.simMeasurement(z_k);

    // perform control
    switched_controller.steadyState_logLin_fbCtrl(z_k);

    mode.col(t) = which_mode;
    y_ref.col(t) = y_ref0;
    y_true.col(t) = controlled_system.getY();
    x_true.col(t) = controlled_system.getX();
    y_hat.col(t) = switched_controller.getY();
    x_hat.col(t) = switched_controller.getX();

    // write control to u
    armaVec u_t = armaVec(u.colptr(t), u.n_rows, false, true);
    u_t = switched_controller.getU();
  }

  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<data_t, std::milli> simTime_ms = finish - start;
  cout << "Finished simulation in " << simTime_ms.count() << " ms.\n";
  cout << "(app. " << (simTime_ms.count() / n_t) * 1e3 << " us/time-step)\n";

  // saved variables: dt, y_hat, x_hat, m_hat, z, u, y_ref, y_true,
  // x_true, m_true saving with hdf5 via armadillo
  arma::hdf5_opts::opts replace = arma::hdf5_opts::replace;

  auto dt_vec = arma::vec(1).fill(dt);
  dt_vec.save(arma::hdf5_name("eg_plds_switched_ctrl.h5", "dt"));
  y_ref.save(arma::hdf5_name("eg_plds_switched_ctrl.h5", "y_ref", replace));
  u.save(arma::hdf5_name("eg_plds_switched_ctrl.h5", "u", replace));
  z.save(arma::hdf5_name("eg_plds_switched_ctrl.h5", "z", replace));
  x_true.save(arma::hdf5_name("eg_plds_switched_ctrl.h5", "x_true", replace));
  y_true.save(arma::hdf5_name("eg_plds_switched_ctrl.h5", "y_true", replace));
  x_hat.save(arma::hdf5_name("eg_plds_switched_ctrl.h5", "x_hat", replace));
  y_hat.save(arma::hdf5_name("eg_plds_switched_ctrl.h5", "y_hat", replace));
  mode.save(arma::hdf5_name("eg_plds_switched_ctrl.h5", "mode", replace));

  return 0;
}
