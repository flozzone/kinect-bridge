#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test_Package
#include <boost/test/unit_test.hpp>

#include "kinect_bridge/kinect_bridge.h"
#include "kinect_bridge/kinect_bridge_buffer.h"
#include "kinect_bridge/kbDebug.h"

#define KINECT_BRIDGE_TEST_IMAGE "image.jpeg"

#define LOG_PROPERTIES_PATH "../../kinect_bridge/log.properties"

DBG_IMPL_DEBUG_MODULE(TestBuffer);

struct Fixture {
    Fixture() {
	kbDebug_init();
	kbDebug_loadConfig(std::string(LOG_PROPERTIES_PATH));
    }
    ~Fixture() {

    }
};

BOOST_FIXTURE_TEST_SUITE(test_Buffer, Fixture);

BOOST_AUTO_TEST_CASE( storePackageGetCheckVersion )
{
    DBG_ENTER("storePackageGetCheckVersion");

    kb::PackageBuffer buffer;

    BOOST_CHECK(buffer.getSize() == 0);

    {
	DBG_DEBUG("Create temporary package");

	kb::Package package;

	BOOST_CHECK(package.m_version == 0);
	BOOST_CHECK(package.m_header.m_version == 0);

	package.m_version = 1;
	package.m_header.m_version = 2;

	BOOST_CHECK(package.m_version == 1);
	BOOST_CHECK(package.m_header.m_version == 2);

	DBG_DEBUG("Store temporary package in buffer");

	buffer.put(package);
    }

    DBG_DEBUG("Retrieve previous stored package from buffer");

    BOOST_CHECK(buffer.getSize() == 1);

    BOOST_CHECK(buffer.get().m_version == 1);
    BOOST_CHECK(buffer.get().m_header.m_version == 2);
}

BOOST_AUTO_TEST_SUITE_END()
