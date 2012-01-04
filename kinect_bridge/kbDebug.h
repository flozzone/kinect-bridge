#ifndef __KINECT_BRIDGE_KBDEBUG_H
#define __KINECT_BRIDGE_KBDEBUG_H

#include <math.h>


//////////////////////////////////////////////////////////////////////
// debug functions provided by log4cplus debugging library			//
//////////////////////////////////////////////////////////////////////
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/logger.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/hierarchylocker.h>
#include <log4cplus/ndc.h>
#include <log4cplus/helpers/loglog.h>
#include <iomanip>

using namespace log4cplus;

// create static instance of debug module
#define DBG_IMPL_DEBUG_MODULE( module ) static Logger debugModule = Logger::getInstance(#module)


// get access to the instance of debug module with given name. If such object didn't exist before,
// it will be created
#define DBG_GET_DEBUG_MODULE( module ) Logger::getInstance(#module)

// various types of print debug messages ( with specified name of the debug module )
#define DBG_TRACE_MODULE( module, fmt ) LOG4CPLUS_TRACE( DBG_GET_DEBUG_MODULE( module ), fmt )
#define DBG_DEBUG_MODULE( module, fmt ) LOG4CPLUS_DEBUG( DBG_GET_DEBUG_MODULE( module ), fmt )
#define  DBG_INFO_MODULE( module, fmt )  LOG4CPLUS_INFO( DBG_GET_DEBUG_MODULE( module ), fmt )
#define  DBG_WARN_MODULE( module, fmt )  LOG4CPLUS_WARN( DBG_GET_DEBUG_MODULE( module ), fmt )
#define DBG_ERROR_MODULE( module, fmt ) LOG4CPLUS_ERROR( DBG_GET_DEBUG_MODULE( module ), fmt )
#define DBG_FATAL_MODULE( module, fmt ) LOG4CPLUS_FATAL( DBG_GET_DEBUG_MODULE( module ), fmt )

// various types of print debug messages ( using local debugModule variable )
#define DBG_TRACE( fmt ) LOG4CPLUS_TRACE( debugModule, fmt )
#define DBG_DEBUG( fmt ) LOG4CPLUS_DEBUG( debugModule, fmt )
#define  DBG_INFO( fmt )  LOG4CPLUS_INFO( debugModule, fmt )
#define  DBG_WARN( fmt )  LOG4CPLUS_WARN( debugModule, fmt )
#define DBG_ERROR( fmt ) LOG4CPLUS_ERROR( debugModule, fmt )
#define DBG_FATAL( fmt ) LOG4CPLUS_FATAL( debugModule, fmt )
#define DBG_ENTER( fmt ) LOG4CPLUS_TRACE( debugModule, "Enter " << __FUNCTION__  << ": " << fmt)
#define DBG_DENTER( fmt ) LOG4CPLUS_DEBUG( debugModule, "Enter " << __FUNCTION__  << ": " << fmt)
#define DBG_LEAVE( fmt ) LOG4CPLUS_TRACE( debugModule, "Leave " << __FUNCTION__  << ": " << fmt)
#define DBG_DLEAVE( fmt ) LOG4CPLUS_DEBUG( debugModule, "Leave " << __FUNCTION__  << ": " << fmt)

// set debugging level for given module
#define DBG_SET_DEBUG_LEVEL_MODULE( module, level ) DBG_GET_DEBUG_MODULE( module ).setLogLevel( level )

// set debugging level for static  module
#define DBG_SET_DEBUG_LEVEL( level ) debugModule.setLogLevel( level )

void kbDebug_init();
void kbDebug_loadConfig(const std::string &filename, bool forceAdditivityOff = true);

class TimeProfiler {
    static std::map<std::string, long> times;

    static float m_speedAv;
    static long m_speedCount;
    static float m_pppAv;
    static long m_pppCount;
public:
    static void setSpeed(size_t size, float sec) {
	m_speedCount++;
	m_speedAv += ((float)size / sec);
    }

    static float getSpeed() {
	return (m_speedAv / m_speedCount);
    }

    static void setPPP(float sec) {
	m_pppCount++;
	m_pppAv += ((float)1/sec);
    }

    static float getPPP() {
	return (m_pppAv / m_pppCount);
    }

    static void start(const char* id) {
	struct timespec current;
	assert(clock_gettime(CLOCK_MONOTONIC, &current) == 0);
	times[std::string(id)] = (current.tv_sec * pow(10, 9)) + current.tv_nsec;
    }

    static float stop(const char* id) {
	if (times[std::string(id)] != 0) {
	    struct timespec current;
	    //char tmp[255];

	    assert(clock_gettime(CLOCK_MONOTONIC, &current) == 0);
	    long diff = ((current.tv_sec* pow(10, 9)) + current.tv_nsec) - times[std::string(id)];
	    float sec = diff / pow(10, 9);


	    //sprintf(tmp, "TIME: %s took: %.5f sec", id, sec);

	    times.erase(std::string(id));

	    return sec;
	}
	return -1;
    }
};


#endif // __KINECT_BRIDGE_KBDEBUG_H
