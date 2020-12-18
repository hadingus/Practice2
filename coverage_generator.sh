#!/bin/bash

rm -r build
mkdir build
cd build
cmake ..
make
./Tests
cd ..

lcov --directory ./build/CMakeFiles/Tests.dir/ --capture --output-file ./coverage.info -rc lcov_branch_coverage=1
genhtml coverage.info --branch-coverage --output-directory ./code_coverage_report/