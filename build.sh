#!/bin/bash

set -xe

mkdir -p build
cd build

name=fallen
dll_name=fallen.so

flags=(-std=gnu99 -g -w -ldl -lGL -lX11 -pthread -lXi -lm)
inc=(-I ~/Dev/libs)
src=(../src/main.c)

gcc ${inc[*]} ${src[*]} ${flags[*]} -o $name

cd ..
