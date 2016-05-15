CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
project(sv)

# Check C++11 compiler
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# Find required packages
FIND_PACKAGE(OpenCV 3.1.0 EXACT REQUIRED)
set(BOOST_ROOT "/usr/share/doc/:/usr/lib/x86_64-linux-gnu/")
find_package(Boost REQUIRED system thread timer chrono filesystem)
link_directories(${Boost_LIBRARY_DIR})

# Add sources
FILE(GLOB SRCS  src/*.cpp src/methods/*.cpp inc/*.hpp inc/methods/*.hpp)

# Build
add_executable(sv ${SRCS})

TARGET_LINK_LIBRARIES(sv
${OpenCV_LIBS}
${Boost_LIBRARIES}
)
