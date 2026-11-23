//===-- tests/test_ctrl.cpp - Controller Unit Tests -------------*- C++ -*-===//
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

namespace {

lds::gaussian::System MakeScalarGLDS(data_t a, data_t b, data_t dt = 1e-3) {
  lds::gaussian::System sys(1, 1, 1, dt, 1e-4, 1e-5, 1e-5);
  sys.set_A(Matrix(1, 1).fill(a));
  sys.set_B(Matrix(1, 1).fill(b));
  sys.set_C(Matrix(1, 1).fill(1.0));
  sys.set_g(Vector(1).fill(1.0));
  sys.Reset();
  return sys;
}

}  // namespace

TEST_SUITE("controller") {
  TEST_CASE("GLDS Controller enforces actuator bounds [u_lb, u_ub] and tracks y_ref") {
    lds::gaussian::System plant = MakeScalarGLDS(0.95, 0.10);
    lds::gaussian::System model = MakeScalarGLDS(0.95, 0.10);

    const data_t u_lb = -1.5;
    const data_t u_ub = 3.0;
    lds::gaussian::Controller ctrl(model, u_lb, u_ub, 0);
    ctrl.set_Kc(Matrix(1, 1).fill(2.5));

    // Step 1: extreme target forces upper saturation clamp u <= u_ub
    ctrl.set_y_ref(Vector(1).fill(50.0));
    Vector u = ctrl.ControlOutputReference(Vector(1).fill(0.0));
    CHECK(u[0] == doctest::Approx(u_ub));

    // Step 2: negative extreme forces lower saturation clamp u >= u_lb
    ctrl.set_y_ref(Vector(1).fill(-50.0));
    u = ctrl.ControlOutputReference(Vector(1).fill(0.0));
    CHECK(u[0] >= u_lb - 1e-12);
    CHECK(u[0] <= u_ub + 1e-12);
    CHECK(u[0] == doctest::Approx(u_lb));

    // Closed-loop tracking of reachable target y_ref = 1.0
    plant.Reset();
    ctrl.Reset();
    ctrl.set_y_ref(Vector(1).fill(1.0));
    for (int t = 0; t < 300; ++t) {
      plant.f(u, false);
      const Vector z_t = plant.C() * plant.x() + plant.d();
      u = ctrl.ControlOutputReference(z_t);
      CHECK(u[0] >= u_lb - 1e-12);
      CHECK(u[0] <= u_ub + 1e-12);
    }
    CHECK(std::abs(plant.x()[0] - 1.0) < 1e-3);
  }

  TEST_CASE("GLDS integral action eliminates steady-state offset under gain mismatch") {
    auto run_loop = [](bool with_integral) -> data_t {
      lds::gaussian::System plant = MakeScalarGLDS(0.95, 0.25);  // true B = 0.25
      lds::gaussian::System model(1, 1, 1, 1e-3, 1e-4, 1e-4, 1e-8);  // high Ke so x_hat tracks z
      model.set_A(Matrix(1, 1).fill(0.95));
      model.set_B(Matrix(1, 1).fill(0.10));  // mismatched B = 0.10
      model.set_C(Matrix(1, 1).fill(1.0));
      model.set_g(Vector(1).fill(1.0));
      model.Reset();

      size_t ctype = with_integral ? lds::kControlTypeIntY : 0;
      lds::gaussian::Controller ctrl(model, -10.0, 10.0, ctype);
      ctrl.set_Kc(Matrix(1, 1).fill(1.5));
      if (with_integral) {
        ctrl.set_Kc_inty(Matrix(1, 1).fill(100.0));
      }
      ctrl.set_y_ref(Vector(1).fill(1.0));

      Vector u(1, arma::fill::zeros);
      for (int t = 0; t < 1500; ++t) {
        plant.f(u, false);
        const Vector z_t = plant.C() * plant.x() + plant.d();
        u = ctrl.ControlOutputReference(z_t);
      }
      return std::abs(plant.x()[0] - 1.0);
    };

    const data_t err_no_int = run_loop(false);
    const data_t err_with_int = run_loop(true);
    CHECK(err_no_int > 0.05);
    CHECK(err_with_int < 1e-3);
  }

  TEST_CASE("Anti-windup with tau_awu bounds recovery after prolonged saturation") {
    auto simulate_recovery = [](data_t tau_awu) -> int {
      lds::gaussian::System plant = MakeScalarGLDS(0.95, 0.05);
      lds::gaussian::System model = MakeScalarGLDS(0.95, 0.05);
      lds::gaussian::Controller ctrl(model, 0.0, 1.0, lds::kControlTypeIntY);
      ctrl.set_Kc(Matrix(1, 1).fill(2.0));
      ctrl.set_Kc_inty(Matrix(1, 1).fill(20.0));
      ctrl.set_tau_awu(tau_awu);

      // Drive unreachable target so u stays saturated at u_ub = 1.0 (true ss ~ 1.0)
      ctrl.set_y_ref(Vector(1).fill(5.0));
      Vector u(1, arma::fill::zeros);
      for (int t = 0; t < 300; ++t) {
        plant.f(u, false);
        const Vector z_t = plant.C() * plant.x() + plant.d();
        u = ctrl.ControlOutputReference(z_t);
      }
      CHECK(u[0] == doctest::Approx(1.0));

      // Drop target to reachable y_ref = 0.2 and measure steps until output <= 0.3
      ctrl.set_y_ref(Vector(1).fill(0.2));
      for (int t = 0; t < 500; ++t) {
        plant.f(u, false);
        const Vector z_t = plant.C() * plant.x() + plant.d();
        u = ctrl.ControlOutputReference(z_t);
        if (plant.x()[0] <= 0.3) {
          return t;
        }
      }
      return 500;
    };

    const int steps_no_awu = simulate_recovery(lds::kInf);
    const int steps_with_awu = simulate_recovery(0.01);
    CHECK(steps_with_awu < steps_no_awu);
  }

  TEST_CASE("Control modes: kControlTypeDeltaU, kControlTypeAdaptM, do_control, do_lock_control") {
    lds::gaussian::System model = MakeScalarGLDS(0.95, 0.10);
    model.do_adapt_m = true;
    const size_t ctype = lds::kControlTypeDeltaU | lds::kControlTypeAdaptM;
    lds::gaussian::Controller ctrl(model, 0.0, 5.0, ctype);
    CHECK((ctrl.control_type() & lds::kControlTypeDeltaU) != 0);
    CHECK((ctrl.control_type() & lds::kControlTypeAdaptM) != 0);
    ctrl.set_Kc(Matrix(1, 1).fill(1.0));
    ctrl.set_Kc_u(Matrix(1, 1).fill(0.2));
    ctrl.set_y_ref(Vector(1).fill(1.0));

    // When do_control=false, u_ref passed via Control() feeds through (limited to [u_lb, u_ub])
    ctrl.set_u_ref(Vector(1).fill(1.25));
    Vector u_open = ctrl.Control(Vector(1).fill(0.0), /*do_control=*/false);
    CHECK(u_open[0] == doctest::Approx(1.25));

    // With do_control=true, then do_lock_control=true holds u constant
    Vector u_active = ctrl.ControlOutputReference(Vector(1).fill(0.2), /*do_control=*/true);
    Vector u_locked1 = ctrl.ControlOutputReference(Vector(1).fill(10.0), /*do_control=*/true,
                                                   /*do_estimation=*/true, /*do_lock_control=*/true);
    Vector u_locked2 = ctrl.ControlOutputReference(Vector(1).fill(-10.0), /*do_control=*/true,
                                                   /*do_estimation=*/true, /*do_lock_control=*/true);
    CHECK(u_locked1[0] == doctest::Approx(u_active[0]));
    CHECK(u_locked2[0] == doctest::Approx(u_active[0]));
  }

  TEST_CASE("PLDS Controller clamps y_ref > 0 and keeps bounded finite control signals") {
    const data_t dt = 1e-3;
    lds::poisson::System p_sys(1, 1, 1, dt, 1e-3, 1e-4);
    p_sys.set_A(Matrix(1, 1).fill(0.985));
    p_sys.set_B(Matrix(1, 1).fill(0.05));
    p_sys.set_C(Matrix(1, 1).fill(1.0));
    p_sys.set_d(Vector(1).fill(std::log(5.0 * dt)));
    p_sys.Reset();

    lds::poisson::Controller p_ctrl(p_sys, 0.0, 5.0, lds::kControlTypeIntY);
    p_ctrl.set_Kc(Matrix(1, 1).fill(1.0));
    p_ctrl.set_Kc_inty(Matrix(1, 1).fill(5.0));

    // Asking for y_ref = 0 must clamp safely above 0 rather than producing -inf in log(y_ref)
    p_ctrl.set_y_ref(Vector(1).fill(0.0));
    CHECK(p_ctrl.y_ref()[0] > 0.0);

    // Closed-loop step with deterministic seeded Poisson observations
    p_ctrl.set_y_ref(Vector(1).fill(25.0 * dt));
    std::mt19937 gen(99);
    std::poisson_distribution<int> pois(5.0 * dt);
    for (int t = 0; t < 100; ++t) {
      Vector z(1);
      z[0] = static_cast<data_t>(pois(gen));
      Vector u = p_ctrl.ControlOutputReference(z);
      CHECK(std::isfinite(u[0]));
      CHECK(u[0] >= 0.0);
      CHECK(u[0] <= 5.0);
    }
  }

  TEST_CASE("kControlTypeIntY integrates linear latent error (cx - cx_ref) in log-rate space for PLDS") {
    // Pins v0.9 behavior: integral action accumulates (sys_.cx() - cx_ref_) * dt
    // rather than nonlinear (sys_.y() - y_ref_) * dt or raw (z_t - y_ref_) * dt.
    const data_t dt = 1e-3;
    lds::poisson::System p_sys(1, 1, 1, dt, 1e-3, 1e-4);
    p_sys.set_A(Matrix(1, 1).fill(0.9));
    p_sys.set_B(Matrix(1, 1).fill(0.1));
    p_sys.set_C(Matrix(1, 1).fill(1.0));
    p_sys.set_d(Vector(1).fill(std::log(5.0 * dt)));
    p_sys.Reset();

    lds::poisson::Controller p_ctrl(p_sys, -10.0, 10.0, lds::kControlTypeIntY);
    p_ctrl.set_Kc(Matrix(1, 1).fill(0.0));
    const data_t k_inty = 10.0;
    p_ctrl.set_Kc_inty(Matrix(1, 1).fill(k_inty));

    const data_t y_ref_val = 25.0 * dt;
    p_ctrl.set_y_ref(Vector(1).fill(y_ref_val));

    // Pass z == y(0) == 5.0 * dt with do_estimation=true so Kalman innovation is 0
    // and sys_.x() == 0, sys_.cx() == 0, sys_.y() == 5.0 * dt.
    // Steady-state setpoint: (A - I) * x_ref + B * u_ref = 0 with C * x_ref = log(5)
    // => x_ref = log(5), u_ref = 0.1 * log(5) / 0.1 = log(5).
    const data_t expected_cx_ref = std::log(5.0);
    const data_t expected_u_ref = expected_cx_ref;
    const data_t expected_int_e = (0.0 - expected_cx_ref) * dt;
    const data_t expected_u = expected_u_ref - k_inty * expected_int_e;

    Vector u = p_ctrl.ControlOutputReference(Vector(1).fill(5.0 * dt),
                                             /*do_control=*/true,
                                             /*do_estimation=*/true,
                                             /*do_lock_control=*/false,
                                             /*sigma_soft_start=*/0.0,
                                             /*sigma_u_noise=*/0.0,
                                             /*do_reset_at_control_onset=*/false);
    CHECK(u[0] == doctest::Approx(expected_u).epsilon(1e-12));
    // Confirm this differs from nonlinear output-space error (y - y_ref) * dt
    const data_t nonlinear_u = expected_u_ref - k_inty * (5.0 * dt - y_ref_val) * dt;
    CHECK(std::abs(u[0] - nonlinear_u) > 1e-2);
  }
}
