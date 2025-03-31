#!/bin/sh

# convert doxygen xml to markdown for usage with hugo static site gen
./convert-doxygen-to-md.sh && cd ..

# generate site
cd misc/docs-hugo && hugo && cd ../..

# create docs directory if it doesn't exist
mkdir -p docs

# remove existing docs content and add new in its place
rm -rf docs/* && cp -r misc/docs-hugo/public/* docs/

