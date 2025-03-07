---
title: eg_plds_est.cpp
summary: Example PLDS Estimation. 

---

# eg_plds_est.cpp



Example PLDS Estimation ```cpp

//===-- eg_plds_est.cpp - Example PLDS Estimation -------------------------===//
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

using lds::Matrix;
using lds::Vector;
using lds::data_t;
using std::cout;

// for generating random input
Matrix random_walk(size_t n_t, const Matrix& Q, const Vector& x0);

int main() {
  cout << " ********** Example Poisson LDS Estimation ********** \n\n";

  // Make SISO system sampled at 1kHz
  data_t dt = 1e-3;
  size_t n_u = 1;                           // no. inputs
  size_t n_x = 1;                           // no. states
  size_t n_y = 1;                           // no. outputs
  auto n_t = static_cast<size_t>(30 / dt);  // no time steps for simulation.

  // construct ground truth system...
  lds::poisson::System system_true(n_u, n_x, n_y, dt);

  // Model parameters
  Matrix a_true(n_x, n_x, arma::fill::eye);
  a_true[0] = exp(-dt / 0.075);
  Matrix b_true = Matrix(n_x, n_u).fill(1e-2);
  Vector m0_true = Vector(n_x, arma::fill::zeros).fill(-7e-2);  // disturbance
  Vector x0_true = m0_true * arma::inv(Matrix(n_x, n_x, arma::fill::eye) -
                                        a_true);  // initial state

  // Assign params.
  system_true.set_A(a_true);
  system_true.set_B(b_true);
  system_true.set_x0(x0_true);
  system_true.set_m(m0_true);
  system_true.Reset();

  // Construct system for estimation
  // e.g., will create a model with incorrect disturbance
  lds::poisson::System system_estimator(n_u, n_x, n_y, dt);

  // Can copy parameters from another system object
  system_estimator = system_true;

  // wrong disturbance
  Vector m0_est = m0_true * 2;
  system_estimator.set_m(m0_est);

  // set new initial conditions
  Vector x0_est = m0_est * arma::inv(Matrix(n_x, n_x, arma::fill::eye) -
                                      a_true);  // initial state
  system_estimator.set_x0(x0_est);
  system_estimator.Reset();  // reset to initial condition.

  // turn on adaptive disturbance estimation
  system_estimator.do_adapt_m = true;

  // set adaptation rate by changing covariance of assumed process noise acting
  // on random-walk evolution of m
  Matrix q_m = Matrix(n_x, n_x, arma::fill::eye) * 1e-6;
  system_estimator.set_Q_m(q_m);

  cout << ".....................................\n";
  cout << "estimator:\n";
  cout << ".....................................\n";
  system_estimator.Print();
  cout << ".....................................\n";

  // Set up simulation :
  // Simulated measurements
  Matrix z(n_y, n_t, arma::fill::zeros);

  // Stimulus (generate random stimulus)
  Matrix q_u =
      Matrix(n_u, n_u, arma::fill::eye) * 1e-3;  // cov of random walk
  Matrix u = random_walk(n_t, q_u, Vector(n_u, arma::fill::zeros));

  // create matrix to save outputs in...
  Matrix y_hat(n_y, n_t, arma::fill::zeros);
  Matrix y_true(n_y, n_t, arma::fill::zeros);

  // states and disturbance params
  Matrix x_hat(n_x, n_t, arma::fill::zeros);
  Matrix m_hat(n_x, n_t, arma::fill::zeros);

  Matrix x_true(n_x, n_t, arma::fill::zeros);
  Matrix m_true(n_x, n_t, arma::fill::zeros);

  // initial conditions
  y_hat.col(0) = system_estimator.y();
  y_true.col(0) = system_true.y();
  x_hat.col(0) = system_estimator.x();
  x_true.col(0) = system_true.x();
  m_hat.col(0) = system_estimator.m();
  m_true.col(0) = system_true.m();

  cout << "Starting " << n_t * dt << " sec simlation ... \n";
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t t = 1; t < n_t; t++) {
    // Simlate the true system.
    z.col(t) = system_true.Simulate(u.col(t - 1));

    // Filter (predict -> update)
    system_estimator.Filter(u.col(t - 1), z.col(t));

    // save signals
    y_hat.col(t) = system_estimator.y();
    y_true.col(t) = system_true.y();

    x_true.col(t) = system_true.x();
    m_true.col(t) = system_true.m();

    x_hat.col(t) = system_estimator.x();
    m_hat.col(t) = system_estimator.m();
  }

  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<data_t, std::milli> sim_time_ms = finish - start;
  cout << "Finished simlation in " << sim_time_ms.count() << " ms.\n";
  cout << "(app. " << (sim_time_ms.count() / n_t) * 1e3 << " us/time-step)\n";

  // saved variables: dt, y_hat, x_hat, m_hat, z, u, y_true,
  // x_true, m_true saving with hdf5 via armadillo
  arma::hdf5_opts::opts replace = arma::hdf5_opts::replace;

  auto dt_vec = Vector(1).fill(dt);
  dt_vec.save(arma::hdf5_name("eg_plds_est.h5", "dt"));
  u.save(arma::hdf5_name("eg_plds_est.h5", "u", replace));
  z.save(arma::hdf5_name("eg_plds_est.h5", "z", replace));
  x_true.save(arma::hdf5_name("eg_plds_est.h5", "x_true", replace));
  m_true.save(arma::hdf5_name("eg_plds_est.h5", "m_true", replace));
  y_true.save(arma::hdf5_name("eg_plds_est.h5", "y_true", replace));
  x_hat.save(arma::hdf5_name("eg_plds_est.h5", "x_hat", replace));
  m_hat.save(arma::hdf5_name("eg_plds_est.h5", "m_hat", replace));
  y_hat.save(arma::hdf5_name("eg_plds_est.h5", "y_hat", replace));

  return 0;
}

// for generating random input
Matrix random_walk(size_t n_t, const Matrix& Q, const Vector& x0) {
  size_t n = Q.n_rows;

  if ((n != Q.n_cols) || (Q.n_cols != Q.n_rows)) {
    throw std::logic_error("Q must be `n` x `n`.");
  }

  Matrix x(n, n_t, arma::fill::zeros);
  x.col(0) = x0;
  for (size_t t = 1; t < n_t; t++) {
    x.col(t) = x.col(t - 1) + arma::mvnrnd(Vector(n, arma::fill::zeros), Q);
  }

  return x;
}
```

_Filename: eg_plds_est.cpp_

-------------------------------

Updated on  5 March 2025 at 21:41:27 EST
