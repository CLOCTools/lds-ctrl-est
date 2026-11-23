//===-- tests/test_plds_sys.cpp - PLDS System Unit Tests --------*- C++ -*-===//
//
// Copyright 2026 CLOCTools Contributors
// Licensed under the Apache License, Version 2.0
//
//===----------------------------------------------------------------------===//

#include <doctest.h>
#include <ldsCtrlEst>

#include <cmath>
#include <random>

using lds::data_t;
using lds::Matrix;
using lds::Vector;

TEST_SUITE("plds_sys") {
  TEST_CASE("Output function h() and stateless h_() follow exp(C*x + d)") {
    lds::poisson::System sys(1, 2, 2, 1e-3);
    Matrix C = {{1.0, 0.5}, {-0.5, 1.0}};
    Vector d = {std::log(0.01), std::log(0.02)};
    Vector x0 = {0.4, -0.2};

    sys.set_C(C);
    sys.set_d(d);
    sys.set_x0(x0);
    sys.Reset();

    Vector expected_y = arma::exp(C * x0 + d);
    CHECK(arma::approx_equal(sys.y(), expected_y, "absdiff", 1e-12));
    CHECK(arma::approx_equal(sys.cx(), C * x0, "absdiff", 1e-12));
    CHECK(arma::all(sys.y() > 0.0));

    Vector x1 = {-0.2, 0.6};
    sys.set_x(x1);
    CHECK(arma::approx_equal(sys.y(), arma::exp(C * x1 + d), "absdiff", 1e-12));
  }

  TEST_CASE("Simulate emits non-negative integer spike counts") {
    lds::poisson::System sys(1, 1, 2, 1e-3);
    sys.set_A(Matrix(1, 1).fill(0.95));
    sys.set_B(Matrix(1, 1).fill(0.05));
    sys.set_C(Matrix(2, 1).fill(1.0));
    sys.set_d(Vector(2).fill(std::log(25.0 * 1e-3)));
    sys.Reset();

    Vector u = {1.0};
    for (int t = 0; t < 25; ++t) {
      const Vector& z = sys.Simulate(u);
      REQUIRE(z.n_elem == 2);
      for (size_t k = 0; k < z.n_elem; ++k) {
        CHECK(std::isfinite(z[k]));
        CHECK(z[k] >= 0.0);
        CHECK(std::floor(z[k]) == z[k]);
      }
    }
  }

  TEST_CASE("Laplace Poisson Filter tracks rate shifts, adapts m, and preserves SPD covariance") {
    const data_t dt = 1e-3;
    lds::poisson::System sys(1, 1, 1, dt, 1e-3, 1e-4);
    sys.set_A(Matrix(1, 1).fill(0.98));
    sys.set_B(Matrix(1, 1).fill(0.02));
    sys.set_C(Matrix(1, 1).fill(1.0));
    sys.set_d(Vector(1).fill(std::log(10.0 * dt)));
    sys.set_Q_m(Matrix(1, 1).fill(1e-4));
    sys.do_adapt_m = true;
    sys.Reset();

    // Generate reproducible Poisson spike counts at elevated target firing rate (50 Hz * dt = 0.05/ms)
    std::mt19937 gen(2026);
    std::poisson_distribution<int> pois(0.05);
    Vector u = {0.0};

    for (int t = 0; t < 500; ++t) {
      Vector z(1);
      z[0] = static_cast<data_t>(pois(gen));
      sys.Filter(u, z);
      CHECK(std::isfinite(sys.x()[0]));
      CHECK(std::isfinite(sys.y()[0]));
    }

    CHECK(sys.P().is_sympd());
    CHECK(sys.y()[0] > 0.02);  // adapted upward from baseline 0.01 toward ~0.05
    CHECK(sys.m()[0] > 0.0);

    sys.Reset();
    CHECK(arma::approx_equal(sys.x(), sys.x0(), "absdiff", 1e-12));
    CHECK(arma::approx_equal(sys.m(), sys.m0(), "absdiff", 1e-12));
  }

  TEST_CASE("Poisson FitEM handles unequal trial lengths when trial 0 is longer than trial 1") {
    const data_t dt = 1e-3;
    const size_t n_u = 1;
    const size_t n_x = 2;
    const size_t n_y = 2;

    arma::arma_rng::set_seed(1234);
    Matrix u0(n_u, 15, arma::fill::randn);
    Matrix u1(n_u, 6, arma::fill::randn);
    Matrix z0 = arma::randi<Matrix>(n_y, 15, arma::distr_param(0, 3));
    Matrix z1 = arma::randi<Matrix>(n_y, 6, arma::distr_param(0, 3));

    lds::UniformMatrixList<lds::kMatFreeDim2> u_list({u0, u1});
    lds::UniformMatrixList<lds::kMatFreeDim2> z_list({z0, z1});

    lds::poisson::FitEM fit_em(n_x, dt, std::move(u_list), std::move(z_list));
    const lds::poisson::Fit* fit_ptr = nullptr;
    REQUIRE_NOTHROW(fit_ptr = &fit_em.Run(true, true, true, true, true, 2, 1e-2));
    REQUIRE(fit_ptr != nullptr);
    CHECK(fit_em.x().size() == 2);
    CHECK(fit_em.x()[0].n_cols == 15);
    CHECK(fit_em.x()[1].n_cols == 6);
    CHECK(fit_ptr->C().is_finite());
    CHECK(fit_ptr->d().is_finite());
  }
}
