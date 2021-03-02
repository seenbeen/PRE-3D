#!/bin/bash
SRC_DIR="ProjectRedemptionEngine/src"
source ../emsdk/emsdk_env.sh
rm -rf ./Embuild
mkdir ./Embuild
mkdir ./Embuild/obj

# compile sauces
find ./ProjectRedemptionEngine -name '*.cpp' -exec emcc -c {} -D GLM_FORCE_PURE -D __PRE_DEBUG__ -O1 -s USE_SDL=2 -I ./ProjectRedemptionEngine/src -I ./deps -I ./deps/glad/include \;

# compile glad
find ./deps -name '*.c' -exec emcc -c {} -O1 -I ./deps/glad/include \;

mv *.o ./Embuild/obj
emcc -g $(find ./Embuild/obj -name '*.o' | tr '\n' ' ') -O1 -s USE_SDL=2 -s FULL_ES2=1 -s FULL_ES3=1 -o Embuild/main.html
