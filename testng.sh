#!/bin/bash

mkdir build
cd build
cmake ..
make
./Tests
cd ..
rm -r build