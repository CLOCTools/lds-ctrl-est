---
title: examples/eg_plds_ctrl.cpp

---

# examples/eg_plds_ctrl.cpp



## Functions

|                | Name           |
| -------------- | -------------- |
| auto | **[main](/lds-ctrl-est/docs/api/files/eg__plds__ctrl_8cpp/#function-main)**() |


## Function Details

### main

```cpp
auto main()
```



Going to simulate a switching disturbance (m) acting on system





## Source code

```cpp
//===-- eg_plds_ctrl.cpp - Example PLDS Control ---------------------===//
//
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

using lds::Matrix;
using lds::Vector;
using lds::data_t;
using std::cout;

auto main() -> int {
  cout << " ********** Example Poisson LDS Control ********** \n\n";

  // Make SISO system sampled at 1kHz
  data_t dt = 1e-3;
  size_t n_u = 1;
  size_t n_x = 1;
  size_t n_y = 1;

  // no time steps for simulation.
  auto n_t = static_cast<size_t>(10.0 / dt);

  // Control variables: _reference/target output, controller gains
  // n.b., Can either use Vector (arma::Col) or std::vector
  Vector y_ref0 = Vector(n_y, arma::fill::ones) * 30.0 * dt;
  Matrix k_x =
      Matrix(n_u, n_x, arma::fill::zeros) + 1;  // gains on state error
  Matrix k_inty = Matrix(n_u, n_y, arma::fill::zeros) +
                   10;  // gains on integrated output err

  // Set control type bit mask, so controller knows what to do
  size_t control_type = lds::kControlTypeIntY;  // integral action

  // // uncomment to use control that was designed to minimize change in control
  // // signal, rather than instantaneous amplitude
  // control_type = control_type | lds::kControlTypeDeltaU;
  // if (control_type & lds::kControlTypeDeltaU) {
  //   k_x *= dt*10;
  //   k_inty *= 0;
  // }

  // Ground-truth parameters for the controlled system
  // (stand-in for physical system to be controlled)
  Matrix a_true(n_x, n_x, arma::fill::eye);
  a_true[0] = 0.986;
  Matrix b_true(n_x, n_u, arma::fill::zeros);
  b_true[0] = 0.054;
  Vector x0_true = Vector(n_x, arma::fill::ones) * log(1 * dt);

  size_t which_m = 0;
  data_t m_low = log(1 * dt) * (1 - a_true[0]);
  data_t pr_lo2hi = 1e-3;
  data_t m_high = log(20 * dt) * (1 - a_true[0]);
  data_t pr_hi2lo = pr_lo2hi;

  Vector m0_true = Vector(n_y, arma::fill::ones) * m_low;
  // construct ground truth system to be controlled...
  lds::poisson::System controlled_system(n_u, n_x, n_y, dt);

  // Assign params.
  controlled_system.set_A(a_true);
  controlled_system.set_B(b_true);
  controlled_system.set_m(m0_true);
  controlled_system.set_x0(x0_true);
  // reset to initial conditions
  controlled_system.Reset();

  cout << ".....................................\n";
  cout << "controlled_system:\n";
  cout << ".....................................\n";
  controlled_system.Print();
  cout << ".....................................\n";

  // Create the controller
  lds::poisson::Controller controller;
  {
    // Create model used for control.
    lds::poisson::System controller_system(controlled_system);

    // for this example, assume model correct, except disturbance
    Vector m0_controller = Vector(n_y, arma::fill::ones) * m_low;
    Vector x0_controller = arma::log(y_ref0);
    controller_system.set_m(m0_controller);
    controller_system.set_x0(x0_controller);
    controller_system.Reset(); //reset to new init condition

    // adaptively re-estimate process disturbance (m)
    controller_system.do_adapt_m = true;

    // set adaptation rate by changing covariance of assumed process noise
    // acting on random-walk evolution of m
    Matrix q_m = Matrix(n_x, n_x, arma::fill::eye) * 1e-5;
    controller_system.set_Q_m(q_m);

    data_t u_lb = 0.0;
    data_t u_ub = 5.0;
    controller = std::move(
        lds::poisson::Controller(std::move(controller_system), u_lb, u_ub));
  }
  // set controller type
  controller.set_control_type(control_type);

  // set controller gains
  controller.set_Kc(k_x);
  controller.set_Kc_inty(k_inty);

  // to protect against integral windup when output is consistently above
  // target:
  data_t tau_awu(0.1);
  controller.set_tau_awu(tau_awu);

  cout << ".....................................\n";
  cout << "controller:\n";
  cout << ".....................................\n";
  controller.Print();
  cout << ".....................................\n";

  // create Matrixrix to save outputs in...
  Matrix y_ref = Matrix(n_y, n_t, arma::fill::zeros);
  y_ref.each_col() += y_ref0;

  // Simulated measurements
  Matrix z(n_y, n_t, arma::fill::zeros);

  // simulated control signal ([=] V)
  Matrix u(n_u, n_t, arma::fill::zeros);

  // outputs, states and gain/disturbance params
  // *_hat indicates online estimates
  Matrix y_hat(n_y, n_t, arma::fill::zeros);
  Matrix x_hat(n_x, n_t, arma::fill::zeros);
  Matrix m_hat(n_y, n_t, arma::fill::zeros);

  // *_true indicates ground truth (system being controlled)
  Matrix y_true(n_y, n_t, arma::fill::zeros);
  Matrix x_true(n_x, n_t, arma::fill::zeros);
  Matrix m_true(n_y, n_t, arma::fill::zeros);

  // set initial val
  y_hat.col(0) = controller.sys().y();
  y_true.col(0) = controlled_system.y();

  x_hat.col(0) = controller.sys().x();
  x_true.col(0) = controlled_system.x();

  m_hat.col(0) = controller.sys().m();
  m_true.col(0) = controlled_system.m();

  cout << "Starting " << n_t * dt << " sec simulation ... \n";
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t t = 1; t < n_t; t++) {
    // simulate a stochastically switched disturbance
    Vector chance = arma::randu<Vector>(1);
    if (which_m == 0)  // low disturbance
    {
      if (chance[0] < pr_lo2hi) {  // switches low -> high disturbance
        m0_true = std::vector<data_t>(n_y, m_high);
        which_m = 1;
      }
    } else {                       // high disturbance
      if (chance[0] < pr_hi2lo) {  // swithces high -> low disturbance
        m0_true = std::vector<data_t>(n_y, m_low);
        which_m = 0;
      }
    }
    controlled_system.set_m(m0_true);

    // e.g., use sinusoidal reference
    data_t f = 0.5;  // freq [=] Hz
    Vector t_vec = Vector(n_y, arma::fill::ones) * t;
    y_ref.col(t) +=
        y_ref0 % arma::sin(f * 2 * lds::kPi * dt * t_vec - lds::kPi / 4);

    // Simulate the true system.
    z.col(t)=controlled_system.Simulate(u.col(t-1));

    // This method uses a steady-state solution to control problem to calculate
    // x_ref, u_ref from reference output y_ref. Notably, it does this in the
    // log-linear space (i.e., log(y)).
    //
    // Therefore, it is only applicable to regulation problems or cases where
    // reference trajectory changes slowly compared to system dynamics.
    controller.set_y_ref(y_ref.col(t));
    u.col(t)=controller.ControlOutputReference(z.col(t));

    y_true.col(t) = controlled_system.y();
    x_true.col(t) = controlled_system.x();
    m_true.col(t) = controlled_system.m();

    y_hat.col(t) = controller.sys().y();
    x_hat.col(t) = controller.sys().x();
    m_hat.col(t) = controller.sys().m();
  }

  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<data_t, std::milli> sim_time_ms = finish - start;
  cout << "Finished simulation in " << sim_time_ms.count() << " ms.\n";
  cout << "(app. " << (sim_time_ms.count() / n_t) * 1e3 << " us/time-step)\n";

  // saved variables: dt, y_hat, x_hat, m_hat, z, u, y_ref, y_true,
  // x_true, m_true saving with hdf5 via armadillo
  arma::hdf5_opts::opts replace = arma::hdf5_opts::replace;

  auto dt_vec = Vector(1).fill(dt);
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
```


-------------------------------

Updated on 24 March 2021 at 13:11:19 EDT
