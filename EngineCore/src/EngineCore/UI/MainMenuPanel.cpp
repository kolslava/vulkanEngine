#include "EngineCore/UI/MainMenuPanel.hpp"
#include "EngineCore/Application.hpp"
#include "imgui.h"

MainMenuPanel::MainMenuPanel(Application* app) 
    : m_app(app) 
{
}

void MainMenuPanel::OnImGuiRender()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                m_app->close();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}