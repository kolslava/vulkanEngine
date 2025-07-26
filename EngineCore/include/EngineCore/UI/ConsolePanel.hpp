#pragma once
#include "EngineCore/UI/UIPanel.hpp"
#include <vector>
#include <string>

/**
 * @class ConsolePanel
 * @brief A UI panel that displays log messages in a console-like window.
 *
 * This class uses a static buffer to store messages, allowing any part of the
 * application to add logs via the static `AddLog` method.
 */
class ConsolePanel : public UIPanel
{
public:
    ConsolePanel() = default;
    
    /**
     * @brief Renders the console window using ImGui.
     */
    void OnImGuiRender() override;

    /**
     * @brief Adds a message to the console's log buffer.
     *
     * This is a static method, so it can be called from anywhere in the code
     * without needing an instance of ConsolePanel.
     *
     * @param message The string message to add to the log.
     */
    static void AddLog(const std::string& message);

private:
    /// @brief A static buffer that stores all log messages for the console.
    static std::vector<std::string> s_MessageBuffer;
};
