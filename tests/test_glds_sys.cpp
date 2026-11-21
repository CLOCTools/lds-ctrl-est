//===-- tests/test_glds_sys.cpp - GLDS System Unit Tests --------*- C++ -*-===//
//
// Copyright 2026 CLOCTools Contributors
// Licensed under the Apache License, Version 2.0
//
//===----------------------------------------------------------------------===//

#include <doctest.h>
#include <ldsCtrlEst>

#include <cmath>
#include <stdexcept>
#include <vector>

using lds::data_t;
using lds::Matrix;
using lds::Vector;

TEST_SUITE("glds_sys") {
  TEST_CASE("Construction, accessors, and dimension validation") {
    lds::gaussian::System sys(2, 3, 2, 1e-3, 1e-4, 1e-5, 1e-2);
    CHECK(sys.n_u() == 2);
    CHECK(sys.n_x() == 3);
    CHECK(sys.n_y() == 2);
    CHECK(sys.dt() == doctest::Approx(1e-3));

    CHECK(arma::approx_equal(sys.R(), 1e-2 * Matrix(2, 2, arma::fill::eye), "absdiff", 1e-12));

    CHECK_THROWS_AS(sys.set_A(Matrix(2, 2, arma::fill::eye)), std::runtime_error);
    CHECK_THROWS_AS(sys.set_B(Matrix(3, 1, arma::fill::zeros)), std::runtime_error);
    CHECK_THROWS_AS(sys.set_C(Matrix(1, 3, arma::fill::zeros)), std::runtime_error);
    CHECK_THROWS_AS(sys.set_R(Matrix(3, 3, arma::fill::eye)), std::runtime_error);
    CHECK_THROWS_AS(sys.set_d(Vector(3, arma::fill::zeros)), std::runtime_error);
  }

  TEST_CASE("Deterministic dynamics f() and output h() / h_() match analytic equations") {
    lds::gaussian::System sys(1, 2, 2, 1e-3);
    Matrix A = {{0.9, 0.1}, {0.0, 0.8}};
    Matrix B(2, 1);
    B(0, 0) = 0.5;
    B(1, 0) = 0.25;
    Vector g = {2.0};
    Vector m = {0.1, -0.05};
    Matrix C = {{1.0, 2.0}, {-0.5, 1.0}};
    Vector d = {0.2, -0.1};
    Vector x0 = {1.0, -2.0};

    sys.set_A(A);
    sys.set_B(B);
    sys.set_g(g);
    sys.set_m(m, true);
    sys.set_C(C);
    sys.set_d(d);
    sys.set_x0(x0);
    sys.Reset();

    CHECK(arma::approx_equal(sys.x(), x0, "absdiff", 1e-12));
    CHECK(arma::approx_equal(sys.y(), C * x0 + d, "absdiff", 1e-12));

    Vector u = {1.5};
    sys.f(u, false);
    Vector expected_x1 = A * x0 + B * (g % u) + m;
    CHECK(arma::approx_equal(sys.x(), expected_x1, "absdiff", 1e-12));

    sys.set_x(expected_x1);
    Vector expected_y1 = C * expected_x1 + d;
    CHECK(arma::approx_equal(sys.y(), expected_y1, "absdiff", 1e-12));
    CHECK(arma::approx_equal(sys.cx(), C * expected_x1, "absdiff", 1e-12));

    // Reset restores x0 and m0
    sys.Reset();
    CHECK(arma::approx_equal(sys.x(), x0, "absdiff", 1e-12));
    CHECK(arma::approx_equal(sys.y(), C * x0 + d, "absdiff", 1e-12));
  }

  TEST_CASE("Kalman Filter tracks ground-truth state and adaptive disturbance m") {
    const size_t n_u = 1;
    const size_t n_x = 1;
    const size_t n_y = 1;
    const data_t dt = 1e-3;

    // Plant with unmodeled constant process disturbance m_true = 0.02
    lds::gaussian::System plant(n_u, n_x, n_y, dt);
    plant.set_A(Matrix(1, 1).fill(0.95));
    plant.set_B(Matrix(1, 1).fill(0.10));
    plant.set_C(Matrix(1, 1).fill(1.0));
    plant.set_m(Vector(1).fill(0.02), true);
    plant.Reset();

    // Estimator starts with wrong x0 and zero initial disturbance m0 = 0
    lds::gaussian::System est(n_u, n_x, n_y, dt, 1e-2, 1e-5, 1e-4);
    est.set_A(Matrix(1, 1).fill(0.95));
    est.set_B(Matrix(1, 1).fill(0.10));
    est.set_C(Matrix(1, 1).fill(1.0));
    est.set_Q_m(Matrix(1, 1).fill(1e-4));
    est.set_x0(Vector(1).fill(0.0));
    est.do_adapt_m = true;
    est.Reset();

    Vector u = {0.5};
    for (int t = 0; t < 250; ++t) {
      plant.f(u, false);
      const Vector z_t = plant.C() * plant.x() + plant.d();
      est.Filter(u, z_t);
    }

    CHECK(est.P().is_sympd());
    CHECK(std::abs(est.x()[0] - plant.x()[0]) < 2e-3);
    CHECK(std::abs(est.m()[0] - 0.02) < 2e-3);
  }

  TEST_CASE("Manual Ke gain disables automatic covariance recursion and nstep_pred_block runs") {
    lds::gaussian::System sys(1, 1, 1, 1e-3);
    sys.set_A(Matrix(1, 1).fill(0.9));
    sys.set_B(Matrix(1, 1).fill(0.1));
    Matrix manual_ke(1, 1);
    manual_ke.fill(0.4);
    sys.set_Ke(manual_ke);
    sys.Reset();

    const Matrix P_before = sys.P();
    sys.Filter(Vector(1).fill(1.0), Vector(1).fill(2.0));
    CHECK(arma::approx_equal(sys.Ke(), manual_ke, "absdiff", 1e-12));
    CHECK(arma::approx_equal(sys.P(), P_before, "absdiff", 1e-12));

    // nstep_pred_block returns {x_filt, x_pred, y_pred} across trials
    lds::UniformMatrixList<lds::kMatFreeDim2> u_list({Matrix(1, 10, arma::fill::ones)});
    lds::UniformMatrixList<lds::kMatFreeDim2> z_list({Matrix(1, 10, arma::fill::ones)});
    auto res = sys.nstep_pred_block(u_list, z_list, 2);
    REQUIRE(res.size() == 3);
    CHECK(res[0].at(0).n_cols == 10);
    CHECK(res[1].at(0).n_cols == 8);
    CHECK(res[2].at(0).n_cols == 8);
  }
}
