#include "EngineCore/UI/DeviceInfoPanel.hpp"
#include "imgui.h"

/**
 * @brief Constructs the DeviceInfoPanel.
 * @param properties A reference to the physical device properties struct.
 */
DeviceInfoPanel::DeviceInfoPanel(const VkPhysicalDeviceProperties& properties)
    : m_deviceProperties(properties)
{
}

/**
 * @brief Renders the Device Info panel using ImGui.
 *
 * This function creates an ImGui window that displays key properties of the
 * physical device (GPU), such as its name, type, and the supported Vulkan API version.
 */
void DeviceInfoPanel::OnImGuiRender()
{
    ImGui::Begin("Device Info");

    // Display basic device info
    ImGui::Text("Device Name: %s", m_deviceProperties.deviceName);

    // Convert device type enum to a human-readable string
    const char* deviceTypeStr = "Other";
    if (m_deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        deviceTypeStr = "Discrete GPU";
    } else if (m_deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
        deviceTypeStr = "Integrated GPU";
    }
    ImGui::Text("Device Type: %s", deviceTypeStr);

    ImGui::Separator();

    // Display version information
    ImGui::Text("Vulkan API Version: %d.%d.%d", 
        VK_VERSION_MAJOR(m_deviceProperties.apiVersion), 
        VK_VERSION_MINOR(m_deviceProperties.apiVersion), 
        VK_VERSION_PATCH(m_deviceProperties.apiVersion));
    
    ImGui::Text("Driver Version: %d", m_deviceProperties.driverVersion);

    ImGui::End();
}
