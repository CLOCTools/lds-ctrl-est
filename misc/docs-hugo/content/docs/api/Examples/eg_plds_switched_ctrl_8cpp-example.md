---
title: eg_plds_switched_ctrl.cpp
summary: Example Switched PLDS Control. 

---

# eg_plds_switched_ctrl.cpp



Example Switched PLDS Control 

```cpp
//===-- eg_plds_switched_ctrl.cpp - Example Switched PLDS Control ---===//
//
// Copyright 2021 Michael Bolus
// Copyright 2021 Georgia Institute of Technology
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
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>

using lds::data_t;
using lds::Matrix;
using lds::Vector;
using std::cout;

auto main() -> int {
  cout << " ********** Example Switched Poisson LDS Control ********** \n\n";

  // whether to do switched control
  bool do_switch_ctrl = true;

  // Make SISO system sampled at 1kHz
  data_t dt = 1e-3;
  size_t n_u = 1;
  size_t n_x = 1;
  size_t n_y = 1;

  // no time steps for simulation.
  auto n_t = static_cast<size_t>(30.0 / dt);

  // for simulating switching
  size_t which_mode = 1;
  data_t pr_21 = 1e-3;   // prob mode 1 -> 2
  data_t pr_12 = pr_21;  // prob mode 2 -> 1

  // simulated system being controlled
  lds::poisson::System controlled_system(n_u, n_x, n_y, dt);

  // **Assume the system is not well characterized by one LDS, but is well
  // characterized by two LDS models with different input matrices.**
  data_t scale_sys_b = 2;

  Matrix a(n_x, n_x, arma::fill::eye);
  a[0] = 0.985;
  Matrix b1 = Matrix(n_x, n_u).fill(0.05);
  Vector d = Vector(n_y, arma::fill::zeros).fill(log(1 * dt));

  controlled_system.set_A(a);
  controlled_system.set_B(b1);
  controlled_system.set_d(d);
  controlled_system.Reset();  // reset to initial conditions

  // reference
  Vector y_ref0 = Vector(n_y, arma::fill::zeros).fill(25.0 * dt);

  // Let underlying system 1 be more sensitive than system 2
  Matrix b2 = Matrix(n_x, n_u).fill(b1[0] / scale_sys_b);

  // create switched controller
  lds::poisson::SwitchedController switched_controller;
  lds::UniformMatrixList<> k_x;  // feedback controller gains
  {
    // create switched controller sub-systems
    // system 1
    lds::poisson::System sys1(controlled_system);

    // set process noise covariance
    Matrix q_controller = Matrix(n_x, n_x, arma::fill::eye) * 5e-3;
    sys1.set_Q(q_controller);

    // adaptively estimate process disturbance (m)
    // n.b. using arbitrary default value for process noise if enabled.
    sys1.do_adapt_m = true;

    // setting initial mode to target to avoid large error at onset:
    Vector x0_controller = arma::log(y_ref0) - d;
    sys1.set_x0(x0_controller);
    sys1.Reset();  // reset to initial conditions

    cout << ".....................................\n";
    cout << "sys1:\n";
    cout << ".....................................\n";
    sys1.Print();
    cout << ".....................................\n";

    // system 2
    lds::poisson::System sys2 = sys1;

    // set parameters
    sys2.set_B(b2);

    cout << ".....................................\n";
    cout << "sys2:\n";
    cout << ".....................................\n";
    sys2.Print();
    cout << ".....................................\n";

    lds::UniformSystemList<lds::poisson::System> systems({sys1, sys2});

    // controller gains for underlying system s:
    Matrix k_x1(n_u, n_x, arma::fill::ones);
    Matrix k_x2 = scale_sys_b * k_x1;  // system2 is x-times less sensitive.
    k_x = lds::UniformMatrixList<>({k_x1, k_x2});

    data_t u_lb = 0.0;
    data_t u_ub = 5.0;
    switched_controller = std::move(
        lds::poisson::SwitchedController(std::move(systems), u_lb, u_ub));
  }
  // Control variables
  size_t control_type = 0;  // no integral action, etc
  switched_controller.set_control_type(control_type);
  switched_controller.set_Kc(std::move(k_x));

  switched_controller.set_y_ref(y_ref0);

  std::vector<lds::poisson::System> systems_vec(3, lds::poisson::System());
  lds::UniformSystemList<lds::poisson::System> systems(std::move(systems_vec));

  cout << ".....................................\n";
  cout << "switched_controller:\n";
  cout << ".....................................\n";
  switched_controller.Print();
  cout << ".....................................\n";

  // Fake measurements
  Matrix z(n_y, n_t, arma::fill::zeros);

  // Will later contain control.
  Matrix u(n_u, n_t, arma::fill::zeros);

  // create Matrixrix to save outputs in...
  Matrix y_hat(n_y, n_t, arma::fill::zeros);
  Matrix y_true(n_y, n_t, arma::fill::zeros);
  Matrix y_ref = Matrix(n_y, n_t).fill(y_ref0[0]);

  // modes and gain/disturbance params
  Matrix x_hat(n_x, n_t, arma::fill::zeros);
  Matrix x_true(n_x, n_t, arma::fill::zeros);
  Matrix mode(1, n_t, arma::fill::ones);

  // set initial val
  y_hat.col(0) = switched_controller.sys().y();
  y_true.col(0) = controlled_system.y();
  x_hat.col(0) = switched_controller.sys().x();
  x_true.col(0) = controlled_system.x();

  cout << "Starting " << n_t * dt << " sec simulation ... \n";
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t t = 1; t < n_t; t++) {
    // Let the controlled system stochastically change gain
    // Assume another algorithm decodes this mode change and signals the
    // switched_controller
    Vector chance(1, arma::fill::randu);
    if (which_mode == 1)  // mode1
    {
      if (chance[0] < pr_21) {
        which_mode = 2;
        controlled_system.set_B(b2);
        if (do_switch_ctrl) {
          switched_controller.Switch(1);
        }
      }
    } else {  // mode2
      if (chance[0] < pr_12) {
        which_mode = 1;
        controlled_system.set_B(b1);
        if (do_switch_ctrl) {
          switched_controller.Switch(0);
        }
      }
    }

    // Simulate the true system.
    z.col(t) = controlled_system.Simulate(u.col(t - 1));

    // perform control
    u.col(t) = switched_controller.ControlOutputReference(z.col(t));

    mode.col(t) = which_mode;
    y_ref.col(t) = y_ref0;
    y_true.col(t) = controlled_system.y();
    x_true.col(t) = controlled_system.x();
    y_hat.col(t) = switched_controller.sys().y();
    x_hat.col(t) = switched_controller.sys().x();
  }

  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<data_t, std::milli> sim_time_ms = finish - start;
  cout << "Finished simulation in " << sim_time_ms.count() << " ms.\n";
  cout << "(app. " << (sim_time_ms.count() / n_t) * 1e3 << " us/time-step)\n";

  // saved variables: dt, y_hat, x_hat, m_hat, z, u, y_ref, y_true,
  // x_true, m_true saving with hdf5 via armadillo
  arma::hdf5_opts::opts replace = arma::hdf5_opts::replace;

  auto dt_vec = Vector(1).fill(dt);
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
```

_Filename: eg_plds_switched_ctrl.cpp_

-------------------------------

Updated on 22 June 2021 at 23:08:17 CDT
