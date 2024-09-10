"""Some tests that can be easily re-used for both Gaussian and Poisson
systems. In hindsight, should have structured tests this way instead
of test_glds.py and test_plds.py"""

import ldsctrlest as lds
import numpy as np
import pytest
from ldsctrlest import gaussian, poisson


@pytest.mark.parametrize("n_u", [1, 3])
@pytest.mark.parametrize("n_x", [1, 4])
@pytest.mark.parametrize("n_y", [1, 5])
@pytest.mark.parametrize("n_t", [1, 100])
@pytest.mark.parametrize("module", [gaussian, poisson])
def test_simulate_imp(module, n_u, n_x, n_y, n_t):
    sys = module.System(n_u, n_x, n_y, 0.001)
    cx_imp = sys.simulate_imp(n_t)
    assert len(cx_imp) == n_u
    assert cx_imp[0].shape == (n_y, n_t)


@pytest.mark.parametrize("n_step", [1, 5])
@pytest.mark.parametrize("n_trials", [1, 6])
@pytest.mark.parametrize("n_u", [1, 3])
@pytest.mark.parametrize("n_x", [1, 4])
@pytest.mark.parametrize("n_y", [1, 5])
@pytest.mark.parametrize("n_t", [5, 100])
@pytest.mark.parametrize("module", [gaussian, poisson])
def test_nstep_pred_block(module, n_u, n_x, n_y, n_t, n_trials, n_step):
    sys = module.System(n_u, n_x, n_y, 0.001)
    u = lds.UniformMatrixList(
        [np.random.randn(n_u, n_t) for _ in range(n_trials)], free_dim=2
    )
    z = lds.UniformMatrixList(
        [np.random.randn(n_y, n_t) for _ in range(n_trials)], free_dim=2
    )
    x_filt, x_pred, y_pred = sys.nstep_pred_block(u, z, n_step)

    for x_filt_i, x_pred_i, y_pred_i in zip(x_filt, x_pred, y_pred):
        assert x_filt_i.shape == (n_x, n_t)
        assert x_pred_i.shape == (n_x, n_t - n_step)
        assert y_pred_i.shape == (n_y, n_t - n_step)


if __name__ == "__main__":
    pytest.main([__file__, "-xs"])
