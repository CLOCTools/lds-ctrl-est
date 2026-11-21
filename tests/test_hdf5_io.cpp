//===-- tests/test_hdf5_io.cpp - Armadillo HDF5 I/O Unit Tests --*- C++ -*-===//
//
// Copyright 2026 CLOCTools Contributors
// Licensed under the Apache License, Version 2.0
//
//===----------------------------------------------------------------------===//

#include <doctest.h>
#include <ldsCtrlEst>

#include <cstdio>
#include <string>

using lds::Matrix;
using lds::Vector;

TEST_SUITE("hdf5_io") {
  TEST_CASE("HDF5 round-trip preserves GLDS parameters and trajectory matrices exactly") {
    const std::string h5_path = "lds_unit_test_roundtrip.h5";
    std::remove(h5_path.c_str());

    lds::gaussian::System sys(1, 2, 2, 1e-3);
    Matrix A = {{0.92, -0.04}, {0.03, 0.88}};
    Matrix B(2, 1);
    B(0, 0) = 0.15;
    B(1, 0) = -0.08;
    Vector g = {1.25};
    Vector m = {0.01, -0.02};
    Matrix Q = {{1e-4, 2e-5}, {2e-5, 3e-4}};
    Matrix C = {{1.0, 0.5}, {-0.25, 1.5}};
    Vector d = {0.05, -0.10};
    Matrix R = {{2e-2, 1e-3}, {1e-3, 4e-2}};

    sys.set_A(A);
    sys.set_B(B);
    sys.set_g(g);
    sys.set_m(m, true);
    sys.set_Q(Q);
    sys.set_C(C);
    sys.set_d(d);
    sys.set_R(R);
    sys.Reset();

    Matrix y_traj(2, 10, arma::fill::zeros);
    Vector u = {0.5};
    for (size_t t = 0; t < 10; ++t) {
      sys.f(u, false);
      sys.set_x(sys.x());
      y_traj.col(t) = sys.y();
    }

    const arma::hdf5_opts::opts replace = arma::hdf5_opts::replace;
    REQUIRE(Vector(1).fill(sys.dt()).save(arma::hdf5_name(h5_path, "dt", replace)));
    REQUIRE(sys.A().save(arma::hdf5_name(h5_path, "A", replace)));
    REQUIRE(sys.B().save(arma::hdf5_name(h5_path, "B", replace)));
    REQUIRE(sys.g().save(arma::hdf5_name(h5_path, "g", replace)));
    REQUIRE(sys.m().save(arma::hdf5_name(h5_path, "m", replace)));
    REQUIRE(sys.C().save(arma::hdf5_name(h5_path, "C", replace)));
    REQUIRE(sys.d().save(arma::hdf5_name(h5_path, "d", replace)));
    REQUIRE(sys.R().save(arma::hdf5_name(h5_path, "R", replace)));
    REQUIRE(y_traj.save(arma::hdf5_name(h5_path, "y_traj", replace)));

    Vector dt_loaded;
    Matrix A_loaded, B_loaded, C_loaded, R_loaded, y_loaded;
    Vector g_loaded, m_loaded, d_loaded;

    REQUIRE(dt_loaded.load(arma::hdf5_name(h5_path, "dt")));
    REQUIRE(A_loaded.load(arma::hdf5_name(h5_path, "A")));
    REQUIRE(B_loaded.load(arma::hdf5_name(h5_path, "B")));
    REQUIRE(g_loaded.load(arma::hdf5_name(h5_path, "g")));
    REQUIRE(m_loaded.load(arma::hdf5_name(h5_path, "m")));
    REQUIRE(C_loaded.load(arma::hdf5_name(h5_path, "C")));
    REQUIRE(d_loaded.load(arma::hdf5_name(h5_path, "d")));
    REQUIRE(R_loaded.load(arma::hdf5_name(h5_path, "R")));
    REQUIRE(y_loaded.load(arma::hdf5_name(h5_path, "y_traj")));

    lds::gaussian::System reloaded(1, 2, 2, dt_loaded[0]);
    reloaded.set_A(A_loaded);
    reloaded.set_B(B_loaded);
    reloaded.set_g(g_loaded);
    reloaded.set_m(m_loaded, true);
    reloaded.set_C(C_loaded);
    reloaded.set_d(d_loaded);
    reloaded.set_R(R_loaded);

    CHECK(arma::approx_equal(reloaded.A(), A, "absdiff", 1e-12));
    CHECK(arma::approx_equal(reloaded.B(), B, "absdiff", 1e-12));
    CHECK(arma::approx_equal(reloaded.g(), g, "absdiff", 1e-12));
    CHECK(arma::approx_equal(reloaded.m(), m, "absdiff", 1e-12));
    CHECK(arma::approx_equal(reloaded.C(), C, "absdiff", 1e-12));
    CHECK(arma::approx_equal(reloaded.d(), d, "absdiff", 1e-12));
    CHECK(arma::approx_equal(reloaded.R(), R, "absdiff", 1e-12));
    CHECK(arma::approx_equal(y_loaded, y_traj, "absdiff", 1e-12));

    std::remove(h5_path.c_str());
  }
}
