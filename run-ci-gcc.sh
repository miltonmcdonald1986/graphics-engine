#!/bin/bash

# Exit immediately if a command fails
set -e

echo "Removing build directory..."
rm -rf build-ci-gcc

echo "Running CMake with GCC..."
cmake -S . -B build-ci-gcc -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DENABLE_COVERAGE_GCC=ON

echo "Building project..."
cmake --build build-ci-gcc

echo "Running tests..."
ctest --test-dir build-ci-gcc/engine-tests --output-on-failure

echo "Generating coverage data..."
lcov --capture --directory build-ci-gcc --output-file build-ci-gcc/coverage.info
echo "Removing unwanted data..."
lcov --remove build-ci-gcc/coverage.info "*/engine-tests/src/*" "*/third-party/googletest/*" "/usr/*" --output-file build-ci-gcc/coverage.info
echo "Generating HTML coverage report..."
genhtml build-ci-gcc/coverage.info --output-directory build-ci-gcc/coverage-html

echo "CI script completed successfully."
