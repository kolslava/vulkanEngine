#pragma once
#include "EngineCore/UI/UIPanel.hpp"

/**
 * @class SceneHierarchyPanel
 * @brief A UI panel that displays the hierarchy of objects in the scene.
 *
 * This panel shows a list or tree view of all entities in the scene,
 * allowing the user to select them for inspection and manipulation.
 */
class SceneHierarchyPanel : public UIPanel
{
public:
    SceneHierarchyPanel() = default;

    /**
     * @brief Renders the scene hierarchy window using ImGui.
     */
    void OnImGuiRender() override;
};
