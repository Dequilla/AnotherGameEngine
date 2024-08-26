#ifndef GE_LOGGING_H_
#define GE_LOGGING_H_

#include <vector>
#include <cstdint>
#include <string>
#include <format>

namespace ge::log
{
    enum class Severity
    {
        UNKNOWN,
        STATUS,
        WARNING,
        ERROR,
        CRITICAL
    };

    std::string severity_to_string(Severity severity);
    Severity severity_from_string(std::string severity);

    class Logger
    {
        std::vector<std::string> m_buffer;

    public:
        Logger();

        void log(Severity severity, const std::string& category, const std::string& msg);
        
        void status(const std::string& category, const std::string& msg);
        void warning(const std::string& category, const std::string& msg);
        void error(const std::string& category, const std::string& msg);
        void critical(const std::string& category, const std::string& msg);

        void flush();
    };
}

namespace ge
{
    extern ge::log::Logger logger;
}

#endif
