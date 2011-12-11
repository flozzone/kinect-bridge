# export parameters for a boost test case call
SET(ENV{BOOST_TEST_PARAMS} "-l all -r detailed")

add_executable(test_cvmatserialization
	test_cvmatserialization.cpp
)

target_link_libraries(test_cvmatserialization
	${Boost_LIBRARIES}
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

