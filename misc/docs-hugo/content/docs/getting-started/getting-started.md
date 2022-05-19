# Getting Started

This library uses the cross-platform tool CMake to orchestrate the building and testing process on Linux, MacOS, and Windows. 

`ldsCtrlEst` requires [Armadillo](http://arma.sourceforge.net/) for linear algebra as well as [HDF5](https://www.hdfgroup.org/downloads/hdf5/) for saving output. [`vcpkg`](https://vcpkg.io/) is a cross-platform C++ package manager which allows us to easily install and use the dependencies in isolation.

## N.B.
Building C++ libraries with complex dependencies can be tricky business&mdash;in our experience builds have inexplicably worked in one environment and failed in another. To save you time, sweat, and tears, we suggest you simply use one of the following setups we know work fairly reliably, using the `RelWithDebInfo` config:
- Ubuntu 18.04 with GCC 7.5 compiler
- macOS 11 (Big Sur) with Apple Clang 12 compiler
- Windows 10 with Visual Studio 16.11 (2019 release) and Clang 12 compiler

That being said, if you want to debug a build for a single platform, here are some things you can try:
- Use different compilers (or even different versions of a single compiler)
- Use different versions of vcpkg (which you can control by checking out a different commit in the vcpkg submodule)

## Mac pre-requisities

Xcode Command Line Tools will get you clang, gcc, make, and git:
```shell
xcode-select --install
```

Homebrew is "The Missing Package Manager for macOS" which will make installing lots of things easy. Install like this:
```shell
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

You can then use it to install CMake and gfortran:
```shell
brew install cmake gfortran
```

## Linux pre-requisites

You'll need Git, CMake, GCC, gfortran, etc.
```shell
sudo apt install git cmake pkg-config gfortran curl zip unzip tar build-essential
```

## Windows installation
Look [here]({{< relref "windows">}}) for Windows-specific instructions.

## Downloading the Library

First, clone the repository along with submodules:
```
git clone https://github.com/cloctools/lds-ctrl-est.git 
cd lds-ctrl-est
git submodule update --init
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
With the `LDSCTRLEST_BUILD_PYTHON` setting (off by default) and the `pybind11` submodule initialized, you can build Python bindings. You will probably want to specify the installation of Python to use by adding a `-DPython3_ROOT_DIR=[path/to/install/dir]` argument to the CMake cache generation command (the first one) so CMake doesn't use an undesired version. That environment needs to have NumPy installed.

```shell
cmake --build . --target python_modules
```

The bindings need to be generated just once per Python version. Once the build is complete, navigate to the `[build location]/python` folder and run `pip install .` to make it importable anywhere for your current environment. *The file structure only works correctly for this if you use a single-config generator like Ninja or Make, though.* You can verify the installation was successful by running `pytest` from the `build/python` directory (`pip install pytest matplotlib` first if you need to).

See `python/ldsctrlest/README.md` for usage details.

Also, beware that a single build will probably not work for both the standalone library and the Python package, since the conversion between NumPy and Armadillo alters the way Armadillo allocates memory. In this case you may want to build once with `-DLDSCTRLEST_BUILD_PYTHON=ON`, install the package, then again with  `-DLDSCTRLEST_BUILD_PYTHON=OFF` for the pure C++ build to work correctly.

## Common issues

1. "I have built the library and installed it in a non-default location. In building my own project linking against `ldsCtrlEst`, `cmake` or `pkg-config` cannot find the library or its configuration information."

  If `cmake` and/or `pkg-config` cannot find the required configuration files for your project to link against ldsCtrlEst, make sure that these utilities know to look for them in the non-default location where you installed the library. For `cmake` this means adding your chosen install prefix to the environment variable `CMAKE_PREFIX_PATH`. Similarly, for `pkg-config` you need to add `your/install/prefix/lib/pkgconfig` to its search path, `PKG_CONFIG_PATH`. Assuming a Unix shell whose login startup file is `~/.profile` and ldsCtrlEst was installed using prefix `your/install/prefix`, add the following to `.profile`.
  ```shell
  export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/your/install/prefix
  export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/your/install/prefix
  ```

2. vcpkg fails on configuration

  Try running `./bootstrap-vcpkg` from the `vcpkg` folder and try again. If that doesn't work, try updating vcpkg to a newer version (in the source control tab, click on the commit hash by the vcpkg repo then select from the dropdown) and running `boostsrap-vcpkg` again. You can also try upgrading your system (e.g., `apt update`, `apt upgrade`).

3. `Could not find Python3 (missing: Python3_NumPy_INCLUDE_DIRS NumPy)`
   
   Make sure NumPy is installed in the Python environment you specified. If CMake still can't find it, you may need to tell CMake exactly where to find it by adding an argument to the configure command: `-DPython3_NumPy_INCLUDE_DIR=...`. You can find that location like this: `python -c 'import numpy; print(numpy.get_include())'`