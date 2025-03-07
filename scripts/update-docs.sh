#!/bin/sh

# convert doxygen xml to markdown for usage with hugo static site gen
./convert-doxygen-to-md.sh && cd ..

# generate site
cd misc/docs-hugo && hugo && cd ../..

# remove existing docs/ and add new in its place
rm -rf ../docs/* && cp -r misc/docs-hugo/public/* ../docs/

