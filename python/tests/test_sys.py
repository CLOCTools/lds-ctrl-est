"""Some tests that can be easily re-used for both Gaussian and Poisson
systems. In hindsight, should have structured tests this way instead
of test_glds.py and test_plds.py"""

import numpy as np
import pytest
from ldsctrlest import gaussian, poisson

@pytest.mark.parametrize("n_u", [1, 2, 4])
@pytest.mark.parametrize("n_x", [1, 2, 4])
@pytest.mark.parametrize("n_y", [1, 2, 4])
@pytest.mark.parametrize("n_t", [1, 100])
@pytest.mark.parametrize("module", [gaussian, poisson])
def test_simulate_imp(module, n_u, n_x, n_y, n_t):
    sys = module.System(n_u, n_x, n_y, .001)
    cx_imp = sys.simulate_imp(n_t)
    assert len(cx_imp) == n_u
    assert cx_imp[0].shape == (n_y, n_t)