# !/usr/bin/bash
mex $(pkg-config ldsCtrlEst-static --cflags) glds_em_refit_mex.cpp $(pkg-config ldsCtrlEst-static --libs)

mex $(pkg-config ldsCtrlEst-static --cflags) glds_ssid_mex.cpp $(pkg-config ldsCtrlEst-static --libs)

mex $(pkg-config ldsCtrlEst-static --cflags) plds_ssid_mex.cpp $(pkg-config ldsCtrlEst-static --libs)
