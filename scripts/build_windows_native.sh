#!/bin/sh
rm -rf build
mkdir -p build
cd build
cmake ../ -G 'MinGW Makefiles'
mingw32-make.exe -j$(nproc)
