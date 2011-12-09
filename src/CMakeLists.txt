##############################################
############### kinect_bridge#################
##############################################

FIND_PACKAGE(Boost COMPONENTS serialization unit_test_framework REQUIRED)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

set(kinect_bridge_HEADERS
        cvmat_serialization.h
)

set(kinect_bridge_SOURCES
        cvmat_serialization_test.cpp
)

qt4_wrap_cpp(kinect_bridge_MOC ${kinect_bridge_HEADERS} OPTIONS -nw)

ADD_LIBRARY(kinect_bridge STATIC
        ${kinect_bridge_SOURCES}
	${kinect_bridge_HEADERS}
	${kinect_bridge_MOC}
)

TARGET_LINK_LIBRARIES(kinect_bridge
	${Boost_LIBRARIES}
        nestk
)

ADD_SUBDIRECTORY(kinect_bridge_server)
