#include "kbDebug.h"

DBG_IMPL_DEBUG_MODULE(KinectBridgeDebug);

void kbDebug_init()
{
}


void kbDebug_loadConfig(const std::string &filename, bool forceAdditivityOff)
{

    // open logger configuration file
    PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(filename));

    std::cout << "Using log configuration at " << filename << std::endl;
    fflush(stdout);

    if (forceAdditivityOff) {
	// force the 'additivity' flag of all loggers to 'false'
	// (so that we don't need to explicitly write it in log.properties for each logger)
	LoggerList l_list = Logger::getCurrentLoggers();
	int l_count = l_list.size();

	for (int i = 0; i < l_count; i++)
	    l_list[i].setAdditivity(false);
    }
}

void TimeProfiler::setSpeed(size_t size, float sec) {
    m_speedCount++;
    m_speedAv += ((float)size / sec);
}

float TimeProfiler::getSpeed() {
    return (m_speedAv / m_speedCount);
}

void TimeProfiler::setPPP(float sec) {
    m_pppCount++;
    m_pppAv += ((float)1/sec);
}

float TimeProfiler::getPPP() {
    return (m_pppAv / m_pppCount);
}

void TimeProfiler::start(const char* id) {
    long usec = helpers::Time::gettimeofday().usec();
    usec += (helpers::Time::gettimeofday().sec() * pow((double)10, 6));
    times[string(id)] = usec;
}

float TimeProfiler::stop(const char* id) {
    if (times[string(id)] != 0) {
	long usec = helpers::Time::gettimeofday().usec();
	usec += (helpers::Time::gettimeofday().sec() * pow((double)10, 6));
	long diff = (usec - times[string(id)]);
	float sec = diff / pow((double)10, 6);

	times.erase(string(id));

	return sec;
    }
    return -1;
}

std::map<std::string, long> TimeProfiler::times = std::map<std::string, long>();
float TimeProfiler::m_speedAv = 0;
long TimeProfiler::m_speedCount = 0;
float TimeProfiler::m_pppAv = 0;
long TimeProfiler::m_pppCount = 0;

