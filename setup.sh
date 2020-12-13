#!/usr/bin/env bash

# This script should run on a fresh clone to install all dependencies
# You should not need to run this more that once.

set -e

cd ./external/vcpkg

# Bootstrap vcpkg
chmod +x bootstrap-vcpkg.sh
source bootstrap-vcpkg.sh

# Install dependencies
./vcpkg install assimp:x64-linux
./vcpkg install glfw3:x64-linux
./vcpkg install pybind11:x64-linux
./vcpkg install spdlog:x64-linux
./vcpkg install physx:x64-linux
