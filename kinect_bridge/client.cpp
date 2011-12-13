//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <vector>

#include "kinect_bridge/kbDebug.h"

DBG_IMPL_DEBUG_MODULE(KinectBridgeClient);

#include "kinect_bridge/connection.h" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>

#include "kinect_bridge/package.h"
#include "kinect_bridge/package_buffer.h"

namespace kb {

/// Downloads stock quote information from a server.
class client
{
public:
    /// Constructor starts the asynchronous connect operation.
    client(boost::asio::io_service& io_service,
	   const std::string& host, const std::string& service)
	: connection_(io_service),
	  m_buffer(kb::getBufSize())
    {
	// Resolve the host name into an IP address.
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

    /// Handle completion of a connect operation.
    void handle_connect(const boost::system::error_code& e,
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
    {
	if (!e)
	{
	    // Successfully established connection. Start operation to read the list
	    // of stocks. The connection::async_read() function will automatically
	    // decode the data that is read from the underlying socket.
	    connection_.async_read(&this->m_buffer,
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
    void handle_read(const boost::system::error_code& e)
    {
	DBG_ENTER("Finished reading package");
	if (e.value() == 0)
	{
	    DBG_INFO("Successfull read package");

	    if (this->m_buffer.is_not_empty() == false) {
		DBG_WARN("Buffer is empty");
	    } else {
		kb::Package package;

		assert(package.m_color.empty() == true);

		DBG_DEBUG("Getting package from buffer");
		this->m_buffer.pop_back(&package);
		DBG_DEBUG("After getting package from buffer");

		assert(package.m_color.empty() == false);


		IplImage image = package.m_color;

		cvNamedWindow("loadSerializedConvertToIplAndDisplay");
		cvShowImage("loadSerializedConvertToIplAndDisplay", &image);
		cvWaitKey(0);

		cvDestroyWindow("loadSerializedConvertToIplAndDisplay");

		assert(package.m_color.empty() == false);

		DBG_TRACE("Got package with pversion: " << package.m_version << " and hversion: " << package.m_header.m_version);
	    }

	    connection_.async_read(&this->m_buffer,
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

private:
    /// The connection to the server.
    connection connection_;

    /// The data received from the server.
    mutable kb::bounded_buffer<Package> m_buffer;
};

} // namespace kb

int main(int argc, char* argv[])
{
    try
    {
	// Check command line arguments.
	if (argc != 4)
	{
	    std::cerr << "Usage: client <host> <port> <log.properties>" << std::endl;
	    return 1;
	}

	kbDebug_init();
	kbDebug_loadConfig(std::string(argv[3]));

	DBG_ENTER("");

	boost::asio::io_service io_service;
	kb::client client(io_service, argv[1], argv[2]);
	io_service.run();
    }
    catch (std::exception& e)
    {
	std::cerr << "Exception:" << e.what() << std::endl;
    }

    return 0;
}