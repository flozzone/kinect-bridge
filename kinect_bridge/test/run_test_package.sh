#!/bin/bash

THIS=`dirname $0`

TEST_SUITE="test_Package"
TEST_CASE=$1

if [ -n "$TEST_CASE" ]; then
	SPECIFIC="-t ${TEST_SUITE}/${TEST_CASE}"
fi

pushd $THIS/../../
	source env.sh
popd

pushd "${CMAKE_BINARY_DIR}/bin" 1>/dev/null
	./test_package ${BOOST_TEST_PARAMS} ${SPECIFIC}
popd
