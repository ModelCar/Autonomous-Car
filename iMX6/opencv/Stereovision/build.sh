#!/bin/bash
rm -rf build
mkdir build
cd build
cmake -DOpenCV_DIR=/Users/Simon/Desktop/opencv-3.1.0/build/ ..
make
