#!/bin/bash

THIS=`dirname $0`

TEST_SUITE=$1
TEST_CASE=$2

SPECIFIC="-t ${TEST_SUITE}/${TEST_SUITE}"
if [ -n "$TEST_CASE" ]; then
	SPECIFIC="${SPECIFIC}/${TEST_CASE}"
fi

pushd $THIS/../../
	source env.sh
popd

pushd "${CMAKE_BINARY_DIR}/bin" 1>/dev/null
	./test_buffer ${BOOST_TEST_PARAMS} ${SPECIFIC}
popd
