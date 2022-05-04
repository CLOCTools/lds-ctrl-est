# `ldsctrlest` Python package

See the "Getting Started" page of the `ldsCtrlEst` docs for installation instructions.

See the scripts in `examples/` to see how to use the bindings. Class and 
function names are nearly identical to those in the C++ code. In general,
you can expect to find things with this in mind:
| C++ namespace | Python (sub)package |
| ------------- | ------------------- |
| `lds`         | `ldsctrlest`        |
| `lds::gaussian`| `ldsctrlest.gaussian`|
| `lds::poisson`| `ldsctrlest.poisson`  |

Also, you can ensure the installation works correctly by running 
```
pytest tests
```