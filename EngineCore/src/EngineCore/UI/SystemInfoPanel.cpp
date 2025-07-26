#include "EngineCore/UI/SystemInfoPanel.hpp"
#include "imgui.h"
#include <chrono>
#include <cmath>

// Platform-specific includes and libraries for performance querying
#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "pdh.lib") // Link against the PDH library
#endif

/**
 * @brief Constructs the SystemInfoPanel.
 * @param physicalDevice Handle to the Vulkan physical device for VRAM queries.
 */
SystemInfoPanel::SystemInfoPanel(VkPhysicalDevice physicalDevice)
    : m_physicalDevice(physicalDevice)
{
    // Initialize history buffers with 100 zero values.
    m_cpuHistory.resize(100, 0.0f);
    m_ramHistory.resize(100, 0.0f);

    // Query total VRAM from the physical device
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryHeapCount; i++) {
        // Sum up the sizes of all device-local memory heaps
        if ((memProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) != 0) {
            m_totalVram += memProperties.memoryHeaps[i].size;
        }
    }
    // Convert from bytes to gigabytes
    m_totalVram /= (1024.0f * 1024.0f * 1024.0f);

#ifdef _WIN32
    // Initialize the Windows Performance Data Helper (PDH) for CPU usage querying.
    PdhOpenQueryA(NULL, 0, &m_cpuQuery);
    // Add a counter for the total processor time across all cores.
    PdhAddEnglishCounterA(m_cpuQuery, "\\Processor Information(_Total)\\% Processor Time", 0, &m_cpuTotal);
    PdhCollectQueryData(m_cpuQuery); // Collect an initial sample.
#endif
}

/**
 * @brief Destroys the SystemInfoPanel.
 */
SystemInfoPanel::~SystemInfoPanel()
{
#ifdef _WIN32
    // Clean up the PDH query resources.
    PdhCloseQuery(m_cpuQuery);
#endif
}

/**
 * @brief Updates the system performance metrics.
 */
void SystemInfoPanel::update()
{
#ifdef _WIN32
    // --- Update RAM Usage (Windows) ---
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    m_totalRam = memInfo.ullTotalPhys / (1024.0f * 1024.0f * 1024.0f);
    m_usedRam = (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / (1024.0f * 1024.0f * 1024.0f);

    // --- Update CPU Usage (Windows) ---
    PDH_FMT_COUNTERVALUE counterVal;
    PdhCollectQueryData(m_cpuQuery);
    PdhGetFormattedCounterValue(m_cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    float cpuLoad = static_cast<float>(counterVal.doubleValue);
#else
    // --- Placeholder for other platforms ---
    float cpuLoad = 0.0f; // No implementation for non-Windows platforms yet
    m_totalRam = 0.0f;
    m_usedRam = 0.0f;
#endif

    // Update history buffers for the graphs
    m_cpuHistory.erase(m_cpuHistory.begin());
    m_cpuHistory.push_back(cpuLoad);
    
    m_ramHistory.erase(m_ramHistory.begin());
    m_ramHistory.push_back(m_totalRam > 0 ? (m_usedRam / m_totalRam) * 100.0f : 0.0f);
}

/**
 * @brief Renders the System Info panel using ImGui.
 */
void SystemInfoPanel::OnImGuiRender()
{
    // Refresh the data on each render call.
    update();

    ImGui::Begin("System Info");

    // Display RAM and VRAM information
    ImGui::Text("RAM: %.2f GB / %.2f GB", m_usedRam, m_totalRam);
    ImGui::Text("VRAM (Total): %.2f GB", m_totalVram);
    ImGui::Separator();

    // Plot CPU load history
    ImGui::Text("CPU Load (%%)");
    ImGui::PlotLines("##cpu", m_cpuHistory.data(), m_cpuHistory.size(), 0, NULL, 0.0f, 100.0f, ImVec2(0, 80));
    
    // Plot RAM usage history
    ImGui::Text("RAM Usage (%%)");
    ImGui::PlotLines("##ram", m_ramHistory.data(), m_ramHistory.size(), 0, NULL, 0.0f, 100.0f, ImVec2(0, 80));
    
    ImGui::End();
}
