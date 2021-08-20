import ldsctrlest.base
from ldsctrlest.base import UniformVectorList#, UniformSystemList
import ldsctrlest.gaussian
import ldsctrlest.poisson

__version__ = ldsctrlest.base.__version__


def UniformMatrixList(mats, free_dim=0):
    if free_dim == 0:
        return ldsctrlest.base.UniformMatrixListFreeDimNone(mats)
    elif free_dim == 1:
        return ldsctrlest.base.UniformMatrixListFreeDim1(mats)
    elif free_dim == 2:
        return ldsctrlest.base.UniformMatrixListFreeDim2(mats)