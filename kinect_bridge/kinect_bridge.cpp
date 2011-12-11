#include "kinect_bridge.h"
#include "kinect_bridge/cvmat_serialization.h"

using namespace kb;

PackageHeader::PackageHeader(const PackageHeader& header)
    : m_version(header.m_version)
{
}

PackageHeader & PackageHeader::operator=(const PackageHeader& header) {
    m_version = header.m_version;
    return *this;
}

Package::Package(const Package &package)
    : m_header(package.m_header),
      m_color(package.m_color),
      m_depth(package.m_depth)
{
}

Package::Package(const PackageHeader &header, const cv::Mat &color, const cv::Mat &depth)
    : m_header(header),
      m_color(color),
      m_depth(depth)
{
}

Package& Package::operator=(const Package& package) {
    if (this != &package) {
	m_header = package.m_header;
	m_color = package.m_color;
	m_depth = package.m_depth;
    }
    return *this;
}
/*
template<class Archive>
void Package::serialize(Archive & ar, const unsigned int version)
{
    ar & m_header;
    ar & m_color;
    ar & m_depth;
}*/
