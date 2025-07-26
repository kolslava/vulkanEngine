#pragma once
#include "EngineCore/UI/UIPanel.hpp"
#include <vector>
#include <vulkan/vulkan.h>

// Platform-specific includes for performance querying
#ifdef _WIN32
#include <pdh.h>
#endif

/**
 * @class SystemInfoPanel
 * @brief A UI panel that displays real-time system performance metrics.
 *
 * This panel shows information such as CPU usage, RAM usage, and VRAM usage.
 * It uses platform-specific APIs (like PDH on Windows) to gather this data.
 */
class SystemInfoPanel : public UIPanel
{
public:
    /**
     * @brief Constructs a SystemInfoPanel.
     * @param physicalDevice A handle to the Vulkan physical device, used for querying VRAM information.
     */
    SystemInfoPanel(VkPhysicalDevice physicalDevice);

    /**
     * @brief Destroys the SystemInfoPanel and cleans up platform-specific resources.
     */
    ~SystemInfoPanel();

    /**
     * @brief Renders the system info window using ImGui.
     */
    void OnImGuiRender() override;

private:
    /**
     * @brief Updates the performance data (CPU, RAM, etc.).
     */
    void update();
    
    /// @brief Handle to the Vulkan physical device for VRAM queries.
    VkPhysicalDevice m_physicalDevice;
    
    // --- Data for graphs ---
    /// @brief Stores a history of CPU usage percentages for plotting.
    std::vector<float> m_cpuHistory;
    /// @brief Stores a history of RAM usage percentages for plotting.
    std::vector<float> m_ramHistory;

    // --- Current Metrics ---
    /// @brief Total physical RAM in GB.
    float m_totalRam = 0.0f;
    /// @brief Currently used physical RAM in GB.
    float m_usedRam = 0.0f;
    /// @brief Total dedicated VRAM in GB.
    float m_totalVram = 0.0f;

// --- Windows-specific members for CPU querying ---
#ifdef _WIN32
    /// @brief A handle to the PDH query.
    PDH_HQUERY m_cpuQuery;
    /// @brief A handle to the performance counter for total CPU usage.
    PDH_HCOUNTER m_cpuTotal;
#endif
};
