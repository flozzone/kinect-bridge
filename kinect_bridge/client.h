#ifndef _KINECT_BRIDGE_CLIENT_H_
#define _KINECT_BRIDGE_CLIENT_H_

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
#include <boost/thread.hpp>
#include <boost/signal.hpp>

#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>

#include "kinect_bridge/kbDebug.h"

DBG_IMPL_DEBUG_MODULE(KinectBridgeClient);

#include "kinect_bridge/connection.h" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>

#include "kinect_bridge/package.h"
#include "kinect_bridge/package_buffer.h"

#include <opencv2/opencv.hpp>


namespace kb {
/// Downloads stock quote information from a server.
class client
{
public:
    typedef void (*t_packageHandler)(const Package* package);

    /// Constructor starts the asynchronous connect operation.
    client(boost::asio::io_service& io_service,
	   const std::string& host, const std::string& service, t_packageHandler ph = 0);
    ~client();

    /// Handle completion of a connect operation.
    void handle_connect(const boost::system::error_code& e,
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

    /// Handle completion of a read operation.
    void handle_read(const boost::system::error_code& e);

private:
    /// The connection to the server.
    mutable connection connection_;

    /// The data received from the server.
    std::vector<Package*> m_buffer;

    boost::signal<void (const Package*)> m_packageReadySig;
};

class PackageGrabber {
public:
    PackageGrabber(const char* host, const char* port, const char* log_properties);
    ~PackageGrabber();
    void setPackageHandler(const kb::client::t_packageHandler);
    void operator()();

    boost::thread* start();

 private:
    kb::client* m_client;
    std::string m_host;
    std::string m_port;
    std::string m_log_properties;
    boost::thread* m_thread;
    kb::client::t_packageHandler m_packageHandler;
};

} // namespace kb


#endif // _KINECT_BRIDGE_CLIENT_H_
