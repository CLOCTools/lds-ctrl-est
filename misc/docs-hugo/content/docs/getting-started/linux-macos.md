# Downloading the Library
The source code for this library can be downloaded from [stanley-rozell/ldsCtrlEst](https://github.com/stanley-rozell/ldsCtrlEst) either by downloading a snapshot or cloning the repository via git.
```shell
git clone https://github.com/stanley-rozell/ldsCtrlEst.git
```
By default, this would check out the master branch. In most cases, we suggest downloading or checking out the latest release instead.
```shell
cd /path/to/ldsCtrlEst
git checkout 0.6
```

# Dependencies
Note that the primary dependencies of this project listed below must be installed along with their header files and with CMake config files or pkg-config files. The latter files are used to configure this project's build. It is strongly encouraged to install the dependencies below using a package manager (*e.g.*, apt, pacman, macports).

- For project configuration, install [`cmake`](https://cmake.org/) as well as [`pkg-config`](https://gitlab.freedesktop.org/pkg-config/pkg-config). The latter is optional.
- The linear algebra library [`armadillo`](http://arma.sourceforge.net/) is used throughout this repository.
- The [HDF5](https://www.hdfgroup.org/downloads/hdf5/) library is used to save output from example test programs.
- For use of this library in Matlab executables (mex) on Linux operating systems, you will need [OpenBlas](http://www.openblas.net/), ensuring the *static* library `libopenblas.a` is installed. You will also need to install [`gfortran`](https://gcc.gnu.org/fortran/).

# Compilation + Installation
This project is configured/compiled/installed by way of CMake and (on Unix-based operating systems) GNU Make. For configuration with CMake, there are three available options.
1. `LDSCTRLEST_BUILD_EXAMPLES`  : [default= ON] whether to build example programs located under `examples/` in the source tree
2. `LDSCTRLEST_BUILD_FIT`       : [default=ON] whether to build the auxiliary fitting portion of the source code that is not pertinent to control implementation
3. `LDSCTRLEST_BUILD_STATIC`    : [default=ON] whether to statically link against OpenBLAS and create a static ldsCtrlEst library for future use

*n.b., If both options 2 and 3 are enabled, Matlab/Octave mex functions will be compiled for exposing some of the fitting functionality to Matlab/Octave, assuming these programs are installed.*

Below are example usages of `cmake`/`make` to configure/build the library.
- For basic project build & install
  ```shell
  cd /path/to/repository
  mkdir build && cd build
  cmake .. #configure build
  make #build the project
  sudo make install #[optional] installs to default location (OS-specific)
  ```

- To set the install prefix
  ```shell
  cd /path/to/repository
  mkdir build && cd build
  cmake -DCMAKE_INSTALL_PREFIX=/your/install/prefix .. #configure build with chosen install location
  make #build the project
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
  3. `CMAKE_PREFIX_PATH`: search path of prefix where CMake will look for package config files

  *e.g.*, Assuming you set `-DCMAKE_INSTALL_PREFIX=/your/install/prefix` during project configuration and your login shell uses the `~/.profile` startup file, open `~/.profile` in a text editor and add ...
  ```shell
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/your/install/prefix/lib
  export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/your/install/prefix/lib/pkgconfig
  export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/your/install/prefix
  ```

# Common issues

1. "I have installed all the dependencies including `gfortran` with a package manager as suggested; however, `cmake` complains it cannot find the `gfortran` library."

  When `gfortran` is installed, its library is usually *not* installed in a standard location like `/usr/lib`. `gfortran` is part of the `gcc` suite, so their libraries are organized together. e.g., When you install `gfortran` on Ubuntu using `apt`, its location is `/usr/lib/gcc/x86_64-linux-gnu/9`, in the case that `gcc` version 9 is installed. The build configuration script in ldsCtrlEst is written to add `LD_LIBRARY_PATH` (Unix) or `PATH` (Windows) to the CMake library search path on Unix or Windows systems, respectively. Therefore, to fix this issue, simply add the directory in which `libgfortran` was installed to the OS-appropriate environment variable. Continuing with the Ubuntu example above and assuming a Unix login shell whose startup file is `~/.profile`, add the following to the file.
  ```shell
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/gcc/x86_64-linux-gnu/9
  ```

2. "I have built the library and installed it in a non-default location. In building my own project linking against `ldsCtrlEst`, `cmake` or `pkg-config` cannot find the library or its configuration information."

  If `cmake` and/or `pkg-config` cannot find the required configuration files for your project to link against ldsCtrlEst, make sure that these utilities know to look for them in the non-default location where you installed the library. For `cmake` this means adding your chosen install prefix to the environment variable `CMAKE_PREFIX_PATH`. Similarly, for `pkg-config` you need to add `your/install/prefix/lib/pkgconfig` to its search path, `PKG_CONFIG_PATH`. Assuming a Unix shell whose login startup file is `~/.profile` and ldsCtrlEst was installed using prefix `your/install/prefix`, add the following to `.profile`.
  ```shell
  export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/your/install/prefix
  export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/your/install/prefix
  ```
