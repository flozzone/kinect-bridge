#include "kinect_bridge/package_buffer.h"
#include "kinect_bridge/package.h"
#include "kinect_bridge/kbDebug.h"

DBG_IMPL_DEBUG_MODULE(KinectBridgeBuffer);

using namespace kb;

TestBuffer* TestBuffer::m_instance = 0;

PackageBuffer* PackageBuffer::m_instance = 0;

PackageBuffer::PackageBuffer()
{

}

PackageBuffer::~PackageBuffer() {
    delete m_instance;
}

PackageBuffer* PackageBuffer::getInstance() {
    if (PackageBuffer::m_instance == 0) {
	PackageBuffer::m_instance = new PackageBuffer();
    }
    return PackageBuffer::m_instance;
}

void PackageBuffer::push(const Package& package) {
    DBG_ENTER("");
    m_buffer.push_front(package);
}

Package& PackageBuffer::get() {
    DBG_ENTER("");
    return m_buffer.back();
}

void PackageBuffer::pop() {
    DBG_ENTER("");
    m_buffer.pop_back();
    return;
}


int PackageBuffer::getSize() {
    return m_buffer.size();
}

bool PackageBuffer::hasPackage() {
    if (this->getSize() > 0) {
	return true;
    }
    return false;
}

