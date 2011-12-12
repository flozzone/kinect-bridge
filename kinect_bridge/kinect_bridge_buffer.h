#ifndef _KINECT_BRIDGE_KINECT_BRIDGE_BUFFER_H_
#define _KINECT_BRIDGE_KINECT_BRIDGE_BUFFER_H_

#include "kinect_bridge/kinect_bridge.h"

namespace kb {

class PackageBuffer {
private:
    typedef std::vector<kb::Package> t_packages;

    t_packages m_packages;

public:
    int getSize();
    kb::Package& get();
    void put(const kb::Package& package);
    void pop();
    bool hasPackage();
};
}

#endif // _KINECT_BRIDGE_KINECT_BRIDGE_BUFFER_H_
