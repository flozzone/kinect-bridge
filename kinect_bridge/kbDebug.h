#ifndef __KINECT_BRIDGE_KBDEBUG_H
#define __KINECT_BRIDGE_KBDEBUG_H

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


#include <QVariant>
#include <QString>
#include <QByteArray>

std::ostream& operator <<(std::ostream &str, const QVariant &v);
std::ostream& operator <<(std::ostream &str, const QString &v);
std::ostream& operator <<(std::ostream &str, const QByteArray &v);
std::ostream& operator <<(std::ostream &str, const QList<QVariantHash>& v);

template <typename T>
std::ostream& operator <<(std::ostream &str, const QList<T> &list)
{
    str << '(';
    for (Q_TYPENAME QList<T>::size_type i = 0; i < list.count(); ++i) {
        if (i) {
            str << ", ";
		}
        str << list.at(i);
    }
    str << ')';
	return str;
}

template <typename T>
std::ostream& operator <<(std::ostream &str, const QVector<T> &vector)
{
    return str << "QVector" << vector.toList();
}

template <class aKey, class aT>
std::ostream& operator <<(std::ostream &str, const QMap<aKey, aT> &map)
{
    str << "QMap(";
    for (typename QMap<aKey, aT>::const_iterator it = map.constBegin();
         it != map.constEnd(); ++it) {
        str << '(' << it.key() << ", " << it.value() << ')';
    }
    str << ')';
    return str;
}

template <class aKey, class aT>
std::ostream& operator <<(std::ostream &str, QHash<aKey, aT> &hash)
{
    str << "QHash(";
    for (typename QHash<aKey, aT>::const_iterator it = hash.constBegin();
            it != hash.constEnd(); ++it)
        str << '(' << it.key() << ", " << it.value() << ')';
    str << ')';
    return str;
}

template <class T1, class T2>
std::ostream& operator <<(std::ostream &str, const QPair<T1, T2> &pair)
{
    return str << "QPair(" << pair.first << ',' << pair.second << ')';
}

template <typename T>
std::ostream& operator <<(std::ostream &str, const QSet<T> &set)
{
    return str << "QSet" << set.toList();
}

void kbDebug_init();
void kbDebug_loadConfig(const std::string &filename, bool forceAdditivityOff = true);


#endif // __KINECT_BRIDGE_KBDEBUG_H
