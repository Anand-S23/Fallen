#!/bin/bash

set -xe

mkdir -p build
cd build

name=fallen
dll_name=fallen.so

flags=(-std=gnu99 -w -ldl -lGL -lX11 -pthread -lXi -lm)
inc=(-I ~/Dev/libs)
src=(../src/main.c)
files=(../src/game.c)

# NOTE: Execute this command whenever gunslinger is updated
# gcc ${inc[*]} ~/Dev/libs/gs.c ${flags[*]} -shared -fPIC -o gs.dll

gcc ${inc[*]} ${files[*]} ${flags[*]} ~/Dev/libs/gs.dll -shared -o $dll_name
gcc ${inc[*]} ${src[*]} ${flags[*]} ~/Dev/libs/gs.dll -o $name

cd ..
