#ifndef GE_LOGGING_H_
#define GE_LOGGING_H_

#include <vector>
#include <cstdint>
#include <string>

namespace ge::log
{
    enum class Severity
    {
        STATUS,
        WARNING,
        ERROR,
        CRITICAL
    };

    class Logger
    {
        std::vector<std::string> m_buffer;

    public:
        Logger();

        void log(Severity severity, const std::string& category, const std::string& msg);

        void flush();
    };
}

extern ge::log::Logger logger;

#endif
