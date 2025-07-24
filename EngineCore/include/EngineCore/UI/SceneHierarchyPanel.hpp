#pragma once
#include "EngineCore/UI/UIPanel.hpp"

class SceneHierarchyPanel : public UIPanel
{
public:
    SceneHierarchyPanel() = default;
    void OnImGuiRender() override;
};