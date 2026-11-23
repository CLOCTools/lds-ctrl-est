//===-- tests/test_utils.cpp - Core lds Utilities Unit Tests ----*- C++ -*-===//
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

TEST_SUITE("utils") {
  TEST_CASE("Limit clamps std::vector, Vector, and Matrix element-wise") {
    std::vector<data_t> vec = {-5.0, 0.25, 1.5, 10.0};
    lds::Limit(vec, 0.0, 2.0);
    CHECK(vec[0] == doctest::Approx(0.0));
    CHECK(vec[1] == doctest::Approx(0.25));
    CHECK(vec[2] == doctest::Approx(1.5));
    CHECK(vec[3] == doctest::Approx(2.0));

    Vector v = {-1.0, 0.5, 3.0};
    lds::Limit(v, -0.5, 1.5);
    CHECK(v[0] == doctest::Approx(-0.5));
    CHECK(v[1] == doctest::Approx(0.5));
    CHECK(v[2] == doctest::Approx(1.5));

    Matrix m = {{-3.0, 0.1}, {0.9, 5.0}};
    lds::Limit(m, 0.0, 1.0);
    CHECK(m(0, 0) == doctest::Approx(0.0));
    CHECK(m(0, 1) == doctest::Approx(0.1));
    CHECK(m(1, 0) == doctest::Approx(0.9));
    CHECK(m(1, 1) == doctest::Approx(1.0));
  }

  TEST_CASE("Reassign copies matching shapes and throws on mismatch") {
    Vector dst(3, arma::fill::zeros);
    Vector src_ok = {1.0, 2.0, 3.0};
    lds::Reassign(dst, src_ok);
    CHECK(arma::approx_equal(dst, src_ok, "absdiff", 1e-12));

    Vector src_bad(2, arma::fill::ones);
    CHECK_THROWS_AS(lds::Reassign(dst, src_bad), std::runtime_error);

    Matrix mdst(2, 2, arma::fill::zeros);
    Matrix msrc_ok = {{1.0, 2.0}, {3.0, 4.0}};
    lds::Reassign(mdst, msrc_ok);
    CHECK(arma::approx_equal(mdst, msrc_ok, "absdiff", 1e-12));

    Matrix msrc_bad(2, 3, arma::fill::ones);
    CHECK_THROWS_AS(lds::Reassign(mdst, msrc_bad), std::runtime_error);
  }

  TEST_CASE("ForceSymPD and ForceSymMinEig restore positive definiteness") {
    // Asymmetric matrix made SPD by ForceSymPD symmetrization
    Matrix asym = {{1.0, 0.05}, {0.0, 1.0}};
    CHECK_FALSE(asym.is_sympd());
    lds::ForceSymPD(asym);
    CHECK(asym.is_sympd());

    // Diagonal negative entry: ForceSymPD shifts minimum eigenvalue strictly > 0
    Matrix diag_neg = {{-0.5, 0.0}, {0.0, 2.0}};
    CHECK_FALSE(diag_neg.is_sympd());
    lds::ForceSymPD(diag_neg);
    CHECK(diag_neg.is_sympd());
    Vector diag_eigs;
    arma::eig_sym(diag_eigs, diag_neg);
    CHECK(arma::all(diag_eigs > 0.0));

    // Indefinite symmetric matrix shifted cleanly by ForceSymMinEig
    Matrix sym_indef = {{1.0, 2.0}, {2.0, 1.0}};  // eigenvalues 3 and -1
    CHECK_FALSE(sym_indef.is_sympd());
    lds::ForceSymMinEig(sym_indef, 0.25);
    CHECK(sym_indef.is_sympd());
    Vector eigvals;
    arma::eig_sym(eigvals, sym_indef);
    CHECK(arma::min(eigvals) >= 0.25 - 1e-12);
  }

  TEST_CASE("ForceSymPD handles off-diagonal precedence, large-scale matrices, and is_sympd()") {
    // (a) Off-diagonal eps precedence bug: {{1, 2}, {2, 1}} has min_eig = -1;
    // adding eps to all entries instead of only the diagonal stalls at min_eig = 0.
    Matrix sym_indef = {{1.0, 2.0}, {2.0, 1.0}};
    CHECK_FALSE(sym_indef.is_sympd());
    CHECK_NOTHROW(lds::ForceSymPD(sym_indef));
    CHECK(sym_indef.is_sympd());

    // Zero off-diagonal must remain zero when diagonal is shifted
    Matrix diag_zero = {{0.0, 0.0}, {0.0, 1.0}};
    CHECK_NOTHROW(lds::ForceSymPD(diag_zero));
    CHECK(diag_zero(0, 1) == 0.0);
    CHECK(diag_zero(1, 0) == 0.0);
    CHECK(diag_zero.is_sympd());

    // (b) Large-scale matrix (~1e6 entries): machine eps at 1e6 is ~1e-10, so
    // adding scalar arma::datum::eps (~2.22e-16) is lost in rounding.
    Matrix large_singular = {{1e6, 1e6}, {1e6, 1e6}};
    CHECK_FALSE(large_singular.is_sympd());
    CHECK_NOTHROW(lds::ForceSymPD(large_singular));
    CHECK(large_singular.is_sympd());

    // (c) eig_sym min_eig > 0 vs is_sympd() mismatch on poorly conditioned / scaled matrices
    Matrix poorly_scaled = {{1e8, 0.0}, {0.0, 0.0}};
    CHECK_FALSE(poorly_scaled.is_sympd());
    CHECK_NOTHROW(lds::ForceSymPD(poorly_scaled));
    CHECK(poorly_scaled.is_sympd());
  }

  TEST_CASE("lq and calcCov satisfy matrix identities") {
    arma::arma_rng::set_seed(42);
    Matrix X(3, 5, arma::fill::randn);
    Matrix L, Qt;
    lds::lq(L, Qt, X);

    CHECK(arma::approx_equal(L * Qt, X, "absdiff", 1e-11));
    CHECK(arma::approx_equal(Qt * Qt.t(), Matrix(3, 3, arma::fill::eye), "absdiff", 1e-11));
    // L should be lower-triangular (entries above diagonal == 0)
    CHECK(std::abs(L(0, 1)) < 1e-11);
    CHECK(std::abs(L(0, 2)) < 1e-11);
    CHECK(std::abs(L(1, 2)) < 1e-11);

    // Deterministic 2x4 sample covariance check (columns are samples)
    Matrix A = {{1.0, -1.0, 1.0, -1.0}, {2.0, 0.0, 2.0, 0.0}};
    Matrix cov = lds::calcCov(A, A);
    Matrix expected = {{1.0, 1.0}, {1.0, 1.0}};
    CHECK(arma::approx_equal(cov, expected, "absdiff", 1e-12));
  }

  TEST_CASE("UniformMatrixList, UniformVectorList, and UniformSystemList enforce uniformity") {
    Matrix m1(2, 2, arma::fill::eye);
    Matrix m2 = 2.0 * m1;
    Matrix mbad(2, 3, arma::fill::zeros);

    lds::UniformMatrixList<> mlist({m1, m2});
    REQUIRE(mlist.size() == 2);
    CHECK_THROWS_AS((lds::UniformMatrixList<>({m1, mbad})), std::runtime_error);
    CHECK_THROWS_AS(mlist.append(mbad), std::runtime_error);

    // Swap exchanges elements cleanly
    Matrix ext = 5.0 * m1;
    mlist.Swap(ext, 1);
    CHECK(arma::approx_equal(mlist.at(1), 5.0 * m1, "absdiff", 1e-12));
    CHECK(arma::approx_equal(ext, m2, "absdiff", 1e-12));

    // UniformVectorList checks vector lengths
    Vector v1 = {1.0, 2.0};
    Vector v2 = {3.0, 4.0};
    Vector vbad = {1.0};
    lds::UniformVectorList vlist({v1, v2});
    REQUIRE(vlist.size() == 2);
    CHECK_THROWS_AS((lds::UniformVectorList({v1, vbad})), std::runtime_error);

    // UniformSystemList validates consistent (n_u, n_x, n_y)
    lds::gaussian::System s1(1, 2, 2, 1e-3);
    lds::gaussian::System s2(1, 2, 2, 1e-3);
    lds::gaussian::System s_bad(2, 2, 2, 1e-3);
    lds::UniformSystemList<lds::gaussian::System> slist({s1, s2});
    CHECK(slist.size() == 2);
    CHECK(slist.dim() == std::array<size_t, 3>{1, 2, 2});
    CHECK_THROWS_AS((lds::UniformSystemList<lds::gaussian::System>({s1, s_bad})), std::runtime_error);
  }
}
