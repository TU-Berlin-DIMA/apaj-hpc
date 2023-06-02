#ifndef APAJ_HPC_APAJLOGGER_HPP
#define APAJ_HPC_APAJLOGGER_HPP
#include <log4cxx/consoleappender.h>
#include <log4cxx/fileappender.h>
#include <log4cxx/logger.h>
#include <log4cxx/patternlayout.h>
#include <iostream>

using namespace log4cxx;
using namespace log4cxx::helpers;

namespace APAJ {
    static log4cxx::LoggerPtr APAJLogger(log4cxx::Logger::getLogger("APAJ Logger"));
    enum class DebugLevel : uint8_t {LOG_NONE, LOG_WARNING, LOG_DEBUG, LOG_INFO, LOG_TRACE};

#define APAJ_DEBUG(TEXT)                                                                                       \
do {                                                                                                                    \
    LOG4CXX_DEBUG(APAJLogger, TEXT);                                                                            \
} while (0)

#define APAJ_INFO(TEXT)                                                                                        \
do {                                                                                                                    \
    LOG4CXX_INFO(APAJLogger, TEXT);                                                                             \
} while (0)


#define APAJ_ERROR(TEXT)                                                                                       \
do {                                                                                                                    \
    LOG4CXX_ERROR(APAJLogger, TEXT);                                                                            \
} while (0)

#define APAJ_VERIFY(CONDITION, TEXT)                                                                           \
do {                                                                                                                    \
    if (!(CONDITION)) {                                                                                                 \
        APAJ_ERROR(TEXT);                                                                                               \
        throw std::runtime_error("Runtime Error on condition " #CONDITION);                                             \
    }                                                                                                                   \
} while (0)

    static std::string getDebugLevelAsString(DebugLevel level) {
        if (level == DebugLevel::LOG_NONE) {
            return "LOG_NONE";
        } else if (level == DebugLevel::LOG_WARNING) {
            return "LOG_WARNING";
        } else if (level == DebugLevel::LOG_DEBUG) {
            return "LOG_DEBUG";
        } else if (level == DebugLevel::LOG_INFO) {
            return "LOG_INFO";
        } else if (level == DebugLevel::LOG_TRACE) {
            return "LOG_TRACE";
        } else {
            return "UNKNOWN";
        }
    }

    static void setupLogging(const std::string& logFileName, DebugLevel level) {
        std::cout << "Logger: SETUP_LOGGING" << std::endl;
        // create PatternLayout
        log4cxx::LayoutPtr layoutPtr(
                new log4cxx::PatternLayout("%d{MMM dd yyyy HH:mm:ss:SSS} %c: %l %X{threadName} [%-5t] [%p] : %m%n"));

        // create FileAppender
        LOG4CXX_DECODE_CHAR(fileName, logFileName);
        log4cxx::FileAppenderPtr file(new log4cxx::FileAppender(layoutPtr, fileName));

        // create ConsoleAppender
        log4cxx::ConsoleAppenderPtr console(new log4cxx::ConsoleAppender(layoutPtr));

        if (level == DebugLevel::LOG_NONE) {
            APAJLogger->setLevel(log4cxx::Level::getOff());
        } else if (level == DebugLevel::LOG_WARNING) {
            APAJLogger->setLevel(log4cxx::Level::getWarn());
        } else if (level == DebugLevel::LOG_DEBUG) {
            APAJLogger->setLevel(log4cxx::Level::getDebug());
        } else if (level == DebugLevel::LOG_INFO) {
            APAJLogger->setLevel(log4cxx::Level::getInfo());
        } else if (level == DebugLevel::LOG_TRACE) {
            APAJLogger->setLevel(log4cxx::Level::getTrace());
        } else {
            APAJ_ERROR("setupLogging: log level not supported " << getDebugLevelAsString(level));
            throw Exception("Error while setup logging");
        }

        APAJLogger->addAppender(file);
        APAJLogger->addAppender(console);
    }
} // namespace APAJ

#endif //APAJ_HPC_APAJLOGGER_HPP
