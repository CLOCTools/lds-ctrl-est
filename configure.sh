#!/bin/sh

# Generate sourcelist
cd scripts && ./generate-lib-sourcelist.sh && cd ..

# remove cache for next cmake run
mkdir -p build
rm -f build/CMakeCache.txt

# configure the build
cmake -DLDSCTRLEST_BUILD_FIT=1 -DLDSCTRLEST_BUILD_STATIC=1 -S . -B build

