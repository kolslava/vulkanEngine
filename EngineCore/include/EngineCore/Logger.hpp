#pragma once

#include "spdlog/spdlog.h"
#include <memory>

/**
 * @class Log
 * @brief A static wrapper class for the spdlog library.
 *
 * Provides a centralized logging system with two distinct loggers:
 * - CoreLogger: For engine-internal messages.
 * - ClientLogger: For messages from the client application (the game or editor).
 */
class Log
{
public:
    /**
     * @brief Initializes the loggers. Must be called once at application startup.
     */
    static void Init();

    /**
     * @brief Gets the core engine logger instance.
     * @return A shared pointer to the core spdlog logger.
     */
    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

    /**
     * @brief Gets the client application logger instance.
     * @return A shared pointer to the client spdlog logger.
     */
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

private:
    /// @brief The static instance of the core logger.
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    
    /// @brief The static instance of the client logger.
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};
