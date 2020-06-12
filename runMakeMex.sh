#!/bin/bash

make all && make all-static && sudo make install
cd mex
./build_mex_funs.sh
cd ..

