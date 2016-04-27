
Updates by Simon Rummert

======================================
- This project was updated to OpenCV 3.1.0, please check if you have this version installed. Otherwise the project will not build.
- See "linux install commands" for instructions on how to build and install all dependencies.

- If you want to develop on OSX with Xcode, use the build_Xcode.sh file for building the project. This will create a new build folder called build_Xcode, where the xcodeproj file will reside.

- If you have OpenCV installed on a non standard folder (e.g. /home/YOU/Software/OpenCV) then you will have to tell cmake where your library is located. To do that, simply add -DOpenCV_DIR=<PATHTOYOUROPENCVDIR> to the cmake command in your build.sh file.
  EXAMPLE: cmake -DOpenCV_DIR=/home/someuser/Software/opencv-3.1.0/release/ ..


By Laurie Marceau
Please find the full documentation in the documentation folder

======================================

Program made for the course HW/SW co-design with a LEGO car (IN2106, IN0012, IN4154) at TUM, in the Winter semester 2015-2016.

This program calibrate a stereocamera (intrinsic and extrinsic), calculate a depth map and provides a top view of the depth map so the automated car can decide where to go. Can only be used with the plugged stereo camera.

=======================================

Install following libraries: 
- Boost c++ 
(sudo apt-get install libboost-system-dev libboost-thread-dev libboost-timer-dev libboost-chrono-dev )
- OpenCV
(See GeneralNotes.txt)

Boost is only for the calibration process. Once the calibration is done, the intrinsics and extrinsics files can be used without using this library.

=======================================

How to make it work:

- Go into at the root of the folder in a terminal, and make sure the file build.sh as the proper access with [sudo chmod 777 build.sh]
- Then send the command [./build.sh]
- Go into the folder build with [cd build]
- If you wanna do the calibration process send [./sv 1] but make sure you have the proper pictures taken with the stereocamera in the calibration folder. A set of at least 7 pairs is necessary.
- If you wanna just use the program, send [./sv]
- Enjoy the depth map and the top view!

=======================================

Things to think about when using the program:

- In the main.cpp verify the two camera index.
- If you use the calibration, verify the chessboard size in the cameracalibrator.hpp file (default is 9,6) and the square size of the chessboard in the cameracalibrator.cpp (default is 2.5 cm)
- To change the default matching method, go in depthsubstraction.cpp, by default it's the BM method.
- If you wanna use the SGBM algorithm, you have to change de disparity parameters, because the default parameters are changed to work with the BM algorithm only.

=======================================








