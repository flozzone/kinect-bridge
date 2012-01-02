#!/bin/bash

THIS=`dirname $0`

pushd $THIS/../
        source env.sh
popd

ARGS="localhost 8081 ${CMAKE_SOURCE_DIR}/kinect_bridge/log.properties"

pushd "${CMAKE_BINARY_DIR}/bin" 1>/dev/null
if [ "$1" == "-d" ]; then
	gdb --args kinect_bridge_client $ARGS
else
        ./kinect_bridge_client
fi
popd

