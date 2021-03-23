//===-- main.cpp - Example ldsCtrlEst Usage -------------------------------===//
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
///
/// \brief Example ldsCtrlEst Usage
///
//===----------------------------------------------------------------------===//

#include <ldsCtrlEst>
#include <iostream>

auto main() -> int {
  std::cout << "Testing ldsCtrlEst pkg-config installation...\n";

  double dt = 1e-3;
  size_t n_u = 1;
  size_t n_x = 2;
  size_t n_y = 2;
  auto sys = lds::gaussian::System(n_u, n_x, n_y, dt);

  std::cout << "Example GLDS::sys_t = \n";
  sys.Print();

  // perform one step of filtering
  lds::Vector z_t = lds::Vector(n_y).fill(arma::fill::randn);
  lds::Vector u_tm1 = lds::Vector(n_u).fill(arma::fill::randn);
  sys.Filter(u_tm1, z_t);
  std::cout << "Example after one step of filtering... \n";
  sys.Print();

  std::cout << "fin.\n";
  return 0;
}
