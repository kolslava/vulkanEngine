#pragma once
#include "EngineCore/UI/UIPanel.hpp"

// Вперед оголошуємо клас, щоб не включати сюди повний Application.hpp
class Application; 

class MainMenuPanel : public UIPanel
{
public:
    MainMenuPanel(Application* app);
    void OnImGuiRender() override;
private:
    Application* m_app;
};