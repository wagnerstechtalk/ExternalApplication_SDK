#!/bin/bash

if [ -d "$1" ]; then
    cd $1	
    mkdir -p build
    cd build
    cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain/a53-toolchain.cmake ..
    make
fi
