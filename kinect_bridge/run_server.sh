#!/bin/bash

THIS=`dirname $0`

pushd $THIS/../
        source env.sh
popd

killall kinect_bridge_server
killall kinect_bridge_client

pushd "${CMAKE_BINARY_DIR}/bin" 1>/dev/null
        ./kinect_bridge_server 8081 &
popd

return 0