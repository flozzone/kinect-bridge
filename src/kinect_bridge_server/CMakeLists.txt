FIND_PACKAGE(Boost COMPONENTS REQUIRED)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/external/nestk)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

set(kinect_bridge_server_HEADERS
)

set(kinect_bridge_server_SOURCES
        kinect_bridge_server.cpp
)

qt4_wrap_cpp(kinect_bridge_server_MOC ${kinect_bridge_server_HEADERS} OPTIONS -nw)

add_executable(kinect_bridge_server
        ${kinect_bridge_server_SOURCES}
        ${kinect_bridge_server_HEADERS}
        ${kinect_bridge_server_MOC}
)

target_link_libraries(kinect_bridge_server
        ${Boost_LIBRARIES}
        nestk
        kinect_bridge
)
