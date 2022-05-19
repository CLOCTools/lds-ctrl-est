# Windows installation

## Windows pre-requisites
Scoop is a very handy tool for easily installing all sorts of command-line applications. Install like this: 

```shell
Set-ExecutionPolicy RemoteSigned -Scope CurrentUser # Optional: Needed to run a remote script the first time
iwr get.scoop.sh | Invoke-Expression
```

Install Git and CMake if you don't already have them:
```shell
scoop install git cmake
```

If that didn't work, follow more detailed instructions [here](https://github.com/ScoopInstaller/Install#readme).

The easiest way to compile C++ project on Windows is with Visual Studio's build tools, which you can download [here](https://visualstudio.microsoft.com/downloads/) (or [here](https://visualstudio.microsoft.com/vs/older-downloads/) for the 2019 release which we tested&mdash;make sure you get the most recent one, e.g., 16.11 at time of writing). In the installer, click on "Desktop development with C++." If you want to build Python bindings, you will need to use the Clang compiler, which you can add on the "Installation details" sidebar under optional features.

And the easiest way to use Visual Studio's build tools is with [VS Code](https://code.visualstudio.com/), along with the [CMake Tools extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools). Install them and you should be ready to go.

## Downloading the Library

First, clone the repository, either from VS Code or the command line:
```shell
git clone https://github.com/cloctools/lds-ctrl-est.git 
cd lds-ctrl-est
```

You'll need to initialize the submodules from the command line after the repo is cloned:
```shell
git submodule update --init
```

## Installation
When you open the folder in VS Code, you will like be prompted by the CMake Tools extension to configure the project. Make sure you select the kit (you'll be prompted when you configure--else there's an icon in the bar on the bottom of the window or type `Ctrl+Shift+P`, then "cmake select kit"). Choose `Clang [latest version] with GNU CLI ... amd64` assuming you are running a 64-bit OS. (MSVC may work okay too if you don't need to build Python bindings.)

Follow along with the "Getting Started" instructions, but where you see config options specified as `-DLDSCTREST_BUILD_STATIC=OFF` or `-DPython3_ROOT_DIR=...`, you will enter those in settings: open with `Ctrl+,`, click "workspace", then search for "CMake: Configure Args" and enter each of your desired arguments as a separate item.

To configure, use `Ctrl+Shift+P` and search for the "CMake: Configure" command. To build, click the "Build" button on the bottom bar. Then click the "CTest" button to run the example scripts.

## Considerations
Development on Windows has been more prone to bugs than on Unix systems, so if you encounter many problems, consider switching&mdash;[WSL (Windows Subsystem for Linux)](https://docs.microsoft.com/en-us/windows/wsl/install) is a good option for Windows users who don't want to work on a different machine.

Compilation has been successfully tested in VS Code using the following kit, **using the "RelWithDebInfo" config**:

```
Clang 12.0.0 (GNU CLI) for MSVC 16.11.31702.278 (Visual Studio Community 2019 Release - amd64)
```


## Troubleshooting
1. The build appears to work, but tests fail with code `0xc0000135` OR "I have built the library and installed it in a non-default location. In building my own project linking against `ldsCtrlEst`, `cmake` or `pkg-config` cannot find the library or its configuration information." 

  Have you installed the library? In VS Code, use Shift+F7 to build a specific target, in this case `INSTALL`. If that doesn't solve your problem, you will likely need to add the build or install folder to your `PATH` environment variable, which you can do using the settings GUI (search for "Edit the system environment variables").

2. On Windows, "Generate CMake Cache" step errs because creating symbolic links is not permitted.

  Certain source files are sym-linked to the build/install directories during configuration with cmake. As such, your user in Windows must be permitted to do so. Make sure that your user is listed next to *Control Panel -> Administrative Tools -> Local Policies -> User Rights Assignment -> Create Symbolic Links*.

