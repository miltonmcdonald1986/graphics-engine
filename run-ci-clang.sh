#!/bin/bash

# Exit immediately if a command fails
set -e

echo "Removing build directory..."
rm -rf build-ci-clang

echo "Running cmake...."
cmake -S . -B build-ci-clang -DCMAKE_C_COMPILER=clang-19 -DCMAKE_CXX_COMPILER=clang++-19

# filtered_files=$(jq -r '[.[] | select(.file | test("third-party/") | not)] | .[].file' build-ci-clang/compile_commands.json)
filtered_files=$(find engine-lib/include/graphics-engine -name "*.h") 
filtered_files+=" $(find engine-lib/src -name "*.h" -o -name "*.cc")"
filtered_files+=" $(find demo-app/src -name "*.h" -o -name "*.cc")"

echo "Running cppcheck..."
echo "$filtered_files" | xargs cppcheck --force --language=c++ --std=c++23 --enable=style --check-level=exhaustive -i build -i build-ci-clang -i build-ci-gcc -i build-ci-windows -i tests -i third-party --inconclusive --error-exitcode=1 .

echo "Running clang-format..."
echo $filtered_files | xargs clang-format-19 -i -style="Google"

echo "Running clang-tidy..."
echo $filtered_files | xargs clang-tidy-19 \
  --warnings-as-errors="bugprone-*,cert-*,clang-analyzer-*,cppcoreguidelines-*,modernize-*,performance-*,readability*-" \
  --checks="bugprone-*,cert-*,clang-analyzer-*,cppcoreguidelines-*,modernize-*,performance-*,readability*-" \
  --header-filter='engine-lib/include/graphics-engine/.*|engine-lib/src/.*|demo-app/src/.*' \
  -p build-ci-clang

echo "Building project..."
cmake --build build-ci-clang

echo "CI script completed successfully."
