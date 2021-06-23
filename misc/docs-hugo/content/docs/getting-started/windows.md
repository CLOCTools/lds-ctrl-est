# Dependencies
To configure and compile the library, you will need [Microsoft Visual Studio](https://visualstudio.microsoft.com/downloads/). After Visual Studio, install the "Desktop Development with C++" extension under *Tools->Get Tools and Features*, ensuring that the "C++ Clang Tools for Windows" and "CMake C++ Tools for Windows" options are checked.

In addition to the dependencies mentioned in [*Linux/MacOS*]({{< relref "linux-macos">}}), on Windows, this library opts to use [Intel's Math Kernel Library (MKL)](https://software.intel.com/content/www/us/en/develop/tools/oneapi/base-toolkit/download.html) for linear algebra, rather than OpenBLAS/gfotran. MKL is part of the OneAPI suite. When using the installation wizard, users may either install the entire base toolkit or just the MKL portion. After installing MKL as well as HDF5, add these libraries to the user's `PATH` environment variable (*System Properties->Advanced->Environment Variables*). Your installation location may vary, but for example, add the following lines to `PATH`: `C:\Program Files (x86)\Intel\oneAPI\mkl\latest\redist\intel64`, `C:\Program Files\HDF_Group\HDF5\1.12.0\lib` .

Once HDF5, MKL, and Visual Studio have been installed, download [armadillo](https://gitlab.com/conradsnicta/armadillo-code) as detailed previously for [*Linux/MacOS*]({{< relref "linux-macos">}}). Open the source code in Visual Studio, configure the build (*Project -> Generate CMake Cache*), and install (*Build -> Install*). Take note of the install location and add this to an environment variable called `CMAKE_PREFIX_PATH`; create this variable if it does not already exist.

# Compilation
After installing the dependencies above and downloading the `ldsCtrlEst` library source code (see [*Linux/MacOS*]({{< relref "linux-macos">}})), open this directory in Visual Studio. Configure the build (*Project -> Generate CMake Cache*) and build/install the targets (*Build -> Install*). Finally, take note of the install location and add this to the environment variable `PATH`.

# Common issues

1. "Generate CMake Cache" step errs because creating symbolic links is not permitted.

  Certain source files are sym-linked to the build/install directories during configuration with cmake. As such, your user in Windows must be permitted to do so. Make sure that your user is listed next to *Control Panel -> Administrative Tools -> Local Policies -> User Rights Assignment -> Create Symbolic Links*.

