#!/bin/bash

# Exit immediately if a command fails
set -e

echo "Run doxygen..."
doxygen ./Doxyfile

echo "Removing build directory..."
rm -rf build-ci-gcc

echo "Running CMake with GCC..."
cmake -S . -B build-ci-gcc -DCMAKE_C_COMPILER=gcc-13 -DCMAKE_CXX_COMPILER=g++-13 -DENABLE_COVERAGE_GCC=ON -DCMAKE_BUILD_TYPE=Release

echo "Building project..."
cmake --build build-ci-gcc --config Release

echo "Running tests..."

cwd=$(pwd)
cd build-ci-gcc/engine-tests
./engine-tests --gtest_output=xml:"$cwd/build-ci-gcc/test_output.xml"
cd "$cwd"
junit2html build-ci-gcc/test_output.xml build-ci-gcc/test_report.html

echo "Generating coverage data..."
lcov --capture --directory build-ci-gcc/engine-lib --output-file build-ci-gcc/coverage.info
lcov --remove build-ci-gcc/coverage.info '/usr/include/c++/*' --output-file build-ci-gcc/coverage.info

echo "Generating HTML coverage report..."
genhtml build-ci-gcc/coverage.info --output-directory build-ci-gcc/coverage-html

echo "CI script completed successfully."
