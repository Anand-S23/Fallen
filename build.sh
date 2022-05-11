#!/bin/bash

set -xe

rm -rf build
mkdir build
cd build

name=fallen

flags=(-std=gnu99 -w -ldl -lGL -lX11 -pthread -lXi -lm)
inc=(-I ~/Dev/libs)
src=(../src/main.c)

gcc ${inc[*]} ${src[*]} ${flags[*]} -o $name

cd ..
