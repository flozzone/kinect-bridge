#include "kbDebug.h"
#include <QDebug>

template <class T>
std::ostream& qtDbg(std::ostream &str, const T &t)
{
	QString buf;
	QDebug(&buf) << t;
	return str << buf.toStdString();
}

std::ostream& operator <<(std::ostream &str, const QVariant &v)
{
	return qtDbg(str, v);
}

std::ostream& operator <<(std::ostream &str, const QString &v)
{
	return (str << v.toStdString());
}

std::ostream& operator <<(std::ostream &str, const QByteArray &v)
{
	return qtDbg(str, v);
}

std::ostream& operator <<(std::ostream &str, const QList<QVariantHash> &v)
{
	return qtDbg(str, v);
}

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

