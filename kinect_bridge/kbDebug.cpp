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
	m_bytes[string(id)] = 0;
}

void TimeProfiler::setBytes(const char*id, long bytes)
{
	if (m_times.find(string(id)) == m_times.end()) {
		DBG_WARN("TimeProfiler: No data found for id " << id);
		return;
	}

	m_bytes[string(id)] += bytes;
	return;
}

TimeProfiler::t_status TimeProfiler::stop(const char* id, TimeProfiler::e_print print) {
	t_status status;
	status.bytes = -1;
	status.speed = -1;
	status.timeDiff = -1;

	if (m_times.find(string(id)) == m_times.end()) {
		DBG_WARN("TimeProfiler: No data found for id " << id);
		return status;
	}

	// calculate timeDiff
	t_ms msec = getMsec();
	t_ms diff = (msec - m_times[string(id)]);
	status.timeDiff = diff / 1000000.0;

	// calculate speed
	status.speed = m_bytes[string(id)] / status.timeDiff;

	// set bytes
	status.bytes = m_bytes[string(id)];

	char tmp[255];
	tmp[0] = 0;
	switch (print)
	{
	case print_all : {
		sprintf(tmp, "TP[%s] took: %.3f sec, %0.2f kB/sec"
			, id, status.timeDiff, status.speed/1000);
		DBG_INFO(tmp);
		break;
	}
	case print_time_only : {
		sprintf(tmp, "TP[%s] took: %.3f sec"
			, id, status.timeDiff);
		DBG_INFO(tmp);
		break;

	}
	}

	m_times.erase(string(id));
	m_bytes.erase(string(id));
	return status;
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

std::map<std::string, long> TimeProfiler::m_bytes = std::map<std::string, long>();
float TimeProfiler::m_speedAv = 0;
long TimeProfiler::m_speedCount = 0;
float TimeProfiler::m_pppAv = 0;
long TimeProfiler::m_pppCount = 0;

