# export parameters for a boost test case call
SET(ENV{BOOST_TEST_PARAMS} "-l all -r detailed")

#####################################
###### test_cvmatserialization ######
#####################################

add_executable(test_cvmatserialization
	test_cvmatserialization.cpp
)

target_link_libraries(test_cvmatserialization
	${Boost_LIBRARIES}
	nestk
	kinect_bridge
)

#####################################
########### test_package ############
#####################################

add_executable(test_package
	test_package.cpp
)

target_link_libraries(test_package
	nestk
	kinect_bridge
)

ADD_TEST(CvMatSerialization::loadJpegFromFile
    "${CMAKE_CURRENT_SOURCE_DIR}/run_test_cvmatserialization.sh" "test_CvMatSerialization" "loadJpegFromFile")
ADD_TEST(CvMatSerialization::loadJpegSaveSerialized
    "${CMAKE_CURRENT_SOURCE_DIR}/run_test_cvmatserialization.sh" "test_CvMatSerialization" "loadJpegSaveSerialized")
ADD_TEST(CvMatSerialization::loadSerializedConvertToIplAndDisplay
    "${CMAKE_CURRENT_SOURCE_DIR}/run_test_cvmatserialization.sh" "test_CvMatSerialization" "loadSerializedConvertToIplAndDisplay")
ADD_TEST(CvMatSerialization::loadJpegSaveSerializedCompressed
    "${CMAKE_CURRENT_SOURCE_DIR}/run_test_cvmatserialization.sh" "test_CvMatSerialization" "loadJpegSaveSerializedCompressed")
ADD_TEST(CvMatSerialization::loadSerializedCompressedConvertToIplAndDisplay
    "${CMAKE_CURRENT_SOURCE_DIR}/run_test_cvmatserialization.sh" "test_CvMatSerialization" "loadSerializedCompressedConvertToIplAndDisplay")

ADD_TEST(Package::copyPackageCheckVersion
    "${CMAKE_CURRENT_SOURCE_DIR}/run_test_package.sh" "Package" "copyPackageCheckVersion")


#####################################
########### test_buffer ############
#####################################

add_executable(test_buffer
	test_buffer.cpp
)

target_link_libraries(test_buffer
	nestk
	kinect_bridge
)

ADD_TEST(Buffer::storePackageGetCheckVersion
    "${CMAKE_CURRENT_SOURCE_DIR}/run_test_package.sh" "Buffer" "storePackageGetCheckVersion")
ADD_TEST(Buffer::storePackageGetCheckVersionBoundedBuffer
    "${CMAKE_CURRENT_SOURCE_DIR}/run_test_package.sh" "Buffer" "storePackageGetCheckVersionBoundedBuffer")

