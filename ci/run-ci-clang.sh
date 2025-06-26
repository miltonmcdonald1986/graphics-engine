#!/bin/bash

# Exit immediately if a command fails
set -e

# Default options
KEEP_BUILD=false
SKIP_CLANG_TIDY=false
SKIP_CPPCHECK=false

# Parse command-line switches
for arg in "$@"; do
    if [[ "$arg" == "--keep-build" ]]; then
        KEEP_BUILD=true
	elif [[ "$arg" == "--skip-clang-tidy" ]]; then
        SKIP_CLANG_TIDY=true
    elif [[ "$arg" == "--skip-cppcheck" ]]; then
        SKIP_CPPCHECK=true
    fi
done

# Remove build directory unless "--keep-build" is passed
if [ "$KEEP_BUILD" == false ]; then
    echo "Removing build directory..."
    rm -rf build-ci-clang
else
    echo "Skipping build directory removal."
fi

echo "Running cmake (clang-19)...."
cmake -S . -B build-ci-clang -DCMAKE_C_COMPILER=clang-19 -DCMAKE_CXX_COMPILER=clang++-19 -DCMAKE_BUILD_TYPE=Release

if [ "$SKIP_CPPCHECK" == false ]; then
	filtered_files=$(find engine-lib demo-app -type f \( -name "*.cc" \))
	echo "The list of files to feed to cppcheck: "
	echo $filtered_files

	echo "Running cppcheck..."
	echo "$filtered_files" | xargs cppcheck --force --language=c++ --std=c++23 --enable=style --check-level=exhaustive --inconclusive --error-exitcode=1
else
	echo "Skipping cppcheck."
fi

filtered_files=$(find engine-lib demo-app -type f \( -name "*.cc" -o -name "*.h" \))
echo "The list of files to feed to clang-format-19 and clang-tidy-19: "
echo $filtered_files

echo "Running clang-format..."
echo $filtered_files | xargs clang-format-19 -i -style="Google"

if [ "$SKIP_CLANG_TIDY" == false ]; then
	echo "Running clang-tidy..."
	echo $filtered_files | xargs clang-tidy-19 -p build-ci-clang
else
	echo "Skipping clang-tidy"
fi

echo "Building project..."
cmake --build build-ci-clang --config Release

echo "CI script completed successfully."
