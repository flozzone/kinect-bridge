#!/bin/bash

THIS=`dirname $0`

BUILD=".build"

LIBUSB_1_INCLUDE_DIR=/usr/include/libusb-1.0

mkdir -p $THIS/$BUILD
cd $THIS/$BUILD
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
