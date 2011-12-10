## nestk/UseNestk.cmake.in defines variable to use nestk.
## It itself includes nestk/deps/cmake/UseEmbeddedNestkDeps.cmake that
## contains most of the cmake find packages commands.
INCLUDE("${nestk_BINARY_DIR}/UseEmbeddedNestk.cmake")

FIND_PACKAGE(Boost COMPONENTS serialization unit_test_framework iostreams system REQUIRED)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
LINK_DIRECTORIES("${CMAKE_BINARY_DIR}/lib")

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

#######################################
########## kinect_bridge ##############
#######################################

set(kinect_bridge_HEADERS
	cvmat_serialization.h
	#kinect_bridge.h
)

set(kinect_bridge_SOURCES
	#kinect_bridge.cpp
)

qt4_wrap_cpp(kinect_bridge_MOC ${kinect_bridge_HEADERS} OPTIONS -nw)

add_library(kinect_bridge
	${kinect_bridge_HEADERS}
	${kinect_bridge_SOURCES}
	${kinect_bridge_MOC}
)

target_link_libraries(kinect_bridge
	${Boost_LIBRARIES}
)

#######################################
####### kinect_bridge_client ##########
#######################################

set(kinect_bridge_client_SOURCES
	kinect_bridge_client.cpp
)

set(kinect_bridge_client_HEADERS
	kinect_bridge_connection.hpp
)

ADD_EXECUTABLE(kinect_bridge_client
	${kinect_bridge_client_SOURCES}
	${kinect_bridge_client_HEADERS}
)

TARGET_LINK_LIBRARIES(kinect_bridge_client
	kinect_bridge
	nestk
	boost_system
)

#######################################
####### kinect_bridge_server ##########
#######################################

set(kinect_bridge_server_SOURCES
	kinect_bridge_server.cpp
)

set(kinect_bridge_server_HEADERS
	kinect_bridge_connection.hpp
)

ADD_EXECUTABLE(kinect_bridge_server
	${kinect_bridge_server_SOURCES}
	${kinect_bridge_server_HEADERS}
)

TARGET_LINK_LIBRARIES(kinect_bridge_server
	kinect_bridge
	nestk
	boost_system
)

#######################################
######## kinect_bridge_demo ###########
#######################################

set(kinect_bridge_demo_HEADERS
	cvmat_serialization.h
	#kinect_bridge.h
)

set(kinect_bridge_demo_SOURCES
	kinect_bridge_demo.cpp
)

qt4_wrap_cpp(kinect_bridge_demo_MOC ${kinect_bridge_demo_HEADERS} OPTIONS -nw)

add_library(kinect_bridge_demo
	${kinect_bridge_demo_HEADERS}
	${kinect_bridge_demo_SOURCES}
	${kinect_bridge_demo_MOC}
)

target_link_libraries(kinect_bridge_demo
	${Boost_LIBRARIES}
	nestk
	kinect_bridge
)

ADD_SUBDIRECTORY(test)
