#pragma once

#include "UIPanel.hpp"
#include <vulkan/vulkan.h> // Потрібно для VkPhysicalDeviceProperties

// Панель, що відображає інформацію про GPU
class DeviceInfoPanel : public UIPanel
{
public:
    // Передаємо властивості GPU при створенні панелі
    DeviceInfoPanel(const VkPhysicalDeviceProperties& properties);

    void OnImGuiRender() override;

private:
    const VkPhysicalDeviceProperties& m_deviceProperties;
};