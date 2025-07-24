#pragma once

// Щоб уникнути включення заголовків Vulkan та GLFW тут,
// використовуємо попереднє оголошення типів. Це прискорює компіляцію.
struct GLFWwindow;
using VkInstance = struct VkInstance_T*;
using VkSurfaceKHR = struct VkSurfaceKHR_T*;
using VkPhysicalDevice = struct VkPhysicalDevice_T*;
using VkDevice = struct VkDevice_T*;
using VkQueue = struct VkQueue_T*;
using VkSwapchainKHR = struct VkSwapchainKHR_T*;
using VkRenderPass = struct VkRenderPass_T*;
using VkDescriptorPool = struct VkDescriptorPool_T*;
using VkCommandPool = struct VkCommandPool_T*;

#include "EngineCore/UI/UIPanel.hpp"
#include "EngineCore/Renderer.hpp"
#include "EngineCore/Camera.hpp"

#include "EngineCore/Events/Event.hpp"
#include "EngineCore/Events/ApplicationEvent.hpp"
#include "EngineCore/Events/KeyEvent.hpp"

#include "EngineCore/UI/ViewportPanel.hpp"

#include <vector>
#include <memory>

#include <vulkan/vulkan.h>

class Application
{
public:
    Application();
    ~Application();

    void run();
    void close();

    void OnEvent(Event& e);

private:
    void init();
    void initWindow();
    void initVulkan();
    void initImGui();
    void mainLoop();
    void cleanup();

    // Допоміжні функції для ініціалізації Vulkan
    void createInstance();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffers();
    void createSyncObjects();

    // Рендеринг кадру
    void drawFrame();

    // Функції-обробники для конкретних подій
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
    bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
    bool OnMouseMoved(MouseMovedEvent& e);
    bool OnMouseScrolled(MouseScrolledEvent& e);
    bool OnKeyPressed(KeyPressedEvent& e);

    void cleanupSwapChain();
    void recreateSwapChain();
    bool m_framebufferResized = false;

    GLFWwindow* m_window = nullptr;
    const int m_width = 1280;
    const int m_height = 720;

    // Vulkan об'єкти
    VkInstance m_instance;
    VkSurfaceKHR m_surface;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_device;
    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;
    VkSwapchainKHR m_swapChain;
    std::vector<VkImage> m_swapChainImages;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;
    std::vector<VkImageView> m_swapChainImageViews;
    std::vector<VkFramebuffer> m_swapChainFramebuffers;
    VkRenderPass m_renderPass;
    VkDescriptorPool m_descriptorPool;
    VkCommandPool m_commandPool;
    std::vector<VkCommandBuffer> m_commandBuffers;

    VkPhysicalDeviceProperties m_deviceProperties; // Зберігаємо властивості тут

    // Список наших UI панелей
    std::vector<std::unique_ptr<UIPanel>> m_UIPanels;
    // Наш рендерер
    std::unique_ptr<Renderer> m_renderer;

    std::unique_ptr<Camera> m_camera;
    glm::vec2 m_lastMousePosition{0.0f, 0.0f};
    bool m_isCameraControlActive = false;
    float m_lastFrameTime = 0.0f;
    void processCameraKeyboardInput(float deltaTime);

    // Об'єкти для синхронізації
    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;
    uint32_t m_currentFrame = 0;

    ViewportPanel* m_viewportPanel = nullptr;
};