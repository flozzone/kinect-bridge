#!/bin/bash

THIS=`dirname $0`

pushd $THIS/../
        source env.sh
popd

pushd "${CMAKE_BINARY_DIR}/bin" 1>/dev/null
        ./kinect_bridge_client localhost 8081 ${CMAKE_SOURCE_DIR}/kinect_bridge/log.properties
popd

