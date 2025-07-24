#pragma once
#include "EngineCore/UI/UIPanel.hpp"
#include <vector>
#include <vulkan/vulkan.h>

#ifdef _WIN32
#include <pdh.h>
#endif

class SystemInfoPanel : public UIPanel
{
public:
    SystemInfoPanel(VkPhysicalDevice physicalDevice);
    ~SystemInfoPanel();
    void OnImGuiRender() override;
private:
    void update();
    
    VkPhysicalDevice m_physicalDevice;
    
    std::vector<float> m_cpuHistory;
    std::vector<float> m_ramHistory;
    float m_totalRam = 0.0f;
    float m_usedRam = 0.0f;
    float m_totalVram = 0.0f;

#ifdef _WIN32
    PDH_HQUERY m_cpuQuery;
    PDH_HCOUNTER m_cpuTotal;
#endif
};