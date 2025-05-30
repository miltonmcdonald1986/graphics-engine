# Exit on error
$ErrorActionPreference = "Stop"

Write-Output "Removing build directory..."
Remove-Item -Recurse -Force build-ci-windows -ErrorAction SilentlyContinue

Write-Output "Running CMake..."
cmake -S . -B build-ci-windows

Write-Output "Building project..."
cmake --build build-ci-windows --config Release

Write-Output "Build completed successfully."
