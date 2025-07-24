#pragma once
#include "EngineCore/UI/UIPanel.hpp"

class Renderer;

class ViewportPanel : public UIPanel
{
public:
    ViewportPanel(Renderer& renderer);
    void OnImGuiRender() override;

    // Новий метод, щоб повідомити, чи активне вікно
    bool IsHovered() const { return m_isHovered; }

private:
    Renderer& m_renderer;
    bool m_isHovered = false; // Нова змінна для зберігання стану
};