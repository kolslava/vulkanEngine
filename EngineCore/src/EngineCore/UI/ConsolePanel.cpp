#include "EngineCore/UI/ConsolePanel.hpp"
#include "imgui.h"

// Initialize the static message buffer for the console.
std::vector<std::string> ConsolePanel::s_MessageBuffer;

/**
 * @brief Adds a log message to the static buffer.
 * @param message The message to be added.
 */
void ConsolePanel::AddLog(const std::string& message)
{
    s_MessageBuffer.push_back(message);
    // TODO: Add a limit to the number of messages to avoid memory overflow.
    // For example, if s_MessageBuffer.size() > 1000, erase the oldest message.
}

/**
 * @brief Renders the console panel using ImGui.
 *
 * This function creates an ImGui window named "Console", iterates through
 * all messages in the static buffer, and displays them. It also includes
 * an auto-scrolling feature to keep the latest message in view.
 */
void ConsolePanel::OnImGuiRender()
{
    ImGui::Begin("Console");
    
    // Display all messages from the buffer.
    for (const auto& msg : s_MessageBuffer)
    {
        ImGui::TextUnformatted(msg.c_str());
    }

    // If the scroll bar is at the bottom, keep it there as new messages are added.
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::End();
}
