cmake_minimum_required(VERSION 2.8)
project( Capture )
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package( OpenCV REQUIRED )
add_definitions(-std=c++11)
add_executable( Capture capture.cpp )

target_link_libraries( Capture ${OpenCV_LIBS} Threads::Threads)

