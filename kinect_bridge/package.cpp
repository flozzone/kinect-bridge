#include "kinect_bridge/kbDebug.h"
#include "kinect_bridge/package.h"
#include "kinect_bridge/cvmat_serialization.h"

DBG_IMPL_DEBUG_MODULE(KinectBridge);

namespace kb {

PackageHeader::PackageHeader(const PackageHeader& header)
{
    DBG_ENTER("CopyConstructor version:" << header.m_version);
    m_version = header.m_version;
}

PackageHeader & PackageHeader::operator=(const PackageHeader& header) {
    m_version = header.m_version;
    return *this;
}

Package::Package(const Package &package)
{
    DBG_ENTER("CopyConstructor version:" << package.m_version);
    m_header = package.m_header;

    m_version = package.m_version;
    package.m_color.copyTo(m_color);
    package.m_depth.copyTo(m_depth);
}

Package& Package::operator=(const Package& package) {
    if (this != &package) {
	m_header = package.m_header;

	m_version = package.m_version;
	package.m_color.copyTo(m_color);
	package.m_depth.copyTo(m_depth);
    }
    return *this;
}
} // namespace kb
