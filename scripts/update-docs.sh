#!/bin/sh

# convert doxygen xml to markdown for usage with hugo static site gen
./convert-doxygen-to-md.sh

# generate site
cd ../misc/docs-hugo && hugo && cd ../../scripts

# remove existing docs/ and add new in its place
rm -rf ../docs/* && cp -r ../misc/docs-hugo/public/* ../docs/

