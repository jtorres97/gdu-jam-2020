#!/bin/sh
rm -rf build
mkdir -p build
cd build
cmake ../ -DCMAKE_TOOLCHAIN_FILE=../cmake/mingw_64_toolchain.cmake
make -j$(nproc)

# TODO: zip up binary and DLLs