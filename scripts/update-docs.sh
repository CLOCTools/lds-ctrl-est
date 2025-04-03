#!/bin/sh

# convert doxygen xml to markdown for usage with hugo static site gen
./convert-doxygen-to-md.sh

# generate site
cd ../misc/docs-hugo && hugo && cd ../../scripts

# create docs directory if it doesn't exist
mkdir -p ../docs

# update docs by copying new files and overwriting changed ones
cp -R ../misc/docs-hugo/public/* ../docs/