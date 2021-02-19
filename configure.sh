#!/bin/sh

# Generate sourcelist
cd scripts
./generate_lib_sourcelist.sh
cd ..

# remove cache for next cmake run
rm -f build/CMakeCache.txt
