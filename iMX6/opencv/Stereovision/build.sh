#!/bin/bash
rm -rf build
mkdir build
cd build
cmake -DOpenCV_DIR=/home/laurie/Software/opencv-3.0.0/release/ ..
make
