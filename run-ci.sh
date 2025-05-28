#!/bin/bash

# Exit immediately if a command fails
set -e

# Enumerate dependencies
echo "The following dependencies are required for this scipt to succeed..."
echo "clang-19"
echo "clang-format-19"
echo "clang-tidy-19"
echo "cmake"
echo "jq"

echo "Removing build directory..."
rm -rf build

echo "Running cmake..."
cmake -S . -B build -DCMAKE_C_COMPILER=clang-19 -DCMAKE_CXX_COMPILER=clang++-19

echo "Running clang-format..."
jq -r '.[].file' build/compile_commands.json | xargs clang-format-19 -i -style="Google"

echo "Running clang-tidy..."
run-clang-tidy-19 -warnings-as-errors="bugprone-*,cert-*,clang-analyzer-*,cppcoreguidelines-*,modernize-*,performance-*,readability*-" -checks="bugprone-*,cert-*,clang-analyzer-*,cppcoreguidelines-*,modernize-*,performance-*,readability*-" -p build

echo "Building project..."
cmake --build build

echo "CI script completed successfully."
