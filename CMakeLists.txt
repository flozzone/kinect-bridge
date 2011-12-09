cmake_minimum_required(VERSION 2.6)

PROJECT(kinect-bridge)


find_package(Qt4 REQUIRED QtCore QtGui QtNetwork)

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/usr/lib")

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
include(${QT_USE_FILE})


# Default values
SET(NESTK_USE_FREENECT 0)
SET(NESTK_USE_OPENNI 1)

ADD_SUBDIRECTORY(external)
ADD_SUBDIRECTORY(src)

## nestk/UseNestk.cmake.in defines variable to use nestk.
## It itself includes nestk/deps/cmake/UseEmbeddedNestkDeps.cmake that
## contains most of the cmake find packages commands.
INCLUDE("${nestk_BINARY_DIR}/UseEmbeddedNestk.cmake")
