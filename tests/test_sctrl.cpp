//===-- tests/test_sctrl.cpp - Switched Controller Unit Tests ---*- C++ -*-===//
//
// Copyright 2026 CLOCTools Contributors
// Licensed under the Apache License, Version 2.0
//
//===----------------------------------------------------------------------===//

#include <doctest.h>
#include <ldsCtrlEst>

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

using lds::data_t;
using lds::Matrix;
using lds::Vector;

TEST_SUITE("switched_controller") {
  TEST_CASE("GLDS SwitchedController switches gains, preserves state/disturbance, and validates index") {
    const data_t dt = 1e-3;
    lds::gaussian::System s0(1, 1, 1, dt, 1e-4, 1e-5, 1e-5);
    s0.set_A(Matrix(1, 1).fill(0.95));
    s0.set_B(Matrix(1, 1).fill(0.10));
    s0.set_C(Matrix(1, 1).fill(1.0));
    s0.do_adapt_m = true;
    s0.Reset();

    lds::gaussian::System s1 = s0;
    s1.set_B(Matrix(1, 1).fill(0.05));

    lds::UniformSystemList<lds::gaussian::System> sys_list({s0, s1});
    lds::gaussian::SwitchedController sctrl(std::move(sys_list), -5.0, 5.0, lds::kControlTypeIntY);

    Matrix kc0 = Matrix(1, 1).fill(1.0);
    Matrix kc1 = Matrix(1, 1).fill(2.0);
    Matrix kci0 = Matrix(1, 1).fill(4.0);
    Matrix kci1 = Matrix(1, 1).fill(8.0);
    sctrl.set_Kc(lds::UniformMatrixList<>({kc0, kc1}));
    sctrl.set_Kc_inty(lds::UniformMatrixList<>({kci0, kci1}));
    sctrl.set_y_ref(Vector(1).fill(1.0));

    // Step forward under mode 0
    for (int t = 0; t < 20; ++t) {
      sctrl.ControlOutputReference(Vector(1).fill(0.25));
    }
    const Vector x_before = sctrl.sys().x();
    const Vector m_before = sctrl.sys().m();

    // No-op switch to current mode
    sctrl.Switch(0, false);
    CHECK(arma::approx_equal(sctrl.Kc(), kc0, "absdiff", 1e-12));

    // Switch to mode 1: swaps active gains and carries state/disturbance continuity
    sctrl.Switch(1);
    CHECK(arma::approx_equal(sctrl.Kc(), kc1, "absdiff", 1e-12));
    CHECK(arma::approx_equal(sctrl.Kc_inty(), kci1, "absdiff", 1e-12));
    CHECK(arma::approx_equal(sctrl.sys().x(), x_before, "absdiff", 1e-12));
    CHECK(arma::approx_equal(sctrl.sys().m(), m_before, "absdiff", 1e-12));
    CHECK(sctrl.sys().B()(0, 0) == doctest::Approx(0.05));

    // Switch back to mode 0
    sctrl.Switch(0);
    CHECK(arma::approx_equal(sctrl.Kc(), kc0, "absdiff", 1e-12));
    CHECK(sctrl.sys().B()(0, 0) == doctest::Approx(0.10));

    // Out-of-bounds mode throws std::out_of_range
    CHECK_THROWS_AS(sctrl.Switch(99), std::out_of_range);
  }

  TEST_CASE("PLDS SwitchedController regulates across mode transitions within bounds") {
    const data_t dt = 1e-3;
    lds::poisson::System p0(1, 1, 1, dt, 1e-4, 1e-4);
    p0.set_A(Matrix(1, 1).fill(0.985));
    p0.set_B(Matrix(1, 1).fill(0.05));
    p0.set_C(Matrix(1, 1).fill(1.0));
    p0.set_d(Vector(1).fill(std::log(1.0 * dt)));
    p0.Reset();

    lds::poisson::System p1 = p0;
    p1.set_B(Matrix(1, 1).fill(0.025));

    lds::poisson::SwitchedController p_sctrl(
        lds::UniformSystemList<lds::poisson::System>({p0, p1}), 0.0, 5.0, 0);
    p_sctrl.set_Kc(lds::UniformMatrixList<>({Matrix(1, 1).fill(1.0), Matrix(1, 1).fill(2.0)}));
    p_sctrl.set_y_ref(Vector(1).fill(25.0 * dt));

    for (int t = 0; t < 40; ++t) {
      if (t == 20) {
        p_sctrl.Switch(1);
      }
      Vector u = p_sctrl.ControlOutputReference(Vector(1).fill(0.0));
      CHECK(std::isfinite(u[0]));
      CHECK(u[0] >= 0.0);
      CHECK(u[0] <= 5.0);
    }
    CHECK(p_sctrl.sys().B()(0, 0) == doctest::Approx(0.025));
  }

  TEST_CASE("SwitchedController with kControlTypeDeltaU and n_u != n_y initializes and switches Kc_u_list_") {
    const data_t dt = 1e-3;
    lds::gaussian::System s0(1, 2, 2, dt, 1e-4, 1e-5, 1e-5);
    s0.set_A(0.95 * Matrix(2, 2, arma::fill::eye));
    Matrix B0(2, 1, arma::fill::zeros);
    B0(0, 0) = 0.10;
    B0(1, 0) = 0.05;
    s0.set_B(B0);
    s0.set_C(Matrix(2, 2, arma::fill::eye));
    s0.Reset();

    lds::gaussian::System s1 = s0;
    Matrix B1(2, 1, arma::fill::zeros);
    B1(0, 0) = 0.05;
    B1(1, 0) = 0.025;
    s1.set_B(B1);

    lds::UniformSystemList<lds::gaussian::System> sys_list({s0, s1});
    lds::gaussian::SwitchedController sctrl(std::move(sys_list), -5.0, 5.0, lds::kControlTypeDeltaU);

    Matrix kc0(1, 2, arma::fill::ones);
    Matrix kc1(1, 2, arma::fill::ones);
    sctrl.set_Kc(lds::UniformMatrixList<>({kc0, kc1}));
    sctrl.set_y_ref(Vector({1.0, 0.5}));

    // Switch() must swap Kc_u_list_ (shape n_u x n_u = 1x1) without dimension
    // mismatch against Kc_inty_ (shape 0x0 when kControlTypeIntY is off).
    std::ostringstream cerr_capture;
    std::streambuf* old_cerr = std::cerr.rdbuf(cerr_capture.rdbuf());
    sctrl.Switch(1);
    std::cerr.rdbuf(old_cerr);
    CHECK(cerr_capture.str().empty());
    CHECK(sctrl.Kc_u().n_rows == 1);
    CHECK(sctrl.Kc_u().n_cols == 1);

    Vector u = sctrl.ControlOutputReference(Vector({0.1, 0.1}));
    CHECK(u.n_elem == 1);
    CHECK(std::isfinite(u[0]));
  }
}
