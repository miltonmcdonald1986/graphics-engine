#!/bin/bash

# Exit immediately if a command fails
set -e

echo "Removing build directory..."
rm -rf build-ci

echo "Running cmake..."
cmake -S . -B build-ci -DCMAKE_C_COMPILER=clang-19 -DCMAKE_CXX_COMPILER=clang++-19

filtered_files=$(jq -r '[.[] | select(.file | test("third-party/") | not)] | .[].file' build-ci/compile_commands.json)

echo "Running clang-format..."
echo $filtered_files | xargs clang-format-19 -i -style="Google"

echo "Running clang-tidy..."
echo $filtered_files | xargs clang-tidy-19 \
  --warnings-as-errors="bugprone-*,cert-*,clang-analyzer-*,cppcoreguidelines-*,modernize-*,performance-*,readability*-" \
  --checks="bugprone-*,cert-*,clang-analyzer-*,cppcoreguidelines-*,modernize-*,performance-*,readability*-" \
  --header-filter='engine-lib/include/graphics-engine/.*|engine-lib/src/.*|demo-app/src/.*' \
  -p build-ci

echo "Building project..."
cmake --build build-ci

echo "CI script completed successfully."
