#!/bin/zsh
# Generates a list of sources to be used in cmake configuration.
# cmake needs the list to be semicolon-delimited
cd ../src
ls *.cpp | tr '\n' ';' > sourcelist.txt


cd ../src-fit
ls *.cpp | tr '\n' ';' > sourcelist.txt
