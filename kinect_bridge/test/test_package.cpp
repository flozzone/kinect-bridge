#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test_Package
#include <boost/test/unit_test.hpp>

#include "kinect_bridge/package.h"

#define KINECT_BRIDGE_TEST_IMAGE "image.jpeg"

BOOST_AUTO_TEST_SUITE( test_Package )

BOOST_AUTO_TEST_CASE( copyPackageCheckVersion )
{
    kb::Package package1;
    {
	kb::Package package2;
	BOOST_CHECK(package2.m_header.m_version == 0);

	package2.m_header.m_version = 1;

	BOOST_CHECK(package2.m_header.m_version == 1);

	package1 = package2;
    }

    BOOST_CHECK(package1.m_header.m_version == 1);
}

BOOST_AUTO_TEST_CASE( loadImageInPackageAndCopy )
{
    kb::Package package1;

    {
	IplImage *image = 0;
	image = cvLoadImage(KINECT_BRIDGE_TEST_IMAGE);

	BOOST_CHECK(image != 0);

	kb::PackageHeader header;
	header.m_version = 2;

	kb::Package package;

	BOOST_CHECK(package.m_header.m_version == 0);
	BOOST_CHECK(package.m_color.empty() == true);
	BOOST_CHECK(package.m_depth.empty() == true);

	package.m_header = header;
	package.m_color = cv::Mat(image);
	package.m_depth = cv::Mat(image);

	BOOST_CHECK(package.m_header.m_version == 2);
	BOOST_CHECK(package.m_color.empty() == false);
	BOOST_CHECK(package.m_depth.empty() == false);

	package1 = package;

	cvReleaseImage(&image);
    }

    BOOST_CHECK(package1.m_header.m_version == 2);
    BOOST_CHECK(package1.m_color.empty() == false);
    BOOST_CHECK(package1.m_depth.empty() == false);
}

BOOST_AUTO_TEST_SUITE_END()
