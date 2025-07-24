#include "EngineCore/UI/ViewportPanel.hpp"
#include "EngineCore/Renderer.hpp"
#include "imgui.h"

ViewportPanel::ViewportPanel(Renderer& renderer)
    : m_renderer(renderer)
{
}

void ViewportPanel::OnImGuiRender()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");

    // Оновлюємо стан фокусу/наведення кожного кадру
    m_isHovered = ImGui::IsWindowHovered();

    ImTextureID textureId = m_renderer.GetImGuiTextureId();
    if (textureId)
    {
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        ImGui::Image(textureId, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
    }

    ImGui::End();
    ImGui::PopStyleVar();
}