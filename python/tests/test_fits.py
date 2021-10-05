import numpy as np

import ldsctrlest.gaussian as glds
import ldsctrlest.poisson as plds
import pytest

def test_gaussian_fit():
    # check R, make sure override works
    gs = glds.Fit(30, 20, 40, .001)
    gs.R
    gs.R = np.eye(gs.n_y)

def test_poisson_fit(capfd):
    # check R, make sure override works
    ps = plds.Fit(30, 20, 40, .001)
    ps.R

    # should raise warning since R doesn't do anything in PLDS
    ps.R = np.eye(ps.n_y)
    warning = capfd.readouterr().err
    assert warning.startswith('WARNING')

def _test_base_fit(fit):
    # getters
    fit.n_u
    fit.n_x
    fit.n_y
    fit.dt

    # getters/setters
    with pytest.raises(RuntimeError):
        fit.A; fit.A = np.eye(fit.n_x + 1)
    fit.A; fit.A = np.eye(fit.n_x)
    fit.B; fit.B = np.ones((fit.n_x, fit.n_u))
    fit.g; fit.g = np.ones(fit.n_u)
    fit.m; fit.m = np.ones(fit.n_x)
    fit.Q; fit.Q = 2*np.eye(fit.n_x)
    fit.x0; fit.x0 = np.ones(fit.n_x)
    fit.P0; fit.P0 = np.eye(fit.n_x)
    assert not np.all(fit.Q == fit.P0)
    fit.P0 = fit.Q
    assert np.all(fit.Q == fit.P0)
    # make sure copies are used to set, not shared memory
    fit.Q = 3*np.eye(fit.n_x)
    assert not np.all(fit.Q == fit.P0)
    fit.C; fit.C = np.ones((fit.n_y, fit.n_x))
    fit.d; fit.d = np.ones(fit.n_y)

    T = 5
    X = np.ones((fit.n_x, T))
    X_post = np.ones((fit.n_x, T))
    U = np.ones((fit.n_u, T))

    # make sure X is getting updated as a side effect
    ans = fit.f(X, U, 2)
    assert np.all(ans == X[:,2])
    ans = fit.f(X, U, 3)
    assert np.all(ans == X[:,3])
    ans = fit.f(X, X_post, U, 2)
    assert np.all(ans == X[:,2])

    fit.f(X, U, 2)
    fit.f(X, X_post, U, 2)
    fit.h(np.ones((fit.n_y, T)), X, 2)

def test_base_fits():
    _test_base_fit(plds.Fit(2, 3, 4, .001))
    _test_base_fit(plds.Fit(12, 13, 14, .001))
    _test_base_fit(glds.Fit(2, 3, 4, .001))
    _test_base_fit(glds.Fit(12, 13, 14, .001))