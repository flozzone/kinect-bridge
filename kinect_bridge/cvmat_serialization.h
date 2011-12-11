#ifndef _KINECT_BRIDGE_CVMAT_SERIALIZATION_H_
#define _KINECT_BRIDGE_CVMAT_SERIALIZATION_H_

#include <opencv2/opencv.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/archive_exception.hpp>
#include <fstream>

BOOST_SERIALIZATION_SPLIT_FREE(::cv::Mat);
namespace boost {
namespace serialization {
/** Serialization support for cv::Mat */
template<class Archive>
void save(Archive & ar, const ::cv::Mat& m, const unsigned int version)
{
    std::size_t elem_size = m.elemSize();
    std::size_t elem_type = m.type();

    ar & m.cols;
    ar & m.rows;
    ar & elem_size;
    ar & elem_type;

    const size_t data_size = m.cols * m.rows * elem_size;
    ar & boost::serialization::make_array(m.ptr(), data_size);
}

/** Serialization support for cv::Mat */
template<class Archive>
void load(Archive & ar, ::cv::Mat& m, const unsigned int version)
{
    int cols, rows;
    size_t elem_size, elem_type;

    ar & cols;
    ar & rows;
    ar & elem_size;
    ar & elem_type;

    m.create(rows, cols, elem_type);

    size_t data_size = m.cols * m.rows * elem_size;
    ar & boost::serialization::make_array(m.ptr(), data_size);
}
// Try read next object from archive
template<class Archive>
bool try_stream_next(Archive &ar, const std::ifstream &s, cv::Mat &o)
{
    bool success = false;

    try {
	ar >> o;
	success = true;
    } catch (const boost::archive::archive_exception &e) {
	if (e.code != boost::archive::archive_exception::output_stream_error) {
	    throw;
	}
    }

    return success;
}
}
}

#endif //_KINECT_BRIDGE_CVMAT_SERIALIZATION_H_



