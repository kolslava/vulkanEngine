#include "EngineCore/UI/DeviceInfoPanel.hpp"
#include "imgui.h"

DeviceInfoPanel::DeviceInfoPanel(const VkPhysicalDeviceProperties& properties)
    : m_deviceProperties(properties)
{
}

void DeviceInfoPanel::OnImGuiRender()
{
    // Починаємо нове вікно ImGui
    ImGui::Begin("Device Info");

    ImGui::Text("Device Name: %s", m_deviceProperties.deviceName);
    ImGui::Text("Device Type: %s", 
        m_deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? "Discrete GPU" :
        m_deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? "Integrated GPU" :
        "Other");

    ImGui::Separator(); // Розділювач

    ImGui::Text("Vulkan API Version: %d.%d.%d", 
        VK_VERSION_MAJOR(m_deviceProperties.apiVersion), 
        VK_VERSION_MINOR(m_deviceProperties.apiVersion), 
        VK_VERSION_PATCH(m_deviceProperties.apiVersion));
    
    ImGui::Text("Driver Version: %d", m_deviceProperties.driverVersion);

    // Закриваємо вікно ImGui
    ImGui::End();
}