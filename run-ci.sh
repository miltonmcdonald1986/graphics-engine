#!/bin/bash

# Exit immediately if a command fails
set -e

wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
sudo apt-add-repository "deb http://apt.llvm.org/$(lsb_release -sc)/ llvm-toolchain-$(lsb_release -sc)-19 main"
sudo apt-get update

sudo apt-get install -y clang-19 clang-format-19 clang-tidy-19 cmake jq

# Remove the build directory if it exists
rm -rf build

# clang-format
jq -r '.[].file' build/compile_commands.json | xargs clang-format-19 -i -style="Google"

# Run CMake configuration
echo "Running CMake..."
cmake -S . -B build -DCMAKE_C_COMPILER=clang-19 -DCMAKE_CXX_COMPILER=clang++-19

run-clang-tidy-19 -warnings-as-errors="bugprone-*,cert-*,clang-analyzer-*,cppcoreguidelines-*,modernize-*,performance-*,readability*-" -checks="bugprone-*,cert-*,clang-analyzer-*,cppcoreguidelines-*,modernize-*,performance-*,readability*-" -p build

# (Optional) Build the project
echo "Building project..."
cmake --build build

echo "CI script completed successfully."

