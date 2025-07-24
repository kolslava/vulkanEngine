#include "EngineCore/UI/SceneHierarchyPanel.hpp"
#include "imgui.h"

void SceneHierarchyPanel::OnImGuiRender()
{
    ImGui::Begin("Scene Hierarchy");
    ImGui::Text("This is where the scene object tree will be.");
    ImGui::End();
}