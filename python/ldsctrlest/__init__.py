import ldsctrlest.base
import ldsctrlest.gaussian
import ldsctrlest.poisson

__version__ = ldsctrlest.base.__version__

from ldsctrlest.base import UniformVectorList

def UniformMatrixList(mats, free_dim=0):
    if free_dim == 0:
        return ldsctrlest.base.UniformMatrixListFreeDimNone(mats)
    elif free_dim == 1:
        return ldsctrlest.base.UniformMatrixListFreeDim1(mats)
    elif free_dim == 2:
        return ldsctrlest.base.UniformMatrixListFreeDim2(mats)

def UniformSystemList(systems):
    sys_type = type(systems[0])  
    if sys_type == ldsctrlest.gaussian.System:
        return ldsctrlest.gaussian.UniformSystemList(systems)
    elif sys_type == ldsctrlest.poisson.System:
        return ldsctrlest.poisson.UniformSystemList(systems)
    else:
        raise TypeError(f"Unsupported system type {sys_type}")