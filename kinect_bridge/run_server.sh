#!/bin/bash

THIS=`dirname $0`

pushd $THIS/../
        source env.sh
popd

killall kinect_bridge_server
killall kinect_bridge_client

ARGS="8081 ${CMAKE_SOURCE_DIR}/kinect_bridge/log.properties"

pushd "${CMAKE_BINARY_DIR}/bin" 1>/dev/null
if [ "$1" == "-d" ]; then 
	gdb --args kinect_bridge_server $ARGS
else
        ./kinect_bridge_server $ARGS
fi
popd

exit 0
