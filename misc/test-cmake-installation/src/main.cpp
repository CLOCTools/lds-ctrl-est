#include <ldsCtrlEst>
#include <iostream>

auto main() -> int {
  std::cout << "Testing ldsCtrlEst CMake installation...\n";

  double dt = 1e-3;
  size_t n_u = 1;
  size_t n_x = 2;
  size_t n_y = 2;
  auto sys = glds::sys_t(n_u, n_x, n_y, dt);

  std::cout << "Example GLDS::sys_t = \n";
  sys.printSys();

  // // generate a random square mat and test inversion.
  // // std::cout << "Inverted a square matrix using dependency armadillo.\n";
  // auto a = arma::mat(5,5).fill(arma::fill::eye);
  // auto a_inv = arma::inv(a);
  // std::cout << "a^-1 = \n" << a_inv << "\n";

  std::cout << "Example after one step of prediction/filtering... \n";
  lds::armaVec z = arma::vec(n_y).fill(arma::fill::randn);
  sys.simPredict();
  sys.filter(z);
  sys.printSys();

  std::cout << "fin.\n";
  return 0;
}
