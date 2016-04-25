CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
project(sv)

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
