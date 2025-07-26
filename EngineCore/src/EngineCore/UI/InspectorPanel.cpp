#include "EngineCore/UI/InspectorPanel.hpp"
#include "EngineCore/Renderer.hpp" // Include the full Renderer definition
#include "imgui.h"

/**
 * @brief Constructs the InspectorPanel.
 * @param renderer A reference to the renderer to access scene object properties.
 */
InspectorPanel::InspectorPanel(Renderer& renderer)
    : m_renderer(renderer)
{
}

/**
 * @brief Renders the Inspector panel using ImGui.
 *
 * This function creates an ImGui window that displays the transformation
 * properties (position, rotation, scale) of the cube in the scene.
 * It retrieves references to these properties from the Renderer and uses
 * ImGui widgets to allow for their direct manipulation.
 */
void InspectorPanel::OnImGuiRender()
{
    ImGui::Begin("Inspector");

    // Get references to the cube's transformation properties from the renderer.
    // Modifying these variables will directly affect the cube in the scene.
    glm::vec3& position = m_renderer.GetCubePosition();
    glm::vec3& rotation = m_renderer.GetCubeRotation();
    glm::vec3& scale = m_renderer.GetCubeScale();

    // Create ImGui widgets to edit the transformation values.
    ImGui::Text("Transform");
    ImGui::DragFloat3("Position", &position.x, 0.1f);
    ImGui::DragFloat3("Rotation", &rotation.x, 1.0f); // Rotation is in degrees
    ImGui::DragFloat3("Scale", &scale.x, 0.1f);
    
    ImGui::End();
}
