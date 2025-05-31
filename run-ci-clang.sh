#!/bin/bash

# Exit immediately if a command fails
set -e

echo "Removing build directory..."
rm -rf build-ci-clang

echo "Running cmake...."
cmake -S . -B build-ci-clang -DCMAKE_C_COMPILER=clang-19 -DCMAKE_CXX_COMPILER=clang++-19

filtered_files=$(find engine-lib demo-app -type f \( -name "*.cc" -o -name "*.h" \))
echo "The list of files to feed to cppcheck, clang-format-19 and clang-tidy-19: "
echo $filtered_files

echo "Running cppcheck..."
echo "$filtered_files" | xargs cppcheck --force --language=c++ --std=c++23 --enable=style --check-level=exhaustive --inconclusive --error-exitcode=1

echo "Running clang-format..."
echo $filtered_files | xargs clang-format-19 -i -style="Google"

echo "Running clang-tidy..."
echo $filtered_files | xargs clang-tidy-19 \
  --warnings-as-errors="bugprone-*,cert-*,clang-analyzer-*,cppcoreguidelines-*,modernize-*,performance-*,readability*-" \
  --checks="bugprone-*,cert-*,clang-analyzer-*,cppcoreguidelines-*,modernize-*,performance-*,readability*-" \
  --header-filter='engine-lib/include/graphics-engine/.*|engine-lib/src/.*|demo-app/src/.*' \
  -p build-ci-clang

echo "Building project..."
cmake --build build-ci-clang --config Release

echo "CI script completed successfully."
