#!/usr/bin/env bash

mkdir build
cd build

conan install ..
cmake .. -G Ninja -DLOTUS_BUILD_EXAMPLE=OFF
cmake --build .
