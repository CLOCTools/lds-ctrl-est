# ldsCtrlEst
`ldsCtrlEst` is a C++ library for estimation and control of linear dynamical systems (LDS) with Gaussian or Poisson observations. It is meant to provide the functionality necessary to implement feedback control of linear dynamical systems experimentally. This library was originally developed for the task of controlling neuronal activity using spike count data as feedback and optogenetic inputs for control. However, the methods are generally applicable.

This library currently provides three namespaces.
 - `lds` : linear dynamical systems (without output/observations)
 - `lds::gaussian`: linear dynamical systems with Gaussian observations
 - `lds::poisson`: linear dynamical systems with Poisson observations

*Future iterations may include an additional namespace for LDS with Bernoulli observations (`lds::bernoulli`).*

# Project Scope
The goal of this project is to provide necessary functions to implement feedback control of linear dynamical systems experimentally: *i.e.*, online estimation of state feedback and calculation of control signal updates. Given its intended use in experiments, the library seeks to be **practical** in all things and thus includes optional features such as adaptive estimation of a process disturbance to improve robustness in state estimation and a mechanism for combatting integrator windup with control signal saturation. For cases where the system to be controlled is not adequately modeled as having linear dynamics but has multiple quasi-linear operating modes, a switched control scheme is also implemented. It switches between multiple controllers designed for each operating mode of the physical system as it changes. It also includes options to toggle on/off feedback control and state estimation independently, which can be practically useful when testing the components of the control system. Moreover, to avoid the need for numerical integration of continuous-time models, all state-space models used here are discrete-time.

Generally, the `ldsCtrlEst` library does **not** endeavor to provide functionality for things that can be carried out offline/before experiments. For example, it does not design controller gains. Given a model of the system to be controlled, these parameters may be optimized before experimental application in most cases, and there are numerous options available to scientists/engineers in languages such as Matlab and Python for design. An exception to this guiding principle to project scope is the included code for fitting state-space models to data. Currently, this fitting portion of the library is a configurable option, but in future releases this may migrate to a separate project as it is not intended for online use.

Among other things, this project also does not provide methods for trajectory optimization, linearization of nonlinear models, or other methods related to nonlinear control, with the exception of the nonlinear state estimator for Poisson-output LDS models.

# Repository Design
- All dynamical systems *with observations* (`lds::gaussian::System`, `lds::poisson::System`) are derived from a prototypical linear dynamical system abstract type (`lds::System`).
- These Gaussian and Poisson system types include user-accessible functions for one-step filtering for one-step simulation, etc.
- The controller types for Gaussian- and Poisson-output systems (`lds::gaussian::Controller`, `lds::gaussian::SwitchedController`, `lds::poisson::Controller`, `lds::poisson::SwitchedController`) are derived from an abstract class template (`lds::Controller`) that is generic over LDS types derived from `lds::System` (here, `lds::gaussian::System` and `lds::poisson::System`). `lds::Controller` provides functions for one-step updates of the control signal, based on feedback and a target/reference signal. For the common problem of output reference tracking, the controller uses the underlying system model to estimate the control signal required to track the target, effectively providing model-based open-loop control if the estimator is disabled.
- In order to ensure dimensionalities always match internally, every property of a system/controller class is `protected` or `private`. Get methods provide read-only references for most signals/parameters. Where appropriate, set methods are defined so users can change hidden parameters if and only if the new parameter has the correct dimensions.
# Repository Organization
- Header files are located under `include/ldsEstCtrl_h`.
- Source files are located under `src/` (main source code) and `src-fit/` (model fitting-related source code).
- Wrappers for exposing functions to Matlab as executables (mex) are located under `matlab/`. Currently, only fitting functions of the library are exposed for use in Matlab.
- Complimentary Matlab functions for control and estimation are also located under `matlab/`. They are provided as methods of `GLDS` and `PLDS` class definitions.
- Example programs and visualization scripts are located under `examples/`.
- Example programs that demonstrate how to use ldsCtrlEst in other projects are provided in `misc/`. See `misc/test-cmake-installation` for a project that uses `cmake` to configure your project build and `misc/test-pkgconfig-installation` which is the same but uses a hand-written Makefile and calls to pkg-config. As the names suggest, building these programs is a simple way to test your installation of ldsCtrlEst.


# Installation
`ldsCtrlEst` requires [Armadillo](http://arma.sourceforge.net/) for linear algebra as well as [HDF5](https://www.hdfgroup.org/downloads/hdf5/) for saving output. [`vcpkg`](https://vcpkg.io/) is a cross-platform C++ package manager which allows us to easily install and use the dependencies in isolation.

First, clone the repository along with submodules:
```
git clone --recurse-submodules https://github.com/stanley-rozell/lds-ctrl-est.git 
cd lds-ctrl-est
# use git submodule update --init if you clone the repo without --recurse-submodules
```

Now generate the cache and build using your IDE or from the command line as follows.
```
mkdir build
cmake -B build -S .
cmake --build build
```

The first time, `vcpkg` will automatically install dependencies into `[build directory]/vcpkg_installed/`, which will likely take about 10-20 minutes.

If you want to use `vcpkg` set up somewhere besides this repo's submodule, add `-DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake` to the `cmake` command directly or [through your IDE's settings](https://github.com/microsoft/vcpkg#using-vcpkg-with-cmake).

## Troubleshooting
If you see an error on Linux about needing `gfortran` in your path, install with `apt install gfortran`.

If the `vcpkg install` command fails, you may want to upgrade your system (e.g., `apt update`, `apt upgrade`) and try again.

## Python bindings
With the `LDSCTRLEST_BUILD_PYTHON` setting (on by default) and the `pybind11` submodule initialized, you can build Python bindings. You will probably want to specify the installation of Python to use by adding a `-DPython_ROOT_DIR=[path/to/install/dir]` argument to the CMake cache generation command (the first one). 

The bindings need to be generated just once per Python version. Once the build is complete, navigate to the `[build location]/python` folder and run `pip install .` to make it importable anywhere for your current environment. *The file structure only works correctly for this if you use a single-config generator like Ninja, though.*

Also, beware that a single build might not work for both the standalone library and the Python package, since the conversion between NumPy and Armadillo alters the way Armadillo allocates memory. 

# Reporting Issues
If you encounter bugs when using this library or have specific feature requests that you believe fall within the stated scope of this project, please [open an issue on GitHub](https://github.com/stanley-rozell/lds-ctrl-est/issues) and use an appropriate issue template where possible. You may also fork the repository and submit pull-requests with your suggested changes.

# Contributing
We welcome any community contributions to this project. Please fork the repository and if possible use `clang-format` and `clang-tidy` to conform to the coding format/style of this repository.

# Acknowledgements

Development and publication of this library was supported in part by the NIH/NINDS Collaborative Research in Computational Neuroscience (CRCNS)/BRAIN Grant 5R01NS115327-02.
