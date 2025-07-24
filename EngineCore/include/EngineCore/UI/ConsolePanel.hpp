#pragma once
#include "EngineCore/UI/UIPanel.hpp"
#include <vector>
#include <string>

class ConsolePanel : public UIPanel
{
public:
    ConsolePanel() = default;
    void OnImGuiRender() override;

    // Статичний метод для додавання логів з будь-якої частини програми
    static void AddLog(const std::string& message);

private:
    // Статичний буфер для зберігання повідомлень
    static std::vector<std::string> s_MessageBuffer;
};