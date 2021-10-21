import numpy as np
from numpy.random import rand as rand
import pytest

import ldsctrlest
from ldsctrlest import SSIDWt
from ldsctrlest.gaussian import FitSSID as GFitSSID, Fit as GFit
from ldsctrlest.poisson import FitSSID as PFitSSID, Fit as PFit

np.random.seed(1847)

dt = 0.001


def _test_FitSSID(FitSSID, n_x, n_u, n_z):
    n_h = int(np.ceil(n_x * 2 / n_z))
    # n_h = 2*n_x
    T1 = 2 * (n_u + n_z) * n_h
    T2 = 20
    u_train = ldsctrlest.UniformMatrixList([rand(n_u, T1), rand(n_u, T2)], free_dim=2)
    z_train = ldsctrlest.UniformMatrixList([rand(n_z, T1), rand(n_z, T2)], free_dim=2)
    # construct without providing d
    fitSSID = FitSSID(n_x, n_h, dt, u_train, z_train)

    fit, sing_vals = fitSSID.Run(SSIDWt.kNone)
    fit, sing_vals = fitSSID.Run(SSIDWt.kMOESP)
    fit, sing_vals = fitSSID.Run(SSIDWt.kCVA)
    assert len(sing_vals) == n_h * n_z

    # need to reclaim these, since memory gets moved to FitSSID
    u_train, z_train = fitSSID.ReturnData()

    # construct with provided d
    fitSSID = FitSSID(n_x, n_h, dt, u_train, z_train, fit.d)

    # block hankel check
    u_train, z_train = fitSSID.ReturnData()
    with pytest.raises(RuntimeError):
        FitSSID(n_x, 2 * n_x, dt, u_train, z_train)


def test_GFitSSID():
    _test_FitSSID(GFitSSID, 13, 12, 14)
    _test_FitSSID(GFitSSID, 3, 2, 4)


def test_PFitSSID():
    _test_FitSSID(PFitSSID, 3, 2, 4)
    _test_FitSSID(PFitSSID, 13, 12, 14)
