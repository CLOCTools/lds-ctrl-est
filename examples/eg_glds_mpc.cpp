//===-- eg_glds_mpc.cpp - Example GMPC Control ---------------------------===//
//
// Copyright 2025 Chia-Chien Hung, Kyle Johnsen
// Copyright 2025 Georgia Institute of Technology
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
/// \brief Example GMPC Control
///
/// \example eg_glds_mpc.cpp
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>

using lds::Matrix;
using lds::Vector;
using lds::data_t;
using std::cout;

auto main() -> int {
  cout << " ********** Example Gaussian LDS Control ********** \n\n";

  // Make 1st-order SISO system, sampled at 1kHz
  data_t dt = 1e-3;
  size_t n_u = 1;
  size_t n_x = 1;
  size_t n_y = 1;

  data_t t_sim = 0.05; // time for each control step
  size_t n_sim = static_cast<int>(t_sim / dt); // number of time steps per control step
  auto n_t = static_cast<size_t>(5.0 / dt); // number of time steps for simulation
  auto n_c = static_cast<size_t>(5.0 / t_sim); // number of control steps for simulation

  // construct ground truth system to be controlled...
  // initializes to random walk model with top-most n_y state observed
  lds::gaussian::System controlled_system(n_u, n_x, n_y, dt);

  // Ground-truth parameters for the controlled system
  // (stand-in for physical system to be controlled)
  Matrix a_true(n_x, n_x, arma::fill::eye);
  a_true[0] = exp(-dt / 0.01);
  Matrix b_true = Matrix(n_x, n_u).fill(2e-4);
  // control signal to model input unit conversion e.g., V -> mW/mm2:
  Vector g_true = Vector(n_y).fill(10.0);

  // output noise covariance
  Matrix r_true = Matrix(n_y, n_y, arma::fill::eye) * 1e-4;

  // Assign params.
  controlled_system.set_A(a_true);
  controlled_system.set_B(b_true);
  controlled_system.set_g(g_true);
  controlled_system.set_R(r_true);

  cout << ".....................................\n";
  cout << "controlled_system:\n";
  cout << ".....................................\n";
  controlled_system.Print();
  cout << ".....................................\n";

  // make a controller
  lds::gaussian::MpcController controller;
  const size_t N = 25;  // Prediction horizon
  const size_t M = 20;  // Control horizon
  {
    // for this demo, just use arbitrary default R
    Matrix r_controller = Matrix(n_y, n_y, arma::fill::eye) * lds::kDefaultR0;

    lds::gaussian::System controller_system(controlled_system);
    controller_system.set_R(r_controller);
    controller_system.Reset();  // reset to new m

    // going to adaptively re-estimate the disturbance
    controller_system.do_adapt_m = true;

    Matrix C = Matrix(n_y, n_x, arma::fill::eye);
    Matrix Q_y = C.t() * C * 1e5;
    Matrix R = Matrix(n_u, n_u, arma::fill::eye) * 1e-1;  // using dense instead of sparse matrix
    Matrix S = Matrix(n_u, n_u, arma::fill::zeros);  // using dense instead of sparse matrix

    Vector umin = {0};
    Vector umax = {5};
    if (n_u == 2) {
      umin = {0, 0};
      umax = {5, 5};
    } else if (n_u == 3) {
      umin = {0, 0, 0};
      umax = {5, 5, 5};
    }

    Vector xmin(b_true.n_rows);
    xmin.fill(-arma::datum::inf);
    Vector xmax(b_true.n_rows);
    xmax.fill(arma::datum::inf);

    controller =
        std::move(lds::gaussian::MpcController(std::move(controller_system)));
    controller.set_cost_output(Q_y, R, S, N, M);
    controller.set_constraint(xmin, xmax, umin, umax);
  }

  // Control variables:
  // Reference/target output, controller gains
  Vector y_ref0 = Vector(n_y).fill(20.0 * dt);

  // setting initial state to target to avoid error at onset:
  Vector x0 = Vector(n_x).fill(y_ref0[0]);

  cout << ".....................................\n";
  cout << "control system:\n";
  cout << ".....................................\n";
  controller.Print();
  cout << ".....................................\n";

  // set up variables for simulation
  // create Matrix to save outputs in...
  Matrix y_ref = Matrix(n_y, n_t + (N * n_sim), arma::fill::ones) * y_ref0[0];

  // Simulated measurements
  Matrix z(n_y, n_t, arma::fill::zeros);

  // simulated control signal ([=] V)
  Matrix u(n_u, n_t, arma::fill::zeros);

  // outputs, states and gain/disturbance params
  // *_hat indicates online estimates
  Matrix y_hat(n_y, n_t, arma::fill::zeros);
  Matrix x_hat(n_x, n_t, arma::fill::zeros);

  // *_true indicates ground truth (system being controlled)
  Matrix y_true(n_y, n_t, arma::fill::zeros);
  Matrix x_true(n_x, n_t, arma::fill::zeros);

  // MPC cost
  Matrix J(1, n_t, arma::fill::zeros);

  // set initial val
  y_hat.submat(0, 0, n_y - 1, 0) = controller.sys().y();
  y_true.submat(0, 0, n_y - 1, 0) = controlled_system.y();

  x_hat.submat(0, 0, n_x - 1, 0) = controller.sys().x();
  x_true.submat(0, 0, n_x - 1, 0) = controlled_system.x();

  cout << "Starting " << n_t * dt << " sec simulation ... \n";
  auto start = std::chrono::high_resolution_clock::now();

  // Simulate the true system.
  for (int i = 0; i < n_sim; i++) {
    // input
    Vector u_tm1(u.colptr(i), u.n_rows, false, true);

    z.col(i) = controlled_system.Simulate(u_tm1);

    // save the signals
    y_true.col(i) = controlled_system.y();
    x_true.col(i) = controlled_system.x();

    y_hat.col(i) = controller.sys().y();
    x_hat.col(i) = controller.sys().x();
  }

  for (size_t s = 1; s < n_c; s++) {
    // Calculate the slice indices
    size_t start_idx = s * n_sim;
    size_t end_idx = ((s + N) * n_sim) - 1;

    // This method uses a steady-state solution to control problem to calculate
    // x_ref, u_ref from reference output y_ref. Therefore, it is only
    // applicable to regulation problems or cases where reference trajectory
    // changes slowly compared to system dynamics.
    auto* j = new data_t;
    auto u_next = controller.ControlOutputReference(t_sim, z.col(start_idx - 1), y_ref.cols(start_idx, end_idx), true, j);
    for (int t = 0; t < n_sim; t++) {
      u.col(start_idx + t) = u_next;
      J.col(start_idx + t) = *j;
    }

    // Simulate the true system.
    for (int i = 0; i < n_sim; i++) {
      int t = start_idx + i;

      // input
      Vector u_tm1(u.colptr(t), u.n_rows, false, true);

      z.col(t) = controlled_system.Simulate(u_tm1);

      // save the signals
      y_true.col(t) = controlled_system.y();
      x_true.col(t) = controlled_system.x();

      y_hat.col(t) = controller.sys().y();
      x_hat.col(t) = controller.sys().x();
    }
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
  dt_vec.save(arma::hdf5_name("eg_glds_mpc.h5", "dt"));
  Matrix y_ref_clip = y_ref.cols(0, n_t - 1);
  y_ref_clip.save(arma::hdf5_name("eg_glds_mpc.h5", "y_ref", replace));
  u.save(arma::hdf5_name("eg_glds_mpc.h5", "u", replace));
  z.save(arma::hdf5_name("eg_glds_mpc.h5", "z", replace));
  x_true.save(arma::hdf5_name("eg_glds_mpc.h5", "x_true", replace));
  y_true.save(arma::hdf5_name("eg_glds_mpc.h5", "y_true", replace));
  x_hat.save(arma::hdf5_name("eg_glds_mpc.h5", "x_hat", replace));
  y_hat.save(arma::hdf5_name("eg_glds_mpc.h5", "y_hat", replace));
  J.save(arma::hdf5_name("eg_glds_mpc.h5", "j", replace));

  cout << "fin.\n";
  return 0;
}
