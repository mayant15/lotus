#!/usr/bin/env bash

mkdir build
cd build

conan install ..
cmake .. -G Ninja
cmake --build .
