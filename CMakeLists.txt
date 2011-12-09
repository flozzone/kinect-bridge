cmake_minimum_required(VERSION 2.6)

PROJECT(kinect-bridge)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# Default values
SET(NESTK_USE_FREENECT 1)
SET(NESTK_USE_OPENNI 1)

ADD_SUBDIRECTORY(external)
## nestk/UseNestk.cmake.in defines variable to use nestk.
## It itself includes nestk/deps/cmake/UseEmbeddedNestkDeps.cmake that
## contains most of the cmake find packages commands.
INCLUDE("${nestk_BINARY_DIR}/UseEmbeddedNestk.cmake")



ADD_EXECUTABLE(kinect-bridge kinect-bridge.cpp)
TARGET_LINK_LIBRARIES(kinect-bridge nestk)
