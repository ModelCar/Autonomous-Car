#!/bin/bash
rm -rf build_Xcode
mkdir build_Xcode
cd build_Xcode
cmake -G Xcode ..
make
