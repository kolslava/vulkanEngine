#pragma once
#include "EngineCore/UI/UIPanel.hpp"

// Forward declaration of Renderer to avoid including the full header.
class Renderer;

/**
 * @class ViewportPanel
 * @brief A UI panel that displays the rendered 3D scene.
 *
 * This panel gets the rendered scene as a texture from the Renderer and
 * displays it within an ImGui window. It also tracks user interactions
 * like hovering, which can be used to control camera movement.
 */
class ViewportPanel : public UIPanel
{
public:
    /**
     * @brief Constructs a ViewportPanel.
     * @param renderer A reference to the main renderer, used to get the
     *                 texture ID of the rendered scene.
     */
    ViewportPanel(Renderer& renderer);

    /**
     * @brief Renders the viewport window and the scene texture using ImGui.
     */
    void OnImGuiRender() override;

    /**
     * @brief Checks if the mouse cursor is currently hovering over the viewport window.
     * @return True if the viewport is hovered, false otherwise.
     */
    bool IsHovered() const { return m_isHovered; }

private:
    /// @brief A reference to the renderer to get the scene texture.
    Renderer& m_renderer;
    
    /// @brief A flag indicating if the mouse is currently over the viewport.
    bool m_isHovered = false;
};
