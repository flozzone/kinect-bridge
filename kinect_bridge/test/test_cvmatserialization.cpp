#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "kinect_bridge/portable_binary_iarchive.hpp"
#include "kinect_bridge/portable_binary_oarchive.hpp"

#include <boost/archive/archive_exception.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <fstream>
#include <opencv2/opencv.hpp>

#ifdef KINECT_DEBUG
#include <ntk/camera/openni_grabber.h>
#endif

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test_CvMatSerialization
#include <boost/test/unit_test.hpp>

#include "kinect_bridge/cvmat_serialization.h"

#define KINECT_BRIDGE_TEST_IMAGE "image.jpeg"


//#define OARCHIVE boost::archive::binary_oarchive
//#define IARCHIVE boost::archive::binary_iarchive
//#define OARCHIVE boost::archive::text_oarchive
//#define IARCHIVE boost::archive::text_iarchive
#define OARCHIVE portable_binary_oarchive
#define IARCHIVE portable_binary_iarchive

#define STREAM_FLAGS  | std::ios::binary
#define SOARCHIVE OARCHIVE oa(ofs);
#define SIARCHIVE IARCHIVE ia(ifs);
#define FOARCHIVE OARCHIVE oa(out);
#define FIARCHIVE IARCHIVE ia(in);

using namespace std;

struct Fixture {
#ifdef KINECT_DEBUG
    ntk::OpenniGrabber *grabber;
#endif
    cv::Mat tempImage;

    Fixture() {
	BOOST_TEST_MESSAGE("Fixture setup");
#ifdef KINECT_DEBUG
	// Set debug level to 1.
	ntk::ntk_debug_level = 1;

	// Declare the global OpenNI driver. Only one can be instantiated in a program.
	ntk::OpenniDriver ni_driver;

	// Declare the frame grabber.
	grabber = new ntk::OpenniGrabber(ni_driver, 0);

	// Start the grabber.
	grabber->connectToDevice();
	grabber->start();
#endif //KINECT_DEBUG
    }
    ~Fixture() {
	BOOST_TEST_MESSAGE("Fixture teardown");
#ifdef KINECT_DEBUG
	grabber->stop();
	delete grabber;
#endif //KINECT_DEBUG
    }
};


BOOST_FIXTURE_TEST_SUITE(test_CvMatSerialization, Fixture)

BOOST_AUTO_TEST_CASE(loadJpegFromFile)
{
    IplImage *image = 0;
    image = cvLoadImage(KINECT_BRIDGE_TEST_IMAGE);

    BOOST_CHECK(image != 0);

    BOOST_CHECK(this->tempImage.empty() == true);

    cv::Mat tmp(image);
    tmp.copyTo(this->tempImage);

    BOOST_CHECK(this->tempImage.empty() == false);
    cvReleaseImage(&image);
    BOOST_CHECK(this->tempImage.empty() == false);
}

BOOST_AUTO_TEST_CASE(loadJpegSaveSerialized)
{

    IplImage *image = 0;
    image = cvLoadImage(KINECT_BRIDGE_TEST_IMAGE);

    BOOST_CHECK(image != 0);

    BOOST_CHECK(this->tempImage.empty() == true);

    cv::Mat tmp(image);
    tmp.copyTo(this->tempImage);

    BOOST_CHECK(this->tempImage.empty() == false);
    cvReleaseImage(&image);
    BOOST_CHECK(this->tempImage.empty() == false);

    std::ofstream ofs("test_matrices_JpegToMat.bin", std::ios::out STREAM_FLAGS);

    try { // use scope to ensure archive goes out of scope before stream
	SOARCHIVE;
	oa << this->tempImage;
    } catch (boost::archive::archive_exception e) {
	BOOST_FAIL(string("archive_exception:") << e.what());
    }

    ofs.close();

}

BOOST_AUTO_TEST_CASE(loadSerializedConvertToIplAndDisplay)
{
    BOOST_CHECK(this->tempImage.empty() == true);

    std::ifstream ifs("test_matrices_JpegToMat.bin", std::ios::in STREAM_FLAGS);

    try {
	SIARCHIVE
		ia >> this->tempImage;
    } catch (boost::archive::archive_exception e) {
	BOOST_FAIL(string("archive_exception:") << e.what());
    }

    ifs.close();

    BOOST_CHECK(this->tempImage.empty() == false);

    IplImage image = this->tempImage;

    cvNamedWindow("loadSerializedConvertToIplAndDisplay");
    cvShowImage("loadSerializedConvertToIplAndDisplay", &image);
    cvWaitKey(0);

    cvDestroyWindow("loadSerializedConvertToIplAndDisplay");
}

BOOST_AUTO_TEST_CASE(loadJpegSaveSerializedCompressed)
{

    IplImage *image = 0;
    image = cvLoadImage(KINECT_BRIDGE_TEST_IMAGE);

    BOOST_CHECK(image != 0);

    BOOST_CHECK(this->tempImage.empty() == true);

    cv::Mat tmp(image);
    tmp.copyTo(this->tempImage);

    BOOST_CHECK(this->tempImage.empty() == false);
    cvReleaseImage(&image);
    BOOST_CHECK(this->tempImage.empty() == false);

    std::ofstream ofs("test_matrices_JpegToMatCompressed.bin", std::ios::out STREAM_FLAGS);

    try {
	namespace io = boost::iostreams;
	io::filtering_ostream out;
	//out.push(io::zlib_compressor(io::zlib::best_speed));
	out.push(ofs);
	{
	    FOARCHIVE
		    oa << this->tempImage;
	}
    } catch (boost::archive::archive_exception e) {
	BOOST_FAIL(string("archive_exception:") << e.what());
    }

    ofs.close();

}

BOOST_AUTO_TEST_CASE(loadSerializedCompressedConvertToIplAndDisplay)
{
    BOOST_CHECK(this->tempImage.empty() == true);

    std::ifstream ifs("test_matrices_JpegToMatCompressed.bin", std::ios::in STREAM_FLAGS);

    try {
	namespace io = boost::iostreams;
	io::filtering_istream in;
	//in.push(io::zlib_decompressor());
	in.push(ifs);

	{
	    FIARCHIVE
		    ia >> this->tempImage;
	}
    } catch (boost::archive::archive_exception e) {
	BOOST_FAIL(string("archive_exception:") << e.what());
    }

    ifs.close();

    BOOST_CHECK(this->tempImage.empty() == false);

    IplImage image = this->tempImage;

    cvNamedWindow("loadSerializedConvertToIplAndDisplay");
    cvShowImage("loadSerializedConvertToIplAndDisplay", &image);
    cvWaitKey(0);

    cvDestroyWindow("loadSerializedConvertToIplAndDisplay");
}

BOOST_AUTO_TEST_SUITE_END()


