#include "EngineCore/UI/ViewportPanel.hpp"
#include "EngineCore/Renderer.hpp"
#include "imgui.h"

/**
 * @brief Constructs the ViewportPanel.
 * @param renderer A reference to the renderer to get the scene texture.
 */
ViewportPanel::ViewportPanel(Renderer& renderer)
    : m_renderer(renderer)
{
}

/**
 * @brief Renders the Viewport panel using ImGui.
 *
 * This function creates an ImGui window that displays the 3D scene.
 * It gets the rendered scene as a texture from the Renderer and draws it
 * to fill the content area of the window. It also updates the hovered state.
 */
void ViewportPanel::OnImGuiRender()
{
    // Remove padding so the image fills the entire window.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");

    // Update the hovered state each frame.
    m_isHovered = ImGui::IsWindowHovered();

    // Get the texture ID for the rendered scene from the renderer.
    ImTextureID textureId = m_renderer.GetImGuiTextureId();
    if (textureId)
    {
        // Get the available content region size to make the image fill the panel.
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        
        // Display the image. The UV coordinates are flipped vertically (from (0,0)-(1,1) to (0,1)-(1,0))
        // because Vulkan's screen coordinates are often inverted compared to ImGui's texture coordinates.
        ImGui::Image(textureId, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
    }

    ImGui::End();
    ImGui::PopStyleVar();
}
