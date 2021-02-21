#!/bin/bash
SRC_DIR="ProjectRedemption/src"
source ../emsdk/emsdk_env.sh
rm -rf Embuild
mkdir Embuild
mkdir Embuild/temp
emcc -c $SRC_DIR/main.cpp -s USE_SDL=2 -o Embuild/temp/main.o
emcc -g Embuild/temp/main.o -s USE_SDL=2 -o Embuild/main.html
rm -rf Embuild/temp
