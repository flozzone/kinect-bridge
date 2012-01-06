#ifndef _KINECT_BRIDGE_SERVER_H_
#define _KINECT_BRIDGE_SERVER_H_

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

#include "kinect_bridge/kbDebug.h"

DBG_IMPL_DEBUG_MODULE(KinectBridgeServer);

#include "kinect_bridge/connection.h" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>

#include "kinect_bridge/package.h"
#include "kinect_bridge/package_buffer.h"

#include <ntk/camera/openni_grabber.h>

using namespace ntk;


#define KINECT_BRIDGE_TEST_IMAGE "image.jpeg"
#define KINECT_ID 0
#define KINECT_HIGHRES false

namespace kb {

/// Serves stock quote information to any client that connects to it.
class server
{
public:
    /// Constructor opens the acceptor and starts waiting for the first incoming
    /// connection.
    server(boost::asio::io_service& io_service, unsigned short port);
    ~server();

    /// Handle completion of a accept operation.
    void handle_accept(const boost::system::error_code& e, connection_ptr conn);

    /// Handle completion of a write operation.
    void send_package(const boost::system::error_code& e, connection_ptr conn);

private:
    void initKinect();

    /// The acceptor object used to accept incoming socket connections.
    boost::asio::ip::tcp::acceptor acceptor_;

    /// The data to be sent to each client.
    //std::vector<Package*> m_buffer;
    Package* m_package;

    OpenniDriver m_ni_driver;
    OpenniGrabber m_grabber;

};

} // namespace kb


#endif // _KINECT_BRIDGE_SERVER_H_
