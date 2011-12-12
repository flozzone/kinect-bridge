//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <vector>
#include "kinect_bridge/kinect_bridge_connection.hpp" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>

#include "kinect_bridge/cvmat_serialization.h"
#include "kinect_bridge.h"
#include "kinect_bridge/kbDebug.h"


#define KINECT_BRIDGE_TEST_IMAGE "image.jpeg"

DBG_IMPL_DEBUG_MODULE(KinectBridgeServer);

namespace s11n_example {

/// Serves stock quote information to any client that connects to it.
class server
{
public:
    /// Constructor opens the acceptor and starts waiting for the first incoming
    /// connection.
    server(boost::asio::io_service& io_service, unsigned short port)
	: acceptor_(io_service,
	      boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {
	// Create the data to be sent to each client.
	IplImage *image = 0;
	image = cvLoadImage(KINECT_BRIDGE_TEST_IMAGE);

	assert((image != 0) && "No image 'image.jpeg' present in this directory");

	cv::Mat color(image);

	cv::Mat depth;
	depth.create(color.size(), CV_8UC1);

	std::cout << "channels:" << color.channels() << std::endl;
	DBG_DEBUG("test");

	std::vector<cv::Mat> color_split;
	cv::split(color, color_split);
	depth = *color_split.begin();


	color.copyTo(this->package_.m_color);
	depth.copyTo(this->package_.m_depth);

	std::cout << "channels:" << depth.channels() << std::endl;

	//color.deallocate();
	//depth.deallocate();

	kb::PackageHeader header;
	header.m_version = 3;

	this->package_.m_header = kb::PackageHeader(header);

	// Start an accept operation for a new connection.
	connection_ptr new_conn(new connection(acceptor_.get_io_service()));
	acceptor_.async_accept(new_conn->socket(),
			       boost::bind(&server::handle_accept, this,
					   boost::asio::placeholders::error, new_conn));
    }

    /// Handle completion of a accept operation.
    void handle_accept(const boost::system::error_code& e, connection_ptr conn)
    {
	if (!e)
	{
	    // Successfully accepted a new connection. Send the list of stocks to the
	    // client. The connection::async_write() function will automatically
	    // serialize the data structure for us.
	    conn->async_write(this->package_,
			      boost::bind(&server::handle_write, this,
					  boost::asio::placeholders::error, conn));

	    /*
      // Start an accept operation for a new connection.
      connection_ptr new_conn(new connection(acceptor_.io_service()));
      acceptor_.async_accept(new_conn->socket(),
   boost::bind(&server::handle_accept, this,
     boost::asio::placeholders::error, new_conn));
      */
	}
	else
	{
	    // An error occurred. Log it and return. Since we are not starting a new
	    // accept operation the io_service will run out of work to do and the
	    // server will exit.
	    std::cerr << e.message() << std::endl;
	}
    }

    /// Handle completion of a write operation.
    void handle_write(const boost::system::error_code& e, connection_ptr conn)
    {
	// Nothing to do. The socket will be closed automatically when the last
	// reference to the connection object goes away.
    }

private:
    /// The acceptor object used to accept incoming socket connections.
    boost::asio::ip::tcp::acceptor acceptor_;

    /// The data to be sent to each client.
    kb::Package package_;
};

} // namespace s11n_example

int main(int argc, char* argv[])
{


    try
    {
	// Check command line arguments.
	if (argc != 3)
	{
	    std::cerr << "Usage: server <port> <log.properties>" << std::endl;
	    return 1;
	}
	unsigned short port = boost::lexical_cast<unsigned short>(argv[1]);

	kbDebug_loadConfig(std::string(argv[2]));

	boost::asio::io_service io_service;
	s11n_example::server server(io_service, port);
	io_service.run();
    }
    catch (std::exception& e)
    {
	std::cerr << e.what() << std::endl;
    }

    return 0;
}
