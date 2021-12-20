#!/bin/bash

### Create log directory
if [ -d log ]; then
    rm -r log
fi

mkdir log

### Create directory for build informaton
if [ -d build ]; 
then
    rm -r build
fi

mkdir build
cd build

cmake ..
make

### Leave build directory
cd ..