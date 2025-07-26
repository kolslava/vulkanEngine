#pragma once

#include "UIPanel.hpp"
#include <vulkan/vulkan.h> // Needed for VkPhysicalDeviceProperties

/**
 * @class DeviceInfoPanel
 * @brief A UI panel that displays information about the selected physical device (GPU).
 */
class DeviceInfoPanel : public UIPanel
{
public:
    /**
     * @brief Constructs a DeviceInfoPanel.
     * @param properties A constant reference to the VkPhysicalDeviceProperties struct,
     *                   which contains information about the GPU.
     */
    DeviceInfoPanel(const VkPhysicalDeviceProperties& properties);

    /**
     * @brief Renders the device info window using ImGui.
     */
    void OnImGuiRender() override;

private:
    /// @brief A reference to the physical device properties, owned by the Application class.
    const VkPhysicalDeviceProperties& m_deviceProperties;
};
