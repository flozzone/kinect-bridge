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

#include <ntk/ntk.h>


namespace kb {
/// Downloads stock quote information from a server.
class client
{
public:
    typedef void (*packageHandler)(const Package* package);


    /// Constructor starts the asynchronous connect operation.
    client(boost::asio::io_service& io_service,
	   const std::string& host, const std::string& service, packageHandler ph = 0);
    ~client();

    /// Handle completion of a connect operation.
    void handle_connect(const boost::system::error_code& e,
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

    /// Handle completion of a read operation.
    void handle_read(const boost::system::error_code& e);

private:
    /// The connection to the server.
    mutable connection connection_;

    struct timespec m_time;
    long m_nsec;
    packageHandler m_packageHandler;

    /// The data received from the server.
    std::vector<Package*> m_buffer;
};

class PackageGrabber {
public:
    PackageGrabber(const char* host, const char* port, const char* log_properties);
    void setPackageHandler(kb::client::packageHandler ph);
    void operator()();

    boost::thread* start();
private:
    std::string m_host;
    std::string m_port;
    std::string m_log_properties;
    boost::thread* m_thread;
    kb::client::packageHandler m_packageHandler;
};

} // namespace kb


#endif // _KINECT_BRIDGE_CLIENT_H_
