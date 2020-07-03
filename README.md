# cpp-ldsCtrlEst
A C++ library for estimation and control of linear dynamical systems (LDS) with Gaussian, Poisson[, or Bernoulli] observations.

This library currently provides three namespaces.
 - `lds` : linear dynamical systems (no output/observation)
 - `lds::gaussian` (aka `glds`) : linear dynamical systems with Gaussian observations
 - `lds::poisson` (aka `plds`) : linear dynamical systems with Poisson observations

Future iterations may include an additional namespace for LDS with Bernoulli (`blds`) observations.

# Linear Dynamical System
	x_{t+1} = A x_{t} + B v_{t} + m + w_{t}

where

	x : state
	v = g%u : input (in units used for model fit)
	u : control signal sent to actuator
	m : process disturbance
	w ~ N(0, Q) : process noise/disturbance

	A : state transition matrix
	B : input coupling matrix
	g : input gain (for converting to control signal actuator voltage and/or adaptive control)

# Output (y) & Observation/Measurement (z)
## Gaussian
	y_{t} = h( x_{t} ) = C x_{t} + d
	z_{t} ~ N(y_{t}, R)

	C : output matrix
	d : output bias
	y : output
	R : measurement noise covariance
	z : measurement

## Poisson
	y_{t} = h( x_{t} ) = exp( C x_{t} + d )
	z_{t} ~ Poisson(y_{t})

# Notes on design
 - All dynamical systems **with observations** (Gaussian, Poisson[, or Bernoulli]) are built upon class definitions for the underlying linear dynamical system (`lds::sys_t`).
 - In each namespace, there are generic system types defined (`lds::sys_t`, `glds::sys_t`, `plds::sys_t`) that include functionality for parameter augmentation, one-step prediction, etc.
 - Additionally, in namespaces with observation equations (`glds`,`plds`), system types include functionality for updating estimates of states/parameters, given current measurements (*i.e.*, filtering).
 - In namespaces for systems with observation equations (`glds`,`plds`), controller types (`ctrl_t`) are built on top of that namespace's system type. Feedback control functionality (as well as previously-defined online estimation) is provided by these controller types.
 - Use of `set*` methods: In order to ensure dimensionalities always match internally and to use intention in handling any mismatches that may be present in parameters, signals, etc., every property of a system/controller class is `protected`. Set methods are defined so users can define parameters in a safe way. Note that this layer of set methods slowed the library down by 0.2x, but the consistency of dimensions and how inconsistencies are handled is of great importance.

# Notes on Repository Organization
- Header files are under `include/ldsEstCtrl_h`
- Source files are under `src/`.
- Wrappers for exposing functions to matlab as matlab executables are located under `mex/`.
- Test programs/visualization scripts are located under `test/`.

# Dependencies
-  Linear algebra library [`armadillo`](http://arma.sourceforge.net/) is used throughout this repository. Either install binaries via package manager or from [source](https://gitlab.com/conradsnicta/armadillo-code).
- For debugging, the [`matio`](https://github.com/tbeu/matio) C library is used in the test programs to save signals in MATLAB `.mat` files. You can install this with many package managers.
-  For use with Matlab in Linux (see mex functions), you may need to compile [OpenBlas](http://www.openblas.net/) from source and make a static library (`libldsCtrlEst.a`) using objects from this repository as well as unarchived objects in `libopenblas.a`. Certain package managers may allow you to install the binaries for static libraries from which you can extract objects using `ar -x` command, rather than compiling from source yourself. However you get these objects, put the OpenBlas objects under `build/static-objects/openblas/`, as the `make all-static` rule is looking for files in this directory.

# Compiling + Installation
- For basic functionality, compile as a dynamic (AKA shared) library and install.
 ```bash
make all && sudo make install
```
- For running from Matlab in Linux, build a static version of this library that includes the needed blas functionality.
```bash
make all-static && sudo make install
```
- On MacOS, the installation directory is set to `/opt/local/lib` which may not be in your system runtime search path by default. For running programs that dynamically link with this library, you may need to create/append to the `DYLD_LIBRARY_PATH` environment variable in your preferred shell configuration script. For linking matlab mex functions, you will need to include something like
```sh
mex ... LDFLAGS='$LDFLAGS -Wl,-rpath,/opt/local/lib -L/opt/local/lib' ... mex_fn_name.cpp
```

- On Linux, the installation directory is set to the more commonly used `/usr/lib`, so this should already be in the runtime search path.
