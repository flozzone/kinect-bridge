cmake_minimum_required(VERSION 2.6)

PROJECT(kinect-bridge)

ENABLE_TESTING()

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/usr/lib")

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# Default values
SET(NESTK_USE_FREENECT 0)
SET(NESTK_USE_OPENNI 1)

add_definitions( "-g" )

ADD_SUBDIRECTORY(external)

## nestk/UseNestk.cmake.in defines variable to use nestk.
## It itself includes nestk/deps/cmake/UseEmbeddedNestkDeps.cmake that
## contains most of the cmake find packages commands.
INCLUDE("${nestk_BINARY_DIR}/UseEmbeddedNestk.cmake")

##############################################
############### kinect_bridge#################
##############################################

FIND_PACKAGE(Boost COMPONENTS serialization unit_test_framework iostreams REQUIRED)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

add_executable(test_cvserialization
	cvmat_serialization_test.cpp
	cvmat_serialization.h
)

target_link_libraries(test_cvserialization
	${Boost_LIBRARIES}
	nestk
)

set(kinect_bridge_HEADERS
	cvmat_serialization.h
)

set(kinect_bridge_SOURCES
	kinect-bridge.cpp
)

qt4_wrap_cpp(kinect_bridge_MOC ${kinect_bridge_HEADERS} OPTIONS -nw)

add_executable(kinect_bridge
	${kinect_bridge_HEADERS}
	${kinect_bridge_SOURCES}
	${kinect_bridge_MOC}
)

target_link_libraries(kinect_bridge
	${Boost_LIBRARIES}
	nestk
)
