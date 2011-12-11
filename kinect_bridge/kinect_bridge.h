#ifndef _KINECT_BRIDGE_KINECT_BRIDGE_H_
#define _KINECT_BRIDGE_KINECT_BRIDGE_H_

#include "kinect_bridge/cvmat_serialization.h"

namespace kb {

class PackageHeader
{
private:
    friend class boost::serialization::access;

    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
	ar & m_version;
    }
public:
    PackageHeader() : m_version(0) {};
    PackageHeader(const PackageHeader&);
    PackageHeader & operator=(const PackageHeader&);

    int m_version;
};

class Package
{
    friend class boost::serialization::access;

    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
	ar & m_header;
	ar & m_color;
	ar & m_depth;
    }
public:
    Package() {};
    Package(const Package&);
    Package(const PackageHeader&, const cv::Mat&, const cv::Mat&);
    Package& operator=(const Package&);

    PackageHeader m_header;
    cv::Mat m_color;
    cv::Mat m_depth;
};

} // namespace kb

#endif // _KINECT_BRIDGE_KINECT_BRIDGE_H_
