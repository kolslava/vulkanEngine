#include "EngineCore/UI/MainMenuPanel.hpp"
#include "EngineCore/Application.hpp" // Include the full Application definition
#include "imgui.h"

/**
 * @brief Constructs the MainMenuPanel.
 * @param app A pointer to the Application instance.
 */
MainMenuPanel::MainMenuPanel(Application* app) 
    : m_app(app) 
{
}

/**
 * @brief Renders the main menu bar using ImGui.
 *
 * This function creates the main menu bar at the top of the application window.
 * It currently contains a "File" menu with an "Exit" option that closes the application.
 */
void MainMenuPanel::OnImGuiRender()
{
    // Begin the main menu bar. This will span the top of the main viewport.
    if (ImGui::BeginMainMenuBar())
    {
        // Create the "File" menu.
        if (ImGui::BeginMenu("File"))
        {
            // Add an "Exit" menu item.
            if (ImGui::MenuItem("Exit"))
            {
                // If "Exit" is clicked, call the close() method on the Application.
                m_app->close();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
