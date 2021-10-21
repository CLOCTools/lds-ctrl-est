import numpy as np
import pytest

from ldsctrlest.poisson import System as PLDS


def test_plds_sys():
    PLDS()
    PLDS(2, 2, 2, 0.001)
    PLDS(2, 2, 2, 0.001, p0=0.4)
    ps = PLDS(2, 2, 2, 0.001, q0=0.4, p0=7)
    with pytest.raises(TypeError):
        PLDS(42)
    ps.__str__()

    u_tm1 = np.ones(ps.n_u)
    z = ps.Simulate(u_tm1)
    prev_x = ps.x
    ps.Filter(u_tm1, z)
    assert not np.all(ps.x == prev_x)

    with pytest.raises(TypeError):
        ps.Filter()

    ps.Reset()
    with pytest.raises(TypeError):
        ps.Simulate()

    with pytest.raises(TypeError):
        PLDS(42)


def test_plds_sys_get_set():
    ps = PLDS(3, 2, 4, 0.001, p0=0.4, q0=7)
    assert len(ps.x) == 2
    ps.x = [7, 7]
    assert np.all(ps.x == [7, 7])

    ps.x0
    ps.x0 = [1, 2]
    ps.m
    ps.m = [3, 4]
    ps.set_m([3, 4], do_force_assign=True)
    ps.A
    ps.A = [[1, 2], [5, 6]]
    ps.B
    ps.B = np.zeros((ps.n_x, ps.n_u))
    ps.g
    ps.g = np.ones(ps.n_u)
    ps.C
    ps.C = np.zeros((ps.n_y, ps.n_x))
    ps.d = np.zeros(ps.n_y)

    ps.do_adapt_m
    ps.do_adapt_m = True

    ps.Q
    ps.Q = np.zeros((ps.n_x, ps.n_x))
    ps.Q_m
    ps.Q_m = np.zeros((ps.n_x, ps.n_x))
    ps.P0
    ps.P0 = np.zeros((ps.n_x, ps.n_x))
    ps.P0_m
    ps.P0_m = np.zeros((ps.n_x, ps.n_x))


def test_plds_sys_get():
    ps = PLDS(2, 2, 2, 0.001, p0=0.4, q0=7)
    ps.n_u
    ps.n_x
    ps.n_y
    ps.dt
    ps.P
    ps.P_m
    ps.cx
    ps.y
    ps.Ke
    ps.Ke_m


def test_plds_copy():
    ps = PLDS(2, 2, 2, 0.001, p0=0.4, q0=7)
    ps.A = np.random.rand(2, 2)
    ps2 = ps.copy()
    assert ps != ps2
    assert np.all(ps.A == ps2.A)

    ps2.A = np.random.rand(2, 2)
    assert not np.all(ps.A == ps2.A)
