#!/bin/bash

rm -rf ./build
cmake -B ./build -S . -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
