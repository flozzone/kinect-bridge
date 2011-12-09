#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <fstream>
#include <boost/test/unit_test.hpp>

#include "cvmat_serialization.h"

#define BOOST_TEST_MODULE CvMatSerialization

BOOST_AUTO_TEST_CASE(record)
{

  cv::Mat depths, colors;

  std::ofstream ofs("matrices.bin", std::ios::out | std::ios::binary);

  { // use scope to ensure archive goes out of scope before stream

    boost::archive::binary_oarchive oa(ofs);

    {
      oa << depths << colors;

    }
  }

  ofs.close();
}

