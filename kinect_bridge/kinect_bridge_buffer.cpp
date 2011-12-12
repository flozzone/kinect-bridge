#include "kinect_bridge/kinect_bridge_buffer.h"
#include "kinect_bridge/kinect_bridge.h"
#include "kinect_bridge/kbDebug.h"

DBG_IMPL_DEBUG_MODULE(KinectBridgeBuffer);

namespace kb {
int PackageBuffer::getSize() {
    return m_packages.size();
}

Package& PackageBuffer::get() {
    DBG_TRACE("Buffer has " << m_packages.size() << " packages");
    return m_packages.back();
}

void PackageBuffer::put(const kb::Package& package) {
    DBG_ENTER("");
    m_packages.insert(m_packages.end(), package);
}

void PackageBuffer::pop() {
    m_packages.pop_back();
    return;
}

bool PackageBuffer::hasPackage() {
    if (this->getSize() > 0) {
	return true;
    }
    return false;
}
} // namespace kb
