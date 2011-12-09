#!/bin/bash

THIS=`dirname $0`

BUILD=".build"

mkdir -p $THIS/$BUILD
cd $THIS/$BUILD
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
