//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING 0

#include "client.h"

#include <iostream>
//#include <vector>
#include <time.h>
#include <math.h>

#include "kinect_bridge/connection.h" // Must come before boost/serialization headers.
//#include <boost/serialization/vector.hpp>

#include "kinect_bridge/package.h"
#include "kinect_bridge/package_buffer.h"

#include <opencv2/opencv.hpp>

#include "kinect_bridge/kbDebug.h"

using namespace kb;

client::client(boost::asio::io_service& io_service,
	       const std::string& host, const std::string& service, kb::client::t_packageHandler ph)
    : connection_(io_service)
{
	DBG_ENTER("");
    //m_packageReadySig.connect(ph);

	DBG_TRACE("create namedWindows");
    cv::namedWindow("color");
    cv::namedWindow("depth");
    //cvNamedWindow("color");
    //cvNamedWindow("depth");

    // Resolve the host name into an IP address.
	DBG_TRACE("Resolve host name");
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(host, service);
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator =
	    resolver.resolve(query);
    boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

    // Start an asynchronous connect operation.
    connection_.socket().async_connect(endpoint,
				       boost::bind(&client::handle_connect, this,
						   boost::asio::placeholders::error, ++endpoint_iterator));
}

client::~client() {
    cv::destroyAllWindows();
}

void client::handle_connect(const boost::system::error_code& e,
			    boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{
    if (!e)
    {
	//assert(this->m_buffer.size() == 0);
	//DBG_ERROR("bufferPointer (handle_connect)=" << &this->m_buffer);
	// Successfully established connection. Start operation to read the list
	// of stocks. The connection::async_read() function will automatically
	// decode the data that is read from the underlying socket.
	TimeProfiler::start("read Package");
	connection_.async_read(this->m_package,
			       boost::bind(&client::handle_read, this,
					   boost::asio::placeholders::error));
    }
    else if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator())
    {
	// Try the next endpoint.
	connection_.socket().close();
	boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
	connection_.socket().async_connect(endpoint,
					   boost::bind(&client::handle_connect, this,
						       boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
    {
	// An error occurred. Log it and return. Since we are not starting a new
	// operation the io_service will run out of work to do and the client will
	// exit.
	std::cerr << "handle_connect: message:" << e.message() << std::endl;
    }
}

/// Handle completion of a read operation.
void client::handle_read(const boost::system::error_code& e)
{
	TimeProfiler::t_status status = TimeProfiler::stop("read Package");

    //DBG_ENTER("Finished reading package");
    if (e.value() == 0)
    {
	//DBG_TRACE("client: buffer size: "  <<  m_buffer.size());


	//kb::Package* package = m_buffer.back();
	kb::Package* package = m_package;
	//m_buffer.pop_back();

	char tmp[255];
	sprintf(tmp, "Read package (%i) took: %.3f sec speed: %0.2f pkg/sec, %0.2f kB/sec", package->m_header.m_version, status.timeDiff, 1/status.timeDiff, status.speed/1000);
	DBG_INFO(tmp);

	assert(package->m_color.empty() == false);

	//cv::imshow("color", package->m_color);
	//cv::imshow("depth", package->m_depth);
	//cv::waitKey(0);

	assert(package->m_color.empty() == false);

	// call the package handler
	//m_packageReadySig(package);

	TimeProfiler::start("read Package");
	connection_.async_read(this->m_package,
			       boost::bind(&client::handle_read, this,
					   boost::asio::placeholders::error));
    }
    else
    {
	if (e.value() == 0) {
	    DBG_INFO("Successfull read stream");
	} else {
	    // An error occurred.
	    DBG_FATAL(e.message() << " code:" << e.value());
	}
    }

    // Since we are not starting a new operation the io_service will run out of
    // work to do and the client will exit.
}

PackageGrabber::PackageGrabber(const char* host, const char* port)
    : m_host(host)
    , m_port(port)
{
}

PackageGrabber::~PackageGrabber()
{
    //delete m_client;
}

void PackageGrabber::setPackageHandler(const kb::client::t_packageHandler handler)
{
    m_packageHandler = handler;
}

void PackageGrabber::operator()() {
    DBG_ENTER("");

    try
    {
	boost::asio::io_service io_service;
	DBG_TRACE("create client");
	m_client = new kb::client(io_service, m_host.c_str(), m_port.c_str(), m_packageHandler);
	DBG_TRACE("end create client");

	io_service.run();
    } catch (std::exception& e)
    {
	std::cerr << "Exception:" << e.what() << std::endl;
    }
}

boost::thread* PackageGrabber::start() {
    m_thread = new boost::thread(*this);
    return m_thread;
}

void myPackage(const Package* package) {
    DBG_ERROR("myPackage: got package " << package->m_header.m_version);
    delete(package);
}

int main(int argc, char* argv[])
{
	std::cout << "starting client" << std::endl;

    // Check command line arguments.
    if (argc != 4)
    {
	std::cerr << "Usage: client <host> <port> <log.properties>" << std::endl;
	return 1;
    }

    std::cout << "load logging" << std::endl;
    kbDebug_init();
    kbDebug_loadConfig(argv[3]);
    std::cout << "finished loading logging" << std::endl;
    DBG_INFO("Starting grabber");
    std::cout << "testlog" << std::endl;


    kb::PackageGrabber pgrabber(argv[1], argv[2]);
    //pgrabber.setPackageHandler(&myPackage);
    pgrabber.start()->join();

    return 0;
}
