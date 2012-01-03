//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "server.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <vector>

#include "kinect_bridge/kbDebug.h"

#include "kinect_bridge/connection.h" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>

#include "kinect_bridge/package.h"
#include "kinect_bridge/package_buffer.h"

#include <ntk/ntk.h>
#include <ntk/utils/debug.h>
#include <ntk/camera/openni_grabber.h>
#include <ntk/gesture/body_event.h>

using namespace ntk;
using namespace kb;

server::server(boost::asio::io_service& io_service, unsigned short port)
    : acceptor_(io_service,
	  boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    , m_grabber(m_ni_driver, KINECT_ID)
{

    initKinect();

    Package* package = new Package();
    m_buffer.push_back(package);

    // Start an accept operation for a new connection.
    connection_ptr new_conn(new connection(acceptor_.get_io_service()));
    acceptor_.async_accept(new_conn->socket(),
			   boost::bind(&server::handle_accept, this,
				       boost::asio::placeholders::error, new_conn));
}

server::~server()
{
    m_grabber.stop();
}

/// Handle completion of a accept operation.
void server::handle_accept(const boost::system::error_code& e, connection_ptr conn)
{
    if (!e)
    {
	//DBG_ENTER("Start writing first package");

	// Successfully accepted a new connection. Send the list of stocks to the
	// client. The connection::async_write() function will automatically
	// serialize the data structure for us.
	//conn->async_write(this->m_buffer,
	//		  boost::bind(&server::handle_write, this,
	//			      boost::asio::placeholders::error, conn));
	send_package(boost::system::error_code(), conn);

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
void server::send_package(const boost::system::error_code& e, connection_ptr conn)
{
    // Nothing to do. The socket will be closed automatically when the last
    // reference to the connection object goes away.

    if (e.value() != 0) {
	DBG_INFO(e.message());
    } else {
	//DBG_DEBUG("Start writing next package: " << e.message() << " value:" << e.value());

	// Holder for the current image.
	RGBDImage image;

	DBG_TRACE("start waitForNextFrame");
	m_grabber.waitForNextFrame();
	DBG_TRACE("after waitForNextFrame");
	m_grabber.copyImageTo(image);

	Package* package = new Package();

	// Prepare the color view with skeleton and handpoint.
	cv::Mat3b debug_color_img;
	image.rgb().copyTo(debug_color_img);
	package->m_color = debug_color_img;

	// Prepare the depth view, mapped onto rgb frame.
	package->m_depth = normalize_toMat1b(image.mappedDepth());

	m_buffer.push_back(package);

	conn->async_write(this->m_buffer,
			  boost::bind(&server::send_package, this,
				      boost::asio::placeholders::error, conn));
    }
}

void server::initKinect()
{
    DBG_ENTER("");
    // Set debug level to 1.
    ntk::ntk_debug_level = 1;

    // High resolution 1280x1024 RGB Image.
    m_grabber.setHighRgbResolution(KINECT_HIGHRES);

    // Start the grabber.
    m_grabber.connectToDevice();
    m_grabber.start();
}

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

	kbDebug_init();
	kbDebug_loadConfig(std::string(argv[2]));

	boost::asio::io_service io_service;
	kb::server server(io_service, port);
	io_service.run();
    }
    catch (std::exception& e)
    {
	std::cerr << e.what() << std::endl;
    }

    return 0;
}
