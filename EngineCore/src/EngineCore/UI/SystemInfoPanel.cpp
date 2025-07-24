#include "EngineCore/UI/SystemInfoPanel.hpp"
#include "imgui.h"
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "pdh.lib")
#endif

#include <cmath>

SystemInfoPanel::SystemInfoPanel(VkPhysicalDevice physicalDevice)
    : m_physicalDevice(physicalDevice)
{
    m_cpuHistory.resize(100, 0.0f);
    m_ramHistory.resize(100, 0.0f);

    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryHeapCount; i++) {
        if ((memProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) != 0) {
            m_totalVram += memProperties.memoryHeaps[i].size;
        }
    }
    m_totalVram /= (1024.0f * 1024.0f * 1024.0f);

#ifdef _WIN32
    // Ініціалізуємо лічильник CPU
    PdhOpenQueryA(NULL, 0ULL, &m_cpuQuery);
    PdhAddEnglishCounterA(m_cpuQuery, "\\Processor Information(_Total)\\% Processor Time", 0ULL, &m_cpuTotal);
    PdhCollectQueryData(m_cpuQuery);
#endif
}

SystemInfoPanel::~SystemInfoPanel()
{
#ifdef _WIN32
    // Очищуємо ресурси, створені в конструкторі
    PdhCloseQuery(m_cpuQuery);
#endif
}

void SystemInfoPanel::update()
{
    // RAM (код без змін)
#ifdef _WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    m_totalRam = memInfo.ullTotalPhys / (1024.0f * 1024.0f * 1024.0f);
    m_usedRam = (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / (1024.0f * 1024.0f * 1024.0f);
#endif

    // --- РЕАЛЬНЕ ОТРИМАННЯ ЗАВАНТАЖЕННЯ CPU ---
    float cpuLoad = 0.0f;
#ifdef _WIN32
    PDH_FMT_COUNTERVALUE counterVal;
    PdhCollectQueryData(m_cpuQuery);
    PdhGetFormattedCounterValue(m_cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    cpuLoad = static_cast<float>(counterVal.doubleValue);
#endif
    // ------------------------------------------

    m_cpuHistory.erase(m_cpuHistory.begin());
    m_cpuHistory.push_back(cpuLoad);
    m_ramHistory.erase(m_ramHistory.begin());
    m_ramHistory.push_back((m_usedRam / m_totalRam) * 100.0f);
}

void SystemInfoPanel::OnImGuiRender()
{
    update();

    ImGui::Begin("System Info");
    ImGui::Text("RAM: %.2f GB / %.2f GB", m_usedRam, m_totalRam);
    ImGui::Text("VRAM (Total): %.2f GB", m_totalVram);
    ImGui::Separator();
    ImGui::Text("CPU Load");
    ImGui::PlotLines("##cpu", m_cpuHistory.data(), m_cpuHistory.size(), 0, NULL, 0.0f, 100.0f, ImVec2(0, 80));
    ImGui::Text("RAM Usage");
    ImGui::PlotLines("##ram", m_ramHistory.data(), m_ramHistory.size(), 0, NULL, 0.0f, 100.0f, ImVec2(0, 80));
    ImGui::End();
}