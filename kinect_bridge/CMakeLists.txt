
if(UNIX)
## nestk/UseNestk.cmake.in defines variable to use nestk.
## It itself includes nestk/deps/cmake/UseEmbeddedNestkDeps.cmake that
## contains most of the cmake find packages commands.
INCLUDE("${nestk_BINARY_DIR}/UseEmbeddedNestk.cmake")
endif(UNIX)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
LINK_DIRECTORIES("${CMAKE_BINARY_DIR}/lib")
LINK_DIRECTORIES("${CMAKE_BINARY_DIR}/lib")

# boost
if(WIN32)
	set(BOOST_ROOT "c:/Program Files/boost/boost_1_47")
	set(OpenCV_DIR "c:/Program Files/OpenCV2.3.1/opencv")
endif(WIN32)

FIND_PACKAGE(Boost COMPONENTS serialization unit_test_framework iostreams system thread signals REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

# CMP0015 - link_directories() treats paths relative to the source dir
cmake_policy(SET CMP0015 NEW)

# OpenCV
FIND_PACKAGE(OpenCV COMPONENTS core highgui)

if(WIN32)
	LINK_DIRECTORIES("c:/Program Files/OpenCV2.3.1/opencv/build/x86/vc9/lib")
endif(WIN32)

# log4cplus
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/external/log4cplus/include)


# export some variables
SET(ENV{CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR})

#######################################
########## kinect_bridge ##############
#######################################

set(kinect_bridge_HEADERS
	kbDebug.h
	cvmat_serialization.h
	package.h
	package_buffer.h
	connection.h
)

set(kinect_bridge_SOURCES
	kbDebug.cpp
	package.cpp
	package_buffer.cpp
)

add_library(kinect_bridge
	${kinect_bridge_HEADERS}
	${kinect_bridge_SOURCES}
)

target_link_libraries(kinect_bridge
	${Boost_LIBRARIES}
	log4cplus
)

#######################################
####### kinect_bridge_client ##########
#######################################

set(kinect_bridge_client_SOURCES
	client.cpp
)

set(kinect_bridge_client_HEADERS
	client.h
)

ADD_EXECUTABLE(kinect_bridge_client
	${kinect_bridge_client_SOURCES}
	${kinect_bridge_client_HEADERS}
)

TARGET_LINK_LIBRARIES(kinect_bridge_client
	${Boost_LIBRARIES}
	${OpenCV_LIBS}
	kinect_bridge
)

IF(UNIX)

    #######################################
    ####### kinect_bridge_server ##########
    #######################################

    set(kinect_bridge_server_SOURCES
	    server.cpp
    )

    set(kinect_bridge_server_HEADERS
	    server.h
    )

    ADD_EXECUTABLE(kinect_bridge_server
	    ${kinect_bridge_server_SOURCES}
	    ${kinect_bridge_server_HEADERS}
    )

    TARGET_LINK_LIBRARIES(kinect_bridge_server
	    ${Boost_LIBRARIES}
	    nestk
	    kinect_bridge
    )

    #######################################
    ######## kinect_bridge_demo ###########
    #######################################

    set(kinect_bridge_demo_HEADERS
	    cvmat_serialization.h
	    #kinect_bridge.h
    )

    set(kinect_bridge_demo_SOURCES
	    demo.cpp
    )

    qt4_wrap_cpp(kinect_bridge_demo_MOC ${kinect_bridge_demo_HEADERS} OPTIONS -nw)

    add_executable(kinect_bridge_demo
	    ${kinect_bridge_demo_HEADERS}
	    ${kinect_bridge_demo_SOURCES}
	    ${kinect_bridge_demo_MOC}
    )

    target_link_libraries(kinect_bridge_demo
	    ${Boost_LIBRARIES}
	    kinect_bridge
	    nestk
    )
ENDIF(UNIX)

if(UNIX)
ADD_SUBDIRECTORY(test)
endif(UNIX)
