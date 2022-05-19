import numpy as np
import pytest

from ldsctrlest.gaussian import System as GLDS


def test_glds_sys():
    GLDS()
    GLDS(2, 2, 2, 0.001)
    GLDS(2, 2, 2, 0.001, r0=0.2)
    GLDS(2, 2, 2, 0.001, p0=0.4)
    gs = GLDS(2, 2, 2, 0.001, p0=0.4, q0=7)
    with pytest.raises(TypeError):
        GLDS(42)
    gs.__repr__()

    u_tm1 = np.ones(gs.n_u)
    z = gs.Simulate(u_tm1)
    prev_x = gs.x
    gs.Filter(u_tm1, z)
    assert not np.all(gs.x == prev_x)

    with pytest.raises(TypeError):
        gs.Filter()

    gs.Reset()
    with pytest.raises(TypeError):
        gs.Simulate()

    with pytest.raises(TypeError):
        GLDS(42)


def test_glds_sys_get_set():
    gs = GLDS(3, 2, 4, 0.001, p0=0.4, q0=7)
    assert len(gs.x) == 2
    gs.x = [7, 7]
    assert np.all(gs.x == [7, 7])

    gs.x0
    gs.x0 = [1, 2]
    gs.m
    gs.m = [3, 4]
    gs.set_m([3, 4], do_force_assign=True)
    gs.A
    gs.A = [[1, 2], [5, 6]]
    gs.B
    gs.B = np.zeros((gs.n_x, gs.n_u))
    gs.g
    gs.g = np.ones(gs.n_u)
    gs.C
    gs.C = np.zeros((gs.n_y, gs.n_x))
    gs.d = np.zeros(gs.n_y)

    gs.do_adapt_m
    gs.do_adapt_m = True

    gs.Q
    gs.Q = np.zeros((gs.n_x, gs.n_x))
    gs.Q_m
    gs.Q_m = np.zeros((gs.n_x, gs.n_x))
    gs.P0
    gs.P0 = np.zeros((gs.n_x, gs.n_x))
    gs.P0_m
    gs.P0_m = np.zeros((gs.n_x, gs.n_x))


def test_glds_sys_get():
    gs = GLDS(2, 2, 2, 0.001, p0=0.4, q0=7)
    gs.n_u
    gs.n_x
    gs.n_y
    gs.dt
    gs.P
    gs.P_m
    gs.cx
    gs.y
    gs.Ke
    gs.Ke_m


def test_glds_copy():
    gs = GLDS(2, 2, 2, 0.001, p0=0.4, q0=7)
    gs.A = np.random.rand(2, 2)
    gs2 = gs.copy()
    assert gs != gs2
    assert np.all(gs.A == gs2.A)

    gs2.A = np.random.rand(2, 2)
    assert not np.all(gs.A == gs2.A)
