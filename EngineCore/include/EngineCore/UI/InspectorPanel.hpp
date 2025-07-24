#pragma once
#include "EngineCore/UI/UIPanel.hpp"

class Renderer; // Попереднє оголошення

class InspectorPanel : public UIPanel
{
public:
    // Конструктор тепер приймає посилання на рендерер
    InspectorPanel(Renderer& renderer);
    void OnImGuiRender() override;
private:
    Renderer& m_renderer;
};