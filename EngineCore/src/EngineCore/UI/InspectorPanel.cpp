#include "EngineCore/UI/InspectorPanel.hpp"
#include "EngineCore/Renderer.hpp"
#include "imgui.h"

InspectorPanel::InspectorPanel(Renderer& renderer)
    : m_renderer(renderer)
{
}

void InspectorPanel::OnImGuiRender()
{
    ImGui::Begin("Inspector");

    // Отримуємо доступ до властивостей куба через геттери
    glm::vec3& position = m_renderer.GetCubePosition();
    glm::vec3& rotation = m_renderer.GetCubeRotation();
    glm::vec3& scale = m_renderer.GetCubeScale();

    // Створюємо віджети для редагування
    ImGui::Text("Transform");
    ImGui::DragFloat3("Position", &position.x, 0.1f);
    ImGui::DragFloat3("Rotation", &rotation.x, 1.0f);
    ImGui::DragFloat3("Scale", &scale.x, 0.1f);
    
    ImGui::End();
}