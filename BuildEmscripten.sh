#!/bin/bash
SRC_DIR="ProjectRedemptionEngine/src"
source ../emsdk/emsdk_env.sh
rm -rf ./Embuild
mkdir ./Embuild
mkdir ./Embuild/obj
find ./ProjectRedemptionEngine -name '*.cpp' -exec emcc -c {} -D GLM_FORCE_PURE -O1 -s USE_SDL=2 -I ./ProjectRedemptionEngine/src -I ./deps \;
mv *.o ./Embuild/obj
emcc -g $(find ./Embuild/obj -name '*.o' | tr '\n' ' ') -O1 -s USE_SDL=2 -o Embuild/main.html
