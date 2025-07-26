#include "EngineCore/UI/SceneHierarchyPanel.hpp"
#include "imgui.h"

/**
 * @brief Renders the Scene Hierarchy panel using ImGui.
 *
 * Currently, this is a placeholder implementation. In the future, this
 * function will iterate through the scene's objects and display them
 * in a tree or list view, allowing for selection.
 */
void SceneHierarchyPanel::OnImGuiRender()
{
    ImGui::Begin("Scene Hierarchy");
    
    // TODO: Implement the actual scene hierarchy tree here.
    ImGui::Text("This is where the scene object tree will be.");

    ImGui::End();
}
