import pytest

import ldsctrlest

def test_glds_sys():
    ldsctrlest.gaussian.System()
    ldsctrlest.gaussian.System(2, 2, 2, .001)
    ldsctrlest.gaussian.System(2, 2, 2, .001, r0=.2)
    ldsctrlest.gaussian.System(2, 2, 2, .001, p0=.4)
    gs = ldsctrlest.gaussian.System(2, 2, 2, .001, p0=.4, q0=7)
    with pytest.raises(TypeError):
        ldsctrlest.gaussian.System(42)
    gs.__repr__()

    gs.Filter([1, 1], [2, 1])
    with pytest.raises(TypeError):
        gs.Filter()

    prev_x = gs.x
    gs.Simulate(3)
    gs.Reset()
    assert gs.x != prev_x
    with pytest.raises(TypeError):
        gs.Simulate()
    
    with pytest.raises(TypeError):
        ldsctrlest.gaussian.System(42)


