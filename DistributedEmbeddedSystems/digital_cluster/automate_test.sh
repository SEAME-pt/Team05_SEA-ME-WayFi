#!/bin/bash
export $(grep -v '^#' .env | xargs)
rm -rf build
mkdir build
cd build
cmake -DBUILD_TESTS=ON ..
make 
ctest -V 