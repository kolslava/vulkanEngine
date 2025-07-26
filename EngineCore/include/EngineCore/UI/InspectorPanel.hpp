#pragma once
#include "EngineCore/UI/UIPanel.hpp"

// Forward declaration of Renderer to avoid including the full header.
// This is a good practice to reduce compilation times.
class Renderer;

/**
 * @class InspectorPanel
 * @brief A UI panel for viewing and editing the properties of scene objects.
 *
 * In the current implementation, it provides controls to modify the
 * transformation (position, rotation, scale) of the cube being rendered.
 */
class InspectorPanel : public UIPanel
{
public:
    /**
     * @brief Constructs an InspectorPanel.
     * @param renderer A reference to the main renderer, used to access and
     *                 modify the properties of rendered objects.
     */
    InspectorPanel(Renderer& renderer);

    /**
     * @brief Renders the inspector window using ImGui.
     */
    void OnImGuiRender() override;

private:
    /// @brief A reference to the renderer to access scene object properties.
    Renderer& m_renderer;
};
