//===-- eg_lqmpc_ctrl.cpp - Example LQMPC Control
//---------------------------===//
//
// Copyright 2024 Chia-Chien Hung and Kyle Johnsen
// Copyright 2024 Georgia Institute of Technology
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
/// \brief Example LQMPC Control
///
/// \example eg_lqmpc_ctrl.cpp
//===----------------------------------------------------------------------===//

#include <armadillo>
#include <ldsCtrlEst>

using lds::data_t;
using lds::Matrix;
using lds::Vector;
using lds::Sparse;
using std::cout;

auto main() -> int {
  cout << " ********** Example Gaussian MPC Control ********** \n\n";

  // Example as provided by CLOCTools/lqmpc

  auto z_to_y = [](const Matrix& z) -> Matrix {
    return (arma::log(z) + 5.468) / 61.4;
  };
  // Didn't implement y_to_z because it's unused

  const data_t dt = 1e-3;  // Sample period
  const size_t n_u = 2;    // Input dimensions
  const size_t n_x = 4;    // State dimensions
  const size_t n_y = 3;    // Output dimensions

  // Define the system matrices
  Matrix A = Matrix({{1, -6.66e-13, -2.03e-9, -4.14e-6},
                     {9.83e-4, 1, -4.09e-8, -8.32e-5},
                     {4.83e-7, 9.83e-4, 1, -5.34e-4},
                     {1.58e-10, 4.83e-7, 9.83e-4, .9994}});

  Matrix B = Matrix({{9.83e-4, 4.83e-7, 1.58e-10, 3.89e-14}}).t();
  Matrix B_ = Matrix({{1e-5, 1e-5, 1e-10, 1e-14}}).t();
  if (n_u == 2) {
    B = arma::join_rows(B, -B);
  } else if (n_u == 3) {
    B = arma::join_rows(B, -B, B_);
  }

  Matrix C = Matrix({{-.0096, .0135, .005, -.0095}});
  if (n_y == 2) {
    C = arma::join_cols(C, 2 * C);
  } else if (n_y == 3) {
    C = arma::join_cols(C, 2 * C, 3 * C);
  }

  // Initialize the system that is being controlled
  lds::gaussian::System controlled_system(n_u, n_x, n_y, dt);
  controlled_system.set_A(A);
  controlled_system.set_B(B);
  controlled_system.set_C(C);
  controlled_system.Reset();

  cout << ".....................................\n";
  cout << "controlled_system:\n";
  cout << ".....................................\n";
  controlled_system.Print();
  cout << ".....................................\n";

  // Initialize the controller
  lds::gaussian::MpcController controller;
  const size_t N = 25;  // Prediction horizon
  {
    Matrix Q = C.t() * C * 1e5;
    Matrix R = Matrix(n_u, n_u, arma::fill::eye) *
               1e-1;  // using dense instead of sparse matrix
    Matrix S = Matrix(
        n_u, n_u, arma::fill::zeros);  // using dense instead of sparse matrix

    Vector umin = {0};
    Vector umax = {5};
    if (n_u == 2) {
      umin = {0, 0};
      umax = {5, 5};
    } else if (n_u == 3) {
      umin = {0, 0, 0};
      umax = {5, 5, 5};
    }

    Vector xmin(B.n_rows);
    xmin.fill(-arma::datum::inf);
    Vector xmax(B.n_rows);
    xmax.fill(arma::datum::inf);

    lds::gaussian::System controller_system(controlled_system);

    controller = std::move(
        lds::gaussian::MpcController(std::move(controller_system), umin, umax));
    controller.set_control(Q, R, S, N, 20);
    controller.set_constraint(xmin, xmax, umin, umax);
  }

  cout << ".....................................\n";
  cout << "control system:\n";
  cout << ".....................................\n";
  controller.Print();
  cout << ".....................................\n";

  // Set up variables for simulation
  Vector u0 = Vector(n_u, arma::fill::zeros);
  Vector x0 = Vector(n_x, arma::fill::zeros);

  const size_t n_t = 120;     // Number of time steps
  const data_t t_sim = 0.25;  // Simulation time step
  Matrix zr = 0.05 * arma::sin(arma::linspace<Matrix>(0, 2 * arma::datum::pi,
                                                      (n_t + 25) * 250) *
                               12) +
              0.1;
  Matrix yr = z_to_y(zr.t());
  if (n_y == 2) {
    yr = arma::join_cols(yr, 2 * yr);
  } else if (n_y == 3) {
    yr = arma::join_cols(yr, 2 * yr, 3 * yr);
  }

  Matrix I = Matrix(n_x, n_x, arma::fill::eye);
  Matrix ur = arma::pinv(C * arma::inv(I - A) * B) * yr;
  Matrix xr = arma::inv(I - A) * B * ur;

  // create Matrix to save outputs in...
  Matrix y_ref = Matrix(n_y, n_t, arma::fill::zeros);
  Matrix y_true(n_y, n_t, arma::fill::zeros);
  Matrix u(n_u, n_t, arma::fill::zeros);
  Matrix J(1, n_t, arma::fill::zeros);

  // Simulate the system
  cout << "Starting " << n_t * t_sim << " sec simulation ... \n";
  auto t1 = std::chrono::high_resolution_clock::now();
  const size_t n_sim = static_cast<int>(t_sim / dt);

  for (size_t t = 0; t < n_t; ++t) {
    // Calculate the slice indices
    size_t start_idx = t * n_sim;
    size_t end_idx = (t + N) * n_sim - 1;

    auto* j = new data_t; // cost

    u0 = controller.Control(t_sim, x0, u0, xr.cols(start_idx, end_idx), j);

    for (size_t i = 0; i < n_sim; i++) {
      controlled_system.Simulate(u0);
    }

    x0 = controlled_system.x();

    // save the signals
    y_ref.col(t) = yr.col(end_idx);
    y_true.col(t) = controlled_system.y();
    u.col(t) = u0;
    J.col(t).fill(*j);
  }

  auto t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<data_t, std::milli> sim_time_ms = t2 - t1;
  cout << "Finished simulation in " << sim_time_ms.count() << " ms.\n";
  cout << "(app. " << (sim_time_ms.count() / n_t) * 1e3 << " µs/time-step)\n";

  cout << "Saving simulation data to disk.\n";

  // saving with hdf5 via armadillo
  arma::hdf5_opts::opts replace = arma::hdf5_opts::replace;

  auto dt_vec = Vector(1).fill(dt);
  dt_vec.save(arma::hdf5_name("eg_lqmpc_ctrl.h5", "dt"));
  y_ref.save(arma::hdf5_name("eg_lqmpc_ctrl.h5", "y_ref", replace));
  y_true.save(arma::hdf5_name("eg_lqmpc_ctrl.h5", "y_true", replace));
  u.save(arma::hdf5_name("eg_lqmpc_ctrl.h5", "u", replace));
  J.save(arma::hdf5_name("eg_lqmpc_ctrl.h5", "j", replace));

  cout << "fin.\n";
  return 0;
}