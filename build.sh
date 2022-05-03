#!/bin/bash

set -xe

name=output
file=main.c

gcc -Wall -Werror -std=c11 -ggdb -o $name $file

