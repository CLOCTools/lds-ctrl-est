import numpy as np
from numpy.random import rand as rand

import ldsctrlest
from ldsctrlest.gaussian import FitEM as GFitEM
from ldsctrlest.poisson import FitEM as PFitEM

def _test_FitEM(FitEM, n_x, n_u, n_z):
    T1 = 10; T2 = 20
    u_train = ldsctrlest.UniformMatrixList([np.ones((n_u, T1)), np.ones((n_u, T2))], free_dim=2)
    z_train = ldsctrlest.UniformMatrixList([rand(n_z, T1), rand(n_z, T2)], free_dim=2)
    fitEM = FitEM(n_x, .001, u_train, z_train)
    # fitEM.Run()

def test_GFitEM():
    _test_FitEM(GFitEM, 3, 2, 4)
    _test_FitEM(GFitEM, 13, 12, 14)

def test_PFitEM():
    _test_FitEM(PFitEM, 3, 2, 4)
    _test_FitEM(PFitEM, 13, 12, 14)
