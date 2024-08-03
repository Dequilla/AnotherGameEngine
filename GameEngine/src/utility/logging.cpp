#include "utility/logging.hpp"

#include <iostream>
#include <format>
#include <chrono>
#include <algorithm>

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
    
    Severity severity_from_string(std::string severity)
    {
        std::transform(severity.begin(), severity.end(), severity.begin(), ::toupper);

        if(severity == "STATUS")
        {
            return Severity::STATUS;
        }
        else if(severity == "WARNING")
        {
            return Severity::WARNING;
        }
        else if(severity == "ERROR")
        {
            return Severity::ERROR;
        }
        else if(severity == "CRITICAL")
        {
            return Severity::CRITICAL;
        }
        else
        {
            return Severity::UNKNOWN;
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
        
    void Logger::status(const std::string& category, const std::string& msg)
    {
        log(Severity::STATUS, category, msg);
    }

    void Logger::warning(const std::string& category, const std::string& msg)
    {
        log(Severity::WARNING, category, msg);
    }

    void Logger::error(const std::string& category, const std::string& msg)
    {
        log(Severity::ERROR, category, msg);
    }

    void Logger::critical(const std::string& category, const std::string& msg)
    {
        log(Severity::CRITICAL, category, msg);
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

namespace ge
{
    ge::log::Logger logger;
}
