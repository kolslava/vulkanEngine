#include "EngineCore/UI/ConsolePanel.hpp"
#include "imgui.h"

// Ініціалізуємо статичний член класу
std::vector<std::string> ConsolePanel::s_MessageBuffer;

void ConsolePanel::AddLog(const std::string& message)
{
    s_MessageBuffer.push_back(message);
    // Можна додати обмеження на кількість повідомлень, щоб не переповнювати пам'ять
}

void ConsolePanel::OnImGuiRender()
{
    ImGui::Begin("Console");
    
    // Відображаємо всі повідомлення з буфера
    for (const auto& msg : s_MessageBuffer)
    {
        ImGui::TextUnformatted(msg.c_str());
    }

    // Автоматично прокручуємо вниз, коли з'являється нове повідомлення
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::End();
}