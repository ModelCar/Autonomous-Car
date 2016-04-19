cmake_minimum_required(VERSION 2.8.12)

project(demo)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(demo opencltest.cpp)
target_link_libraries(demo ${OpenCV_LIBS})