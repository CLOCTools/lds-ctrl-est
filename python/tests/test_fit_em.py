import numpy as np
from numpy.random import rand as rand

import ldsctrlest
from ldsctrlest.gaussian import FitEM as GFitEM, Fit as GFit
from ldsctrlest.poisson import FitEM as PFitEM, Fit as PFit

dt = 0.001


def _test_FitEM(FitEM, n_x, n_u, n_z, Fit):
    T1 = 10
    T2 = 20
    u_train = ldsctrlest.UniformMatrixList([rand(n_u, T1), rand(n_u, T2)], free_dim=2)
    z_train = ldsctrlest.UniformMatrixList([rand(n_z, T1), rand(n_z, T2)], free_dim=2)
    fitEM = FitEM(n_x, dt, u_train, z_train)

    fitEM.Run()

    # need to reclaim these, since memory gets moved to FitEM
    u_train, z_train = fitEM.ReturnData()

    # getters
    x_est = fitEM.x
    assert len(x_est) == len(u_train)
    assert x_est[0].shape == (n_x, T1)
    assert x_est[1].shape == (n_x, T2)
    y_est = fitEM.y
    assert len(y_est) == len(u_train)
    assert y_est[0].shape == (n_z, T1)
    assert y_est[1].shape == (n_z, T2)
    fitEM.sum_E_x_t_x_t
    fitEM.sum_E_xu_tm1_xu_tm1
    fitEM.sum_E_xu_t_xu_tm1
    # -1 for each trial because E step starts at t_1
    assert fitEM.n_t_tot == T1 + T2 - len(u_train)
    fitEM.theta

    # construct from initial fit
    fit = Fit(n_u, n_x, n_z, dt)
    # u_train and z_train need to be reclaimed before this
    FitEM(fit, u_train, z_train)


def test_GFitEM():
    _test_FitEM(GFitEM, 13, 12, 14, GFit)
    _test_FitEM(GFitEM, 3, 2, 4, GFit)


def test_PFitEM():
    _test_FitEM(PFitEM, 3, 2, 4, PFit)
    _test_FitEM(PFitEM, 13, 12, 14, PFit)


# test_GFitEM()
