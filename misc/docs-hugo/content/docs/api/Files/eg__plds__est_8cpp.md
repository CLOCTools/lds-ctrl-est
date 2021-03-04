---
title: examples/eg_plds_est.cpp

---

# examples/eg_plds_est.cpp



## Functions

|                | Name           |
| -------------- | -------------- |
| armaMat | **[random_walk](/ldsctrlest/docs/api/files/eg__plds__est_8cpp/#function-random_walk)**(size_t n_t, const arma::mat & Q, arma::vec x0) |
| int | **[main](/ldsctrlest/docs/api/files/eg__plds__est_8cpp/#function-main)**(void ) |


## Function Details

### random_walk

```cpp
armaMat random_walk(
    size_t n_t,
    const arma::mat & Q,
    arma::vec x0
)
```



### main

```cpp
int main(
    void 
)
```






## Source code

```cpp
//===-- eg_plds_est.cpp - Example PLDS Estimation -------------------------===//
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
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>

using lds::armaMat;
using lds::armaVec;
using lds::data_t;
using std::cout;
using std::vector;

// for generating random input
armaMat random_walk(size_t n_t, const arma::mat& Q, arma::vec x0);

int main(void) {
  cout << " ********** Example Poisson LDS Estimation ********** \n\n";

  // Make SISO system sampled at 1kHz
  data_t dt = 1e-3;
  size_t n_u = 1;                             // no. inputs
  size_t n_x = 1;                             // no. states
  size_t n_y = 1;                             // no. outputs
  size_t n_t = static_cast<size_t>(30 / dt);  // no time steps for simulation.

  // construct ground truth system...
  lds::poisson::sys_t system_true(n_u, n_x, n_y, dt);

  // Model parameters
  armaMat a_true(n_x, n_x, arma::fill::eye);
  a_true[0] = exp(-dt / 0.075);
  armaMat b_true = armaMat(n_x, n_u).fill(1e-2);
  armaVec m0_true = armaVec(n_x, arma::fill::zeros).fill(-7e-2);  // disturbance
  armaVec x0_true = m0_true * arma::inv(armaMat(n_x, n_x, arma::fill::eye) -
                                        a_true);  // initial state

  // Assign params.
  system_true.setA(a_true);
  system_true.setB(b_true);
  system_true.setX0(x0_true);
  system_true.setM(m0_true);
  system_true.reset();

  // Construct system for estimation
  // e.g., will create a model with incorrect disturbance
  lds::poisson::sys_t system_estimator(n_u, n_x, n_y, dt);

  // Can copy parameters from another system object
  system_estimator = system_true;

  // wrong disturbance
  armaVec m0_est = m0_true * 2;
  system_estimator.setM(m0_est);

  // set new initial conditions
  armaVec x0_est = m0_est * arma::inv(armaMat(n_x, n_x, arma::fill::eye) -
                                      a_true);  // initial state
  system_estimator.setX0(x0_est);
  system_estimator.reset();  // reset to initial condition.

  // turn on adaptive disturbance estimation
  system_estimator.adaptM = true;

  // set adaptation rate by changing covariance of assumed process noise acting
  // on random-walk evolution of m
  armaMat q_m = armaMat(n_x, n_x, arma::fill::eye) * 1e-6;
  system_estimator.setQ_m(q_m);

  cout << ".....................................\n";
  cout << "estimator:\n";
  cout << ".....................................\n";
  system_estimator.printSys();
  cout << ".....................................\n";

  // Set up simulation :

  // Simulated measurements
  armaMat z(n_y, n_t, arma::fill::zeros);

  // Stimulus (generate random stimulus)
  armaMat q_u =
      armaMat(n_u, n_u, arma::fill::eye) * 1e-3;  // cov of random walk
  armaMat u = random_walk(n_t, q_u, armaVec(n_u, arma::fill::zeros));

  // create matrix to save outputs in...
  armaMat y_hat(n_y, n_t, arma::fill::zeros);
  armaMat y_true(n_y, n_t, arma::fill::zeros);

  // states and disturbance params
  armaMat x_hat(n_x, n_t, arma::fill::zeros);
  armaMat m_hat(n_x, n_t, arma::fill::zeros);

  armaMat x_true(n_x, n_t, arma::fill::zeros);
  armaMat m_true(n_y, n_t, arma::fill::zeros);

  cout << "Starting " << n_t * dt << " sec simlation ... \n";
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t t = 0; t < n_t; t++) {
    // input
    armaVec u_k(u.colptr(t), u.n_rows, false, true);

    // Simlate the true system.
    system_true.setU(u_k);
    system_true.simPredict();

    // generate a measurement
    armaVec z_k(z.colptr(t), z.n_rows, false, true);
    system_true.simMeasurement(z_k);

    // filter (predict -> update)
    system_estimator.filter(z_k);

    // save signals
    y_hat.col(t) = system_estimator.getY();
    y_true.col(t) = system_true.getY();

    x_true.col(t) = system_true.getX();
    m_true.col(t) = system_true.getM();

    x_hat.col(t) = system_estimator.getX();
    m_hat.col(t) = system_estimator.getM();

    // for next time.
    system_estimator.setU(u_k);
  }

  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<data_t, std::milli> simTime_ms = finish - start;
  cout << "Finished simlation in " << simTime_ms.count() << " ms.\n";
  cout << "(app. " << (simTime_ms.count() / n_t) * 1e3 << " us/time-step)\n";

  // saved variables: dt, y_hat, x_hat, m_hat, z, u, y_true,
  // x_true, m_true saving with hdf5 via armadillo
  arma::hdf5_opts::opts replace = arma::hdf5_opts::replace;

  auto dt_vec = arma::vec(1).fill(dt);
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
armaMat random_walk(size_t n_t, const arma::mat& Q, arma::vec x0) {
  size_t n = Q.n_rows;

  if ((n != Q.n_cols) || (Q.n_cols != Q.n_rows)) {
    throw std::logic_error("Q must be `n` x `n`.");
  }

  arma::mat x(n, n_t, arma::fill::zeros);
  x.col(0) = x0;
  for (size_t t = 1; t < n_t; t++) {
    x.col(t) = x.col(t - 1) + arma::mvnrnd(arma::vec(n, arma::fill::zeros), Q);
  }

  return x;
}
```


-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
