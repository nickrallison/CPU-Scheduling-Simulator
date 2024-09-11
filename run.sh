#!/bin/sh

set -e # exit on error

mkdir -p build
cd build

export ninja_installed=$(which ninja)
if [ -n "$ninja_installed" ]; then
    if [ -e Makefile ]; then
        rm -rf *
    fi
    cmake -GNinja ..
    ninja
else
    if [ -e build.ninja ]; then
        rm -rf *
    fi
    cmake ..
    make
fi

./main