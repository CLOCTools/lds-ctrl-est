import ldsctrlest
import numpy as np
from numpy.random import rand
import pytest

from ldsctrlest import kControlTypeIntY, kControlTypeDeltaU, kControlTypeAdaptM
from ldsctrlest.gaussian import Controller as GCtrl, System as GSys
from ldsctrlest.poisson import Controller as PCtrl, System as PSys

dt = 0.001

def _noisy_y(sys):
    return sys.y + rand(sys.n_y).reshape((-1, 1))

def _test_ctrl(Ctrl, Sys, n_u, n_x, n_y):
    sys = Sys(n_u, n_x, n_y, dt)
    sys.B = rand(n_x, n_u) * 0.054
    sys.A = 0.986 * np.eye(n_x)
    # sys.A = rand(n_x, n_x)
    sys.C = rand(n_y, n_x)
    sys.Q = 1e-8 * np.eye(n_x)
    if Sys == ldsctrlest.gaussian.System:
        sys.R = 1e-2 * np.eye(n_y)
    sys.x0 = 0.001 * np.ones(n_x)
    sys.Reset()
    Ctrl()
    Ctrl(sys, -1000, 1000, kControlTypeDeltaU)

    # configure controller
    ctrl = Ctrl(sys, -5, 5, 0x0)
    ctrl.y_ref = rand(n_y)
    ctrl.Kc = 100*np.ones((n_u, n_x))
    ctrl.g_design = sys.g
    ctrl.tau_awu = 0.1

    # run with a bunch of options
    u = ctrl.ControlOutputReference(_noisy_y(ctrl.sys))
    assert u.shape == (n_u, 1)
    u_ref_prev = ctrl.u_ref
    u = ctrl.Control(_noisy_y(ctrl.sys), do_control = False)
    assert np.all(u == u_ref_prev)
    u = ctrl.Control(_noisy_y(ctrl.sys), sigma_soft_start=.1, do_reset_at_control_onset=False)
    ctrl.y_ref = rand(n_y)
    u_prev = ctrl.ControlOutputReference(_noisy_y(ctrl.sys))
    u = ctrl.ControlOutputReference(_noisy_y(ctrl.sys), do_lock_control=True)
    assert np.all(u == u_prev)
    u = ctrl.ControlOutputReference(_noisy_y(ctrl.sys), do_lock_control=True, sigma_u_noise=.2)
    assert np.all(u == u_prev)
    u = ctrl.ControlOutputReference(_noisy_y(ctrl.sys), sigma_u_noise=1)
    u = ctrl.ControlOutputReference(_noisy_y(ctrl.sys), do_reset_at_control_onset=False)

    # use integrated error
    ctrl.control_type |= kControlTypeIntY
    ctrl.Kc_inty = 1e-3*np.ones((n_u, n_y))
    prev_x = ctrl.sys.x
    ctrl.ControlOutputReference(_noisy_y(ctrl.sys), do_estimation=False)
    assert not np.all(prev_x == ctrl.sys.x)

    # du control
    with pytest.raises(RuntimeError):
        # Kc_u hasn't been initialized to the right size
        # since du ctrl type hasn't been set
        ctrl.Kc_u = .05 * np.ones((n_u, n_u))
    ctrl.sys.do_adapt_m = True
    ctrl.sys.Q_m = 1e-5 * np.eye(n_x)
    ctrl.control_type |= kControlTypeDeltaU
    ctrl.Kc_u = .05 * np.ones((n_u, n_u))
    ctrl.Control(_noisy_y(ctrl.sys))

    # adaptive control
    ctrl.control_type = kControlTypeIntY | kControlTypeAdaptM
    ctrl.sys.do_adapt_m = True
    ctrl.Control(_noisy_y(ctrl.sys))


def test_gaussian_ctrl():
    _test_ctrl(GCtrl, GSys, 2, 3, 4)
    _test_ctrl(GCtrl, GSys, 12, 13, 14)


def test_poisson_ctrl():
    _test_ctrl(PCtrl, PSys, 2, 3, 4)
    _test_ctrl(PCtrl, PSys, 12, 13, 14)


if __name__ == '__main__':
    test_gaussian_ctrl()
    test_poisson_ctrl()