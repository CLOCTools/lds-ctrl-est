"""Tests for uniform matrix, vector, and system classes"""
import numpy as np
import pytest

import ldsctrlest
from ldsctrlest import UniformMatrixList as UML, UniformVectorList as UVL

arr2x2 = np.eye(2)
arr3x3 = np.eye(3)
arr2x3 = np.ones((2, 3))
arr3x2 = np.ones((3, 2))


def _test_other_funcs(uml, capfd):
    assert uml.size == 3
    assert np.all(uml.at(1) == uml[1])

    # Swap
    z2x2 = np.zeros((2, 2))
    uml.Swap(z2x2, 0)
    assert np.all(z2x2 == uml[0])

    with pytest.raises(TypeError):
        uml.Swap(z2x2, -1)

    uml.Swap(z2x2, 42)
    warning = capfd.readouterr().err
    assert warning.startswith("Requested UniformMatrixList element out of bounds")
    


def test_uniform_mat_list_free_dim_0(capfd):
    # 0 dim free (default)
    uml = UML([arr2x2, arr2x2, arr2x2])
    with pytest.raises(RuntimeError):
        UML([arr3x3, arr2x2, arr2x2])
    with pytest.raises(RuntimeError):
        UML([arr2x2, arr2x3, arr2x2])
    with pytest.raises(RuntimeError):
        UML([arr2x2, arr2x2, arr3x2])
    assert uml.dim() == [2, 2]
    assert uml.dim(2) == [2, 2]
    _test_other_funcs(uml, capfd)


def test_uniform_mat_list_free_dim_1(capfd):
    # 1st dim free
    uml = UML([arr2x2, arr3x2, arr2x2], free_dim=1)
    with pytest.raises(RuntimeError):
        UML([arr2x3, arr2x2, arr2x3], free_dim=1)
    with pytest.raises(RuntimeError):
        UML([arr2x2, arr2x2, arr3x3], free_dim=1)
    assert uml.dim() == [2, 2]
    assert uml.dim(1) == [3, 2]
    _test_other_funcs(uml, capfd)


def test_uniform_mat_list_free_dim_2(capfd):
    # 2nd dim free
    uml = UML([arr2x2, arr2x3, arr2x2], free_dim=2)
    with pytest.raises(RuntimeError):
        UML([arr2x3, arr3x3, arr2x3], free_dim=2)
    with pytest.raises(RuntimeError):
        UML([arr2x2, arr2x2, arr3x3], free_dim=2)
    assert uml.dim() == [2, 2]
    assert uml.dim(1) == [2, 3]
    _test_other_funcs(uml, capfd)


def test_uniform_vec_list(capfd):
    v3 = np.ones(3)
    v4 = np.ones(4)
    uvl = UVL([v3, v3, v3])
    with pytest.raises(RuntimeError):
        UVL([v3, v3, v4])
    with pytest.raises(RuntimeError):
        UVL([v4, v3, v3])
    assert uvl.dim == 3
    assert uvl.size == 3
    assert UVL([v4, v4]).dim == 4
    assert UVL([v4, v4]).size == 2
    assert np.all(uvl[0] == uvl.at(0))
    uvl.Swap(np.zeros(3), 1)

    # assigning with wrong dims gets skipped
    uvl.Swap(np.zeros(42), 0)
    warning = capfd.readouterr().err
    assert not np.all(uvl[0] == np.zeros(3))
    assert warning.startswith("Cannot swap")
    # skip when at wrong index
    uvl.Swap(np.zeros(4), 42)
    warning = capfd.readouterr().err
    assert warning.startswith("Requested UniformMatrixList element out of bounds")
    # fail when index is negative (not size_t)
    with pytest.raises(TypeError):
        uvl.Swap(np.zeros(4), -1)


def test_uniform_sys_list(capfd):
    pass