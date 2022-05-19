import ldsctrlest
import numpy as np
from numpy.random import rand
import pytest

from ldsctrlest import (
    UniformMatrixList as UML,
    UniformVectorList as UVL,
    kControlTypeIntY,
    kControlTypeDeltaU,
    kControlTypeAdaptM,
)
from ldsctrlest.gaussian import SwitchedController as SGCtrl, System as GSys
from ldsctrlest.poisson import SwitchedController as SPCtrl, System as PSys

np.random.seed(1847)

dt = 0.001


def _noisy_y(sys):
    return sys.y + rand(sys.n_y).reshape((-1, 1))


def _test_sctrl(SCtrl, Sys, n_u, n_x, n_y):
    systems = []
    for i in range(3):
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
        systems.append(sys)
    SCtrl()
    SCtrl(systems, -1000, 1000, kControlTypeDeltaU)

    # configure controller
    ctrl = SCtrl(systems, -5, 5, 0x0)
    ctrl.y_ref = rand(n_y)
    ctrl.Kc = UML([100 * np.ones((n_u, n_x))] * 3)
    ctrl.g_design = UVL([sys.g] * 3)
    ctrl.tau_awu = 0.1

    # run with a bunch of options
    u = ctrl.ControlOutputReference(_noisy_y(ctrl.sys))
    assert u.shape == (n_u, 1)
    u_ref_prev = ctrl.u_ref
    u = ctrl.Control(_noisy_y(ctrl.sys), do_control=False)
    assert np.all(u == u_ref_prev)
    u = ctrl.Control(
        _noisy_y(ctrl.sys), sigma_soft_start=0.1, do_reset_at_control_onset=False
    )
    ctrl.y_ref = rand(n_y)
    u_prev = ctrl.ControlOutputReference(_noisy_y(ctrl.sys))
    u = ctrl.ControlOutputReference(_noisy_y(ctrl.sys), do_lock_control=True)
    assert np.all(u == u_prev)
    u = ctrl.ControlOutputReference(
        _noisy_y(ctrl.sys), do_lock_control=True, sigma_u_noise=0.2
    )
    assert np.all(u == u_prev)
    u = ctrl.ControlOutputReference(_noisy_y(ctrl.sys), sigma_u_noise=1)
    u = ctrl.ControlOutputReference(_noisy_y(ctrl.sys), do_reset_at_control_onset=False)

    # test switching with B (since random)
    ctrl.Switch(0)
    ctrl.Control(_noisy_y(ctrl.sys))
    prev_B = ctrl.sys.B
    ctrl.Switch(1)
    ctrl.Control(_noisy_y(ctrl.sys))
    assert not np.all(ctrl.sys.B == prev_B)
    prev_B = ctrl.sys.B
    ctrl.Switch(2)
    ctrl.Control(_noisy_y(ctrl.sys))
    assert not np.all(ctrl.sys.B == prev_B)
    prev_B = ctrl.sys.B

    # use integrated error
    ctrl.control_type |= kControlTypeIntY
    ctrl.Kc_inty = UML([1e-3 * np.ones((n_u, n_y))] * 3)
    prev_x = ctrl.sys.x
    ctrl.ControlOutputReference(_noisy_y(ctrl.sys), do_estimation=False)
    assert not np.all(prev_x == ctrl.sys.x)
    ctrl.Switch(0)
    ctrl.Control(_noisy_y(ctrl.sys))
    ctrl.Switch(1)
    ctrl.Control(_noisy_y(ctrl.sys))
    ctrl.Switch(2)
    ctrl.Control(_noisy_y(ctrl.sys))

    # du control
    ctrl.sys.do_adapt_m = True
    ctrl.sys.Q_m = 1e-5 * np.eye(n_x)
    ctrl.control_type |= kControlTypeDeltaU
    ctrl.Kc_u = UML([0.05 * np.ones((n_u, n_u))] * 3)
    ctrl.Switch(0)
    ctrl.Control(_noisy_y(ctrl.sys))
    ctrl.Switch(1)
    ctrl.Control(_noisy_y(ctrl.sys))
    ctrl.Switch(2)
    ctrl.Control(_noisy_y(ctrl.sys))

    # adaptive control
    ctrl.Switch(0)
    ctrl.sys.do_adapt_m = True
    ctrl.control_type = kControlTypeIntY | kControlTypeAdaptM
    ctrl.Control(_noisy_y(ctrl.sys))
    ctrl.Switch(1)
    ctrl.sys.do_adapt_m = True
    ctrl.Control(_noisy_y(ctrl.sys))
    ctrl.Switch(2)
    ctrl.sys.do_adapt_m = True
    ctrl.Control(_noisy_y(ctrl.sys))


def test_gaussian_ctrl():
    _test_sctrl(SGCtrl, GSys, 2, 3, 4)
    _test_sctrl(SGCtrl, GSys, 12, 13, 14)


def test_poisson_ctrl():
    _test_sctrl(SPCtrl, PSys, 2, 3, 4)
    _test_sctrl(SPCtrl, PSys, 12, 13, 14)


if __name__ == "__main__":
    test_gaussian_ctrl()
    test_poisson_ctrl()
