#!/bin/sh

# Create directories if they don't exist
mkdir -p ../misc/docs-hugo/content/docs/api

# remove existing api docs
rm -rf ../misc/docs-hugo/content/docs/api/*

# use doxybook to convert doxygen xml to markdown for site generation
doxybook2 --input ../build/doxygen/xml --output ../misc/docs-hugo/content/docs/api --config ../misc/doxybook-hugo-config.json --templates ../misc/doxybook-templates

