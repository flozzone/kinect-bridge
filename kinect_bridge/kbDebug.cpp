#include "kbDebug.h"

#ifdef _MSC_VER // Identifies MS compilers
# include "Windows.h"
#endif
#include <ctime>

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
	m_times[string(id)] = getMsec();
}

float TimeProfiler::stop(const char* id) {
	if (m_times[string(id)] != 0) {
		t_ms msec = getMsec();
	t_ms diff = (msec - m_times[string(id)]);
	float sec = diff / 1000000.0;

	m_times.erase(string(id));

	return sec;
    }
    return -1;
}

#ifdef _MSC_VER // Identifies MS compilers
ULONGLONG TimeProfiler::getMsec()
{
	SYSTEMTIME systemTime;
	GetSystemTime(&systemTime);

	FILETIME fileTime;
	SystemTimeToFileTime(&systemTime, &fileTime);

	ULARGE_INTEGER uli;
	uli.LowPart = fileTime.dwLowDateTime;
	uli.HighPart = fileTime.dwHighDateTime;

	return uli.QuadPart/10;
}
std::map<std::string, ULONGLONG> TimeProfiler::m_times = std::map<std::string, ULONGLONG>();
#else
long TimeProfiler::getMsec()
{
	long usec = helpers::Time::gettimeofday().usec();
    usec += (helpers::Time::gettimeofday().sec() * 1000000);
	DBG_TRACE("t.sec=" << helpers::Time::gettimeofday().sec() << " t.usecusec=" << helpers::Time::gettimeofday().sec() << " usec=" << usec);
    return usec;
}
std::map<std::string, long> TimeProfiler::m_times = std::map<std::string, long>();
#endif // _MSC_VER

float TimeProfiler::m_speedAv = 0;
long TimeProfiler::m_speedCount = 0;
float TimeProfiler::m_pppAv = 0;
long TimeProfiler::m_pppCount = 0;

