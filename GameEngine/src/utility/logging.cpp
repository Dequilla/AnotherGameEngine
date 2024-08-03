#include "utility/logging.hpp"

#include <iostream>
#include <format>
#include <chrono>

namespace ge::log
{
    std::string severity_to_string(Severity severity)
    {
        switch(severity)
        {
            case Severity::STATUS:
                return "STATUS";
            case Severity::WARNING:
                return "WARNING";
            case Severity::ERROR:
                return "ERROR";
            case Severity::CRITICAL:
                return "CRITICAL";
            default:
                return "UNKNOWN";
        }
    }

    Logger::Logger()
    {
    }

    void Logger::log(Severity severity, const std::string& category, const std::string& msg)
    {
        m_buffer.push_back( 
            std::format(
                "[{:%FT%T}][{}][{}] => {}", 
                std::chrono::floor<std::chrono::seconds>( std::chrono::system_clock::now()), 
                severity_to_string(severity), category, msg
            ) 
        );
    }

    void Logger::flush()
    {
        for( const std::string& msg : m_buffer )
        {
            std::cout << msg << "\n";
        }

        std::cout << std::flush;
    }

}

ge::log::Logger logger;
