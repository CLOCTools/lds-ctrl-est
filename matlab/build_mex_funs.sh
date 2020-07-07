# !/usr/bin/bash
if [[ "$OSTYPE" == "darwin"* ]]; then
  # LDFLAGS="$LDFLAGS -Wl,-rpath,/opt/local/lib $(pkg-config ldsCtrlEst --libs)"
  # mex $(pkg-config ldsCtrlEst-static --cflags) LDFLAGS='$LDFLAGS' glds_ssid_mex.cpp

  # TODO: should not have to hard code the below! But the above doesn't work.
  mex -I/opt/local/include LDFLAGS='$LDFLAGS -Wl,-rpath,/opt/local/lib -L/opt/local/lib -lldsCtrlEst -larmadillo' glds_ssid_mex.cpp
  mex -I/opt/local/include LDFLAGS='$LDFLAGS -Wl,-rpath,/opt/local/lib -L/opt/local/lib -lldsCtrlEst -larmadillo' glds_em_refit_mex.cpp
  mex -I/opt/local/include LDFLAGS='$LDFLAGS -Wl,-rpath,/opt/local/lib -L/opt/local/lib -lldsCtrlEst -larmadillo' plds_ssid_mex.cpp
else
  mex $(pkg-config ldsCtrlEst-static --cflags) glds_em_refit_mex.cpp $(pkg-config ldsCtrlEst-static --libs)
  mex $(pkg-config ldsCtrlEst-static --cflags) glds_ssid_mex.cpp $(pkg-config ldsCtrlEst-static --libs)
  mex $(pkg-config ldsCtrlEst-static --cflags) plds_ssid_mex.cpp $(pkg-config ldsCtrlEst-static --libs)
fi
