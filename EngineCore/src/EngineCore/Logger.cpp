#include "EngineCore/Logger.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"

// Initialize static logger pointers
std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

/**
 * @brief Initializes the static loggers.
 * 
 * This function sets a global pattern for all log messages and creates
 * two thread-safe, color-coded console loggers: one for the engine ("ENGINE")
 * and one for the client application ("APP").
 */
void Log::Init()
{
    // Set the format for log messages: [Timestamp] LoggerName: Message
    spdlog::set_pattern("%^[%T] %n: %v%$");

    // Create the core logger with the name "ENGINE"
    s_CoreLogger = spdlog::stdout_color_mt("ENGINE");
    s_CoreLogger->set_level(spdlog::level::trace); // Log all messages from trace level upwards

    // Create the client logger with the name "APP"
    s_ClientLogger = spdlog::stdout_color_mt("APP");
    s_ClientLogger->set_level(spdlog::level::trace); // Log all messages from trace level upwards
}
