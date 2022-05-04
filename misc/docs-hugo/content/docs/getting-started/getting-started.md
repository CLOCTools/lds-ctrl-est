# Getting Started

This library uses the cross-platform tool CMake to orchestrate the building and testing process on Linux, MacOS, and Windows. 

`ldsCtrlEst` requires [Armadillo](http://arma.sourceforge.net/) for linear algebra as well as [HDF5](https://www.hdfgroup.org/downloads/hdf5/) for saving output. [`vcpkg`](https://vcpkg.io/) is a cross-platform C++ package manager which allows us to easily install and use the dependencies in isolation.

## Downloading the Library

First, clone the repository along with submodules:
```
git clone --recurse-submodules https://github.com/stanley-rozell/lds-ctrl-est.git 
cd lds-ctrl-est
# use git submodule update --init if you clone the repo without --recurse-submodules
```

## Compilation + Installation

Now generate the cache and build using your IDE or from the command line as follows.
```source
mkdir build && cd build
cmake ..
cmake --build .
```

The first time, `vcpkg` will automatically install dependencies into `[build directory]/vcpkg_installed/`, which will likely take about 10-20 minutes.

If you want to use `vcpkg` set up somewhere besides this repo's submodule, add `-DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake` to the `cmake` command directly or [through your IDE's settings](https://github.com/microsoft/vcpkg#using-vcpkg-with-cmake).

You can verify the build is working by running `ctest` from the build folder, which runs all the example scripts.

## Options

This project is configured/compiled/installed by way of CMake and (on Unix-based operating systems) GNU Make. For configuration with CMake, there are three available options.
1. `LDSCTRLEST_BUILD_EXAMPLES`  : [default=ON] whether to build example programs located under `examples/` in the source tree
2. `LDSCTRLEST_BUILD_FIT`       : [default=ON] whether to build the auxiliary fitting portion of the source code that is not pertinent to control implementation
3. `LDSCTRLEST_BUILD_STATIC`    : [default=ON] whether to statically link against OpenBLAS and create a static ldsCtrlEst library for future use

*n.b., If both options 2 and 3 are enabled, Matlab/Octave mex functions will be compiled for exposing some of the fitting functionality to Matlab/Octave, assuming these programs are installed.*

Below are example usages of `cmake` to configure/build the library.
- For basic project build & install
  ```shell
  cd /path/to/repository
  mkdir build && cd build
  cmake .. #configure build
  cmake --build #build the project
  sudo make install #[optional] installs to default location (OS-specific)
  ```

- To set the install prefix
  ```shell
  cd /path/to/repository
  mkdir build && cd build
  cmake -DCMAKE_INSTALL_PREFIX=/your/install/prefix .. #configure build with chosen install location
  cmake --build #build the project
  make install #install to /your/install/prefix
  ```

- To build the *bare bones* project, excluding fit code and Matlab mex code.
  ```shell
  cd /path/to/repository
  mkdir build && cd build
  cmake -DLDSCTRLEST_BUILD_FIT=0 .. #configure not to build the fitting portion of library
  make #build the project
  ```

  *n.b.*, If you choose not to install the library or install it to the non-default location, ensure you have updated the following environment variables on Unix-based operating systems.
  1. `LD_LIBRARY_PATH`: search path for dynamically loaded libraries
  2. `PKG_CONFIG_PATH`: search path for `pkg-config` tool

On Windows, you may need to add the build location to the PATH environment variable for the library to be used elsewhere.

## Python bindings package `ldsctrlest`
With the `LDSCTRLEST_BUILD_PYTHON` setting (off by default) and the `pybind11` submodule initialized, you can build Python bindings. You will probably want to specify the installation of Python to use by adding a `-DPython_ROOT_DIR=[path/to/install/dir]` argument to the CMake cache generation command (the first one) so CMake doesn't use an undesired version. That environment needs to have NumPy installed.

The bindings need to be generated just once per Python version. Once the build is complete, navigate to the `[build location]/python` folder and run `pip install .` to make it importable anywhere for your current environment. *The file structure only works correctly for this if you use a single-config generator like Ninja or Make, though.* You can verify the installation was successful by running `pytest` from the `build/python` directory.

See `python/ldsctrlest/README.md` for usage details.

Also, beware that a single build might not work for both the standalone library and the Python package (especially on Windows), since the conversion between NumPy and Armadillo alters the way Armadillo allocates memory. In this case you may want to build once with `-DLDSCTRLEST_BUILD_PYTHON=ON`, install the package, then again with  `-DLDSCTRLEST_BUILD_PYTHON=OFF` for the pure C++ build to work correctly.

## Considerations for Windows
First of all, development on Windows has been more prone to bugs, so if you encounter many problems, consider using a Unix-based system&mdash;[WSL (Windows Subsystem for Linux)](https://docs.microsoft.com/en-us/windows/wsl/install) is a good option for Windows users who don't want to work on a different machine.

You will likely need to get compiler tools through the Visual Studio installer. Compilation was tested in VS Code using the following kit:

```
Clang 12.0.0 (GNU CLI) for MSVC 16.11.31702.278 (Visual Studio Community 2019 Release - amd64)
```

## Common issues

1. "I have built the library and installed it in a non-default location. In building my own project linking against `ldsCtrlEst`, `cmake` or `pkg-config` cannot find the library or its configuration information."

  If `cmake` and/or `pkg-config` cannot find the required configuration files for your project to link against ldsCtrlEst, make sure that these utilities know to look for them in the non-default location where you installed the library. For `cmake` this means adding your chosen install prefix to the environment variable `CMAKE_PREFIX_PATH`. Similarly, for `pkg-config` you need to add `your/install/prefix/lib/pkgconfig` to its search path, `PKG_CONFIG_PATH`. Assuming a Unix shell whose login startup file is `~/.profile` and ldsCtrlEst was installed using prefix `your/install/prefix`, add the following to `.profile`.
  ```shell
  export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/your/install/prefix
  export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/your/install/prefix
  ```

  On Windows, you will likely need to add the build or install folder to your `PATH` environment variable, which you can do using the settings GUI (search for "Edit the system environment variables").

2. On Windows, "Generate CMake Cache" step errs because creating symbolic links is not permitted.

  Certain source files are sym-linked to the build/install directories during configuration with cmake. As such, your user in Windows must be permitted to do so. Make sure that your user is listed next to *Control Panel -> Administrative Tools -> Local Policies -> User Rights Assignment -> Create Symbolic Links*.
