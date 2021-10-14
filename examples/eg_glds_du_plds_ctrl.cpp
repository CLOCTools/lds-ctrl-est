//===-- eg_glds_du_plds_ctrl.cpp - Example GLDS Delta u Control of PLDS ---===//
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
/// \brief Example GLDS Control of PLDS where change in control (du) is being
/// updated, rather than amplitude (u).
///
/// \example eg_glds_du_plds_ctrl.cpp
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>

using lds::data_t;
using lds::Matrix;
using lds::Vector;
using std::cout;

auto main() -> int {
  cout << " ********** Example Gaussian LDS du Control of PLDS ********** \n\n";

  // Make 1st-order SISO system, sampled at 1kHz
  data_t dt = 1e-3;
  size_t n_u = 1;
  size_t n_x = 1;
  size_t n_y = 1;

  // no time steps for simulation.
  auto n_t = static_cast<size_t>(5.0 / dt);

  // construct ground truth system to be controlled...
  // initializes to random walk model with top-most n_y state observed
  lds::poisson::System controlled_system(n_u, n_x, n_y, dt);

  // Ground-truth parameters for the controlled system
  // (stand-in for physical system to be controlled)
  Matrix a_true(n_x, n_x, arma::fill::eye);
  a_true[0] = exp(-dt / 0.01);
  Matrix b_true = Matrix(n_x, n_u).fill(2.5e-2);
  // control signal to model input unit conversion e.g., V -> mW/mm2:
  Vector g_true = Vector(n_y).fill(10.0);

  /// Going to simulate a switching disturbance (m) acting on system
  size_t which_m = 0;  // whether low or high disturbance (0, 1)
  data_t m_low = log(1 * dt) * (1 - a_true[0]);
  data_t pr_lo2hi =
      0;  // 1e-3;  // probability of going from low to high disturb.
  data_t m_high = log(20 * dt) * (1 - a_true[0]);
  data_t pr_hi2lo = pr_lo2hi;

  // initially let m be low
  Vector m0_true = Vector(n_y).fill(m_low);
  Vector x0_true = Vector(n_x, arma::fill::ones) * log(1 * dt);

  // Assign params.
  controlled_system.set_A(a_true);
  controlled_system.set_B(b_true);
  controlled_system.set_m(m0_true);
  controlled_system.set_g(g_true);
  controlled_system.set_x0(x0_true);
  controlled_system.Reset();

  cout << ".....................................\n";
  cout << "controlled_system:\n";
  cout << ".....................................\n";
  controlled_system.Print();
  cout << ".....................................\n";

  // make a controller
  lds::gaussian::Controller controller;
  {
    // Create **incorrect** model used for control.
    // (e.g., imperfect model fitting)
    Matrix b_controller = b_true / 50;

    // let's assume zero process disturbance initially
    // (will be re-estimating)
    Vector m_controller = Vector(n_x, arma::fill::zeros);

    // process noise covariance
    Matrix q_controller = Matrix(n_y, n_y, arma::fill::eye) * 1e-8;

    // output noise covariance
    Matrix r_controller = Matrix(n_y, n_y, arma::fill::eye) * 1e-2;

    lds::gaussian::System controller_system(n_u, n_x, n_y, dt);
    controller_system.set_A(a_true);
    controller_system.set_B(b_controller);
    controller_system.set_g(g_true);
    controller_system.set_m(m_controller);
    controller_system.set_Q(q_controller);
    controller_system.set_R(r_controller);
    controller_system.Reset();  // reset to new m

    // going to adaptively re-estimate the disturbance
    controller_system.do_adapt_m = true;

    // set adaptation rate by changing covariance of assumed process noise
    // acting on random-walk evolution of m
    Matrix q_m = Matrix(n_x, n_x, arma::fill::eye) * 1e-8;
    controller_system.set_Q_m(q_m);

    // create controller
    // lower and upper bounds on control signal (e.g., in Volts)
    data_t u_lb = 0.0;  // [=] V
    data_t u_ub = 5.0;  // [=] V
    controller = std::move(
        lds::gaussian::Controller(std::move(controller_system), u_lb, u_ub));
  }

  // Control variables:
  // Reference/target output, controller gains
  Vector y_ref0 = Vector(n_y).fill(20.0 * dt);

  // to design for this example, augmented state with control and made the input
  // du; cost on output q_y = 1, on integral output = 1e2, on u = 0, on du =
  // 1e-2.
  Matrix k_x = Matrix(n_u, n_x).fill(2.44);     // gains on state error
  Matrix k_inty = Matrix(n_u, n_y).fill(97.4);  // gains on integrated err
  Matrix k_u = Matrix(n_u, n_u).fill(5.23e-2);  // gains on input amp

  // set up controller type bit mask so controller knows how to proceed
  size_t control_type = 0;
  // use integral action to minimize DC error
  control_type = control_type | lds::kControlTypeIntY;
  // update change in control (LP filters control)
  control_type = control_type | lds::kControlTypeDeltaU;

  // set controller type
  controller.set_control_type(control_type);

  // Let's say these controller gains were designed assuming g was 9 V/(mW/mm2):
  Vector g_design = Vector(n_u).fill(10);

  // Set params.
  // **n.b. using arbitrary defaults for Q, R in this example. Really, these
  // should be set by users, as they tune characteristics of Kalman filter.
  // Users can also choose not to recursively calculate the estimator gain and
  // supply it (setKe) instead of covariances.**
  controller.set_y_ref(y_ref0);
  controller.set_Kc(k_x);
  controller.set_Kc_inty(k_inty);
  controller.set_Kc_u(k_u);
  controller.set_g_design(g_design);

  cout << ".....................................\n";
  cout << "control system:\n";
  cout << ".....................................\n";
  controller.Print();
  cout << ".....................................\n";

  // set up variables for simulation
  // create Matrix to save outputs in...
  Matrix y_ref = Matrix(n_y, n_t, arma::fill::ones) * y_ref0[0];

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

  // get initial val
  y_hat.submat(0, 0, n_y - 1, 0) = controller.sys().y();
  y_true.submat(0, 0, n_y - 1, 0) = controlled_system.y();

  x_hat.submat(0, 0, n_x - 1, 0) = controller.sys().x();
  x_true.submat(0, 0, n_x - 1, 0) = controlled_system.x();

  m_hat.submat(0, 0, n_x - 1, 0) = controller.sys().m();
  m_true.submat(0, 0, n_x - 1, 0) = controlled_system.m();

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

    // input
    Vector u_tm1(u.colptr(t - 1), u.n_rows, false, true);

    // Simulate the true system.
    z.col(t) = controlled_system.Simulate(u_tm1);

    // This method uses a steady-state solution to control problem to calculate
    // x_ref, u_ref from reference output y_ref. Therefore, it is only
    // applicable to regulation problems or cases where reference trajectory
    // changes slowly compared to system dynamics.
    u.col(t) = controller.ControlOutputReference(z.col(t));

    // save the signals
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

  cout << "Saving simulation data to disk.\n";

  // saved variables: dt, lambdaHat, xHat, mHat, z, u, lambdaRef, lambdaTrue,
  // xTrue, mTrue saving with hdf5 via armadillo
  arma::hdf5_opts::opts replace = arma::hdf5_opts::replace;

  auto dt_vec = Vector(1).fill(dt);
  dt_vec.save(arma::hdf5_name("eg_glds_du_plds_ctrl.h5", "dt"));
  y_ref.save(arma::hdf5_name("eg_glds_du_plds_ctrl.h5", "y_ref", replace));
  u.save(arma::hdf5_name("eg_glds_du_plds_ctrl.h5", "u", replace));
  z.save(arma::hdf5_name("eg_glds_du_plds_ctrl.h5", "z", replace));
  x_true.save(arma::hdf5_name("eg_glds_du_plds_ctrl.h5", "x_true", replace));
  m_true.save(arma::hdf5_name("eg_glds_du_plds_ctrl.h5", "m_true", replace));
  y_true.save(arma::hdf5_name("eg_glds_du_plds_ctrl.h5", "y_true", replace));
  x_hat.save(arma::hdf5_name("eg_glds_du_plds_ctrl.h5", "x_hat", replace));
  m_hat.save(arma::hdf5_name("eg_glds_du_plds_ctrl.h5", "m_hat", replace));
  y_hat.save(arma::hdf5_name("eg_glds_du_plds_ctrl.h5", "y_hat", replace));

  cout << "fin.\n";
  return 0;
}
