"""Tests for uniform matrix, vector, and system classes"""
import numpy as np
import pytest

from ldsctrlest import UniformMatrixList

def test_uniform_mat_list():
    arr2x2 = np.eye(2)
    arr3x3 = np.eye(3)
    arr2x3 = np.ones((2, 3))
    arr3x2 = np.ones((3, 2))

    # 0 dim free (default)
    UniformMatrixList([arr2x2, arr2x2, arr2x2])
    with pytest.raises(RuntimeError):
        UniformMatrixList([arr2x2, arr2x2, arr2x2])
    with pytest.raises(RuntimeError):
        UniformMatrixList([arr2x2, arr2x3, arr2x2])
    with pytest.raises(RuntimeError):
        UniformMatrixList([arr2x2, arr2x2, arr3x2])

    # 1st dim free 
    UniformMatrixList([arr2x2, arr3x2, arr2x2], free_dim=1)
    with pytest.raises(RuntimeError):
        UniformMatrixList([arr2x3, arr2x2, arr2x3], free_dim=1)
    with pytest.raises(RuntimeError):
        UniformMatrixList([arr2x2, arr2x2, arr3x3], free_dim=1)

    # 2nd dim free 
    UniformMatrixList([arr2x2, arr2x3, arr2x2], free_dim=2)
    with pytest.raises(RuntimeError):
        UniformMatrixList([arr2x3, arr3x3, arr2x3], free_dim=2)
    with pytest.raises(RuntimeError):
        UniformMatrixList([arr2x2, arr2x2, arr3x3], free_dim=2)
