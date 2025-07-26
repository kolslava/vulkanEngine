#include <EngineCore/Application.hpp>
#include <EngineCore/Logger.hpp>
#include <stdexcept>
#include <iostream>
#include <memory> // Required for std::unique_ptr

/**
 * @brief The main entry point for the Vulkan Engine Editor application.
 * @return EXIT_SUCCESS on successful execution, EXIT_FAILURE on error.
 */
int main()
{
    // First, initialize the logging system.
    Log::Init();

    // Create the application instance using a smart pointer for automatic memory management.
    // This ensures that the Application destructor is called even if an exception occurs.
    std::unique_ptr<Application> app;
    try
    {
        app = std::make_unique<Application>();
    }
    catch (const std::exception& e)
    {
        // Log any critical errors that occur during application initialization.
        Log::GetCoreLogger()->critical("Application failed to initialize: {0}", e.what());
        return EXIT_FAILURE;
    }

    try
    {
        // Run the main application loop.
        app->run();
    }
    catch (const std::exception& e)
    {
        // Catch and log any unhandled exceptions that occur during the main loop.
        Log::GetClientLogger()->critical("Unhandled exception during runtime: {0}", e.what());
        return EXIT_FAILURE;
    }

    // The unique_ptr will automatically delete the application instance here.
    return EXIT_SUCCESS;
}
