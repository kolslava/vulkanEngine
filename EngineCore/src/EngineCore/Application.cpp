#include "EngineCore/Application.hpp"
#include "EngineCore/Logger.hpp"

#include "EngineCore/UI/DeviceInfoPanel.hpp"
#include "EngineCore/UI/MainMenuPanel.hpp"
#include "EngineCore/UI/SceneHierarchyPanel.hpp"
#include "EngineCore/UI/InspectorPanel.hpp"
#include "EngineCore/UI/ConsolePanel.hpp"
#include "EngineCore/UI/ViewportPanel.hpp"
#include "EngineCore/UI/SystemInfoPanel.hpp"

#include "EngineCore/Renderer.hpp"

#include "EngineCore/Camera.hpp"

#include "EngineCore/Events/ApplicationEvent.hpp"
#include "EngineCore/Events/KeyEvent.hpp"
#include "EngineCore/Events/MouseEvent.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

#include <stdexcept>
#include <vector>
#include <iostream>

Application::Application()
{
    init();
}

Application::~Application()
{
    cleanup();
}

void Application::init()
{
    Log::GetCoreLogger()->info("Starting application initialization...");
    initWindow();
    initVulkan();
    initImGui();

    // Створюємо рендерер ПІСЛЯ ініціалізації Vulkan, передаючи йому необхідні ресурси
    m_renderer = std::make_unique<Renderer>(m_device, m_physicalDevice, m_commandPool, m_graphicsQueue, 2, m_swapChainExtent);

    // Створюємо камеру
    m_camera = std::make_unique<Camera>(45.0f, (float)m_width / (float)m_height, 0.1f, 100.0f);

    // Створюємо та додаємо всі наші UI панелі до списку
    m_UIPanels.push_back(std::make_unique<MainMenuPanel>(this));
    m_UIPanels.push_back(std::make_unique<DeviceInfoPanel>(m_deviceProperties));
    m_UIPanels.push_back(std::make_unique<SystemInfoPanel>(m_physicalDevice));
    auto viewportPanel = std::make_unique<ViewportPanel>(*m_renderer);
    m_viewportPanel = viewportPanel.get(); // Зберігаємо вказівник
    m_UIPanels.push_back(std::move(viewportPanel));
    m_UIPanels.push_back(std::make_unique<SceneHierarchyPanel>());
    m_UIPanels.push_back(std::make_unique<InspectorPanel>(*m_renderer));
    m_UIPanels.push_back(std::make_unique<ConsolePanel>());
    
    Log::GetCoreLogger()->info("Application initialized successfully.");
}

void Application::run()
{
    mainLoop();
}

void Application::close()
{
    glfwSetWindowShouldClose(m_window, true);
}

void Application::initWindow()
{
    Log::GetCoreLogger()->info("Initializing GLFW and creating a window...");
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(m_width, m_height, "Vulkan Engine", nullptr, nullptr);
    if (!m_window) {
        throw std::runtime_error("Failed to create GLFW window!");
    }

    // --- ДОДАЙТЕ ЦЕЙ БЛОК ---
    glfwSetWindowUserPointer(m_window, this);

    // Встановлюємо callback для закриття вікна
    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
    {
        Application& app = *(Application*)glfwGetWindowUserPointer(window);
        WindowCloseEvent e; // Створюємо нашу подію
        app.OnEvent(e); // Відправляємо її на обробку
    });

    // Встановлюємо callback для кнопок миші
    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
    {
        Application& app = *(Application*)glfwGetWindowUserPointer(window);
        switch (action)
        {
            case GLFW_PRESS:
            {
                MouseButtonPressedEvent e(button);
                app.OnEvent(e);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent e(button);
                app.OnEvent(e);
                break;
            }
        }
    });

    // Встановлюємо callback для руху миші
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos)
    {
        Application& app = *(Application*)glfwGetWindowUserPointer(window);
        MouseMovedEvent e((float)xpos, (float)ypos);
        app.OnEvent(e);
    });

    // Встановлюємо callback для скролу
    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset)
    {
        Application& app = *(Application*)glfwGetWindowUserPointer(window);
        MouseScrolledEvent e((float)xoffset, (float)yoffset);
        app.OnEvent(e);
    });
    // -------------------------

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Application& app = *(Application*)glfwGetWindowUserPointer(window);
        switch (action)
        {
            case GLFW_PRESS:
            {
                KeyPressedEvent e(key);
                app.OnEvent(e);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent e(key);
                app.OnEvent(e);
                break;
            }
        }
    });

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
    {
        Application& app = *(Application*)glfwGetWindowUserPointer(window);
        app.m_framebufferResized = true;
    });
}

void Application::initVulkan()
{
    Log::GetCoreLogger()->info("--- Initializing Vulkan ---");
    createInstance();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createRenderPass();
    createCommandPool();
    createFramebuffers();
    createCommandBuffers();
    createSyncObjects();
}

void Application::initImGui()
{
    Log::GetCoreLogger()->info("--- Initializing ImGui ---");

    // 1. Створення пулу дескрипторів
    VkDescriptorPoolSize pool_sizes[] =
    {
        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };
    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1000 * std::size(pool_sizes);
    pool_info.poolSizeCount = (uint32_t)std::size(pool_sizes);
    pool_info.pPoolSizes = pool_sizes;
    if (vkCreateDescriptorPool(m_device, &pool_info, nullptr, &m_descriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor pool for ImGui!");
    }

    // 2. Ініціалізація ImGui та увімкнення докінгу
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    // 3. Налаштування бекендів
    ImGui_ImplGlfw_InitForVulkan(m_window, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = m_instance;
    init_info.PhysicalDevice = m_physicalDevice;
    init_info.Device = m_device;
    init_info.QueueFamily = 0;
    init_info.Queue = m_graphicsQueue;
    init_info.DescriptorPool = m_descriptorPool;
    init_info.MinImageCount = 2;
    init_info.ImageCount = static_cast<uint32_t>(m_swapChainImages.size());
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    
    // Передаємо RenderPass в структуру
    init_info.RenderPass = m_renderPass;
    
    // Викликаємо Init з одним аргументом
    ImGui_ImplVulkan_Init(&init_info);

    // 4. Завантаження шрифтів більше не потрібне!
    // ImGui_ImplVulkan_NewFrame() зробить це автоматично.
    Log::GetCoreLogger()->info("ImGui Initialized. Fonts will be uploaded on the first frame.");
}

void Application::mainLoop()
{
    Log::GetCoreLogger()->info("Entering main loop...");
    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
        drawFrame();
    }
    // Чекаємо, поки GPU завершить усі операції перед виходом
    vkDeviceWaitIdle(m_device);
}

void Application::drawFrame()
{
    // 1. Розраховуємо час, що пройшов з минулого кадру (deltaTime)
    float currentTime = (float)glfwGetTime();
    float deltaTime = currentTime - m_lastFrameTime;
    m_lastFrameTime = currentTime;

    // 2. Обробляємо ввід з клавіатури для плавного руху камери
    // Події миші (кліки, рух, скрол) обробляються через систему подій, яку ми налаштували раніше
    processCameraKeyboardInput(deltaTime);
    
    // 3. Оновлюємо дані для рендерингу
    // Передаємо актуальні матриці з камери в рендерер
    m_renderer->SetViewProjection(m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
    
    // 4. Рендеримо 3D-сцену в окрему текстуру
    m_renderer->Render(m_currentFrame);

    // 5. Рендеримо інтерфейс (UI) у головне вікно
    vkWaitForFences(m_device, 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);
    
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(m_device, m_swapChain, UINT64_MAX, m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || m_framebufferResized) {
        m_framebufferResized = false;
        recreateSwapChain();
        return; // Пропускаємо решту кадру
    }

    vkResetFences(m_device, 1, &m_inFlightFences[m_currentFrame]);
    vkResetCommandBuffer(m_commandBuffers[m_currentFrame], 0);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(m_commandBuffers[m_currentFrame], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    // Починаємо рендер-пас для головного вікна
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_renderPass;
    renderPassInfo.framebuffer = m_swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_swapChainExtent;
    VkClearValue clearColor = {{{0.1f, 0.1f, 0.1f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    
    vkCmdBeginRenderPass(m_commandBuffers[m_currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        
        // Початок нового кадру ImGui
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // Створення робочої області Dockspace
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        ImGui::Begin("DockSpace Window", nullptr, window_flags);
        ImGui::PopStyleVar(3);
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        ImGui::End();
        
        // Рендеринг всіх наших UI панелей
        for (const auto& panel : m_UIPanels)
        {
            panel->OnImGuiRender();
        }

        // Завершення кадру ImGui
        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_commandBuffers[m_currentFrame]);

    vkCmdEndRenderPass(m_commandBuffers[m_currentFrame]);

    if (vkEndCommandBuffer(m_commandBuffers[m_currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
    
    // Відправка команд на GPU та відображення кадру
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphores[m_currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffers[m_currentFrame];
    VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphores[m_currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, m_inFlightFences[m_currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }
    
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    VkSwapchainKHR swapChains[] = {m_swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    vkQueuePresentKHR(m_presentQueue, &presentInfo);
    
    m_currentFrame = (m_currentFrame + 1) % 2;
}

void Application::cleanupSwapChain()
{
    for (auto framebuffer : m_swapChainFramebuffers) {
        vkDestroyFramebuffer(m_device, framebuffer, nullptr);
    }

    // Звільняємо командні буфери, оскільки вони залежать від кількості зображень у swap chain
    vkFreeCommandBuffers(m_device, m_commandPool, static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());

    // Якщо ваш pipeline або render pass залежать від формату зображення (що в нашому випадку так),
    // їх теж потрібно видаляти, але в нашому простому випадку ми можемо їх не чіпати.
    // У складніших проектах тут було б більше коду очищення.

    for (auto imageView : m_swapChainImageViews) {
        vkDestroyImageView(m_device, imageView, nullptr);
    }
    
    vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
}

void Application::recreateSwapChain()
{
    // Чекаємо, поки вікно не стане нульового розміру (при згортанні)
    int width = 0, height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(m_window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(m_device);
    
    cleanupSwapChain();
    
    // Створюємо все заново
    createSwapChain();
    createImageViews();
    createFramebuffers();
    createCommandBuffers(); // <--- ОСЬ ВАЖЛИВИЙ РЯДОК, ЯКОГО НЕ ВИСТАЧАЛО

    // Повідомляємо рендерер про новий розмір
    m_renderer->OnResize(m_swapChainExtent);
}

void Application::cleanup()
{
    Log::GetCoreLogger()->info("--- Cleaning up resources ---");
    if (m_device) {
        vkDeviceWaitIdle(m_device);
    }

    m_renderer.reset();
    
    // Єдина правильна функція для очищення всіх ресурсів ImGui
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    // Очищення ресурсів Vulkan
    if (m_descriptorPool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr);
    }
    for (size_t i = 0; i < 2; i++) {
        vkDestroySemaphore(m_device, m_renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(m_device, m_imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(m_device, m_inFlightFences[i], nullptr);
    }
    if (m_commandPool != VK_NULL_HANDLE) {
        vkDestroyCommandPool(m_device, m_commandPool, nullptr);
    }
    for (auto framebuffer : m_swapChainFramebuffers) {
        vkDestroyFramebuffer(m_device, framebuffer, nullptr);
    }
    if (m_renderPass != VK_NULL_HANDLE) {
        vkDestroyRenderPass(m_device, m_renderPass, nullptr);
    }
    for (auto imageView : m_swapChainImageViews) {
        vkDestroyImageView(m_device, imageView, nullptr);
    }
    if (m_swapChain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
    }
    if (m_device) {
        vkDestroyDevice(m_device, nullptr);
    }
    if (m_surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    }
    if (m_instance) {
        vkDestroyInstance(m_instance, nullptr);
    }

    // Очищення GLFW
    glfwDestroyWindow(m_window);
    glfwTerminate();
    Log::GetCoreLogger()->info("Cleanup complete.");
}

void Application::createInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Engine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance!");
    }
    Log::GetCoreLogger()->info("Vulkan instance created.");
}

void Application::createSurface() {
    if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
    Log::GetCoreLogger()->info("Vulkan surface created.");
}

bool Application::OnKeyPressed(KeyPressedEvent& e)
{
    // Отримуємо ім'я клавіші від GLFW
    const char* keyName = glfwGetKeyName(e.GetKeyCode(), 0);
    if (keyName)
    {
        ConsolePanel::AddLog("Key Pressed: " + std::string(keyName));
    }
    else
    {
        // Для клавіш, що не мають імені (напр. Enter, Shift)
        ConsolePanel::AddLog("Key Pressed: code " + std::to_string(e.GetKeyCode()));
    }
    return false; // Повертаємо false, щоб інші системи теж могли обробити подію
}

void Application::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());
    m_physicalDevice = devices[0]; // Беремо першу знайдену відеокарту

    vkGetPhysicalDeviceProperties(m_physicalDevice, &m_deviceProperties);

    Log::GetCoreLogger()->info("Picked physical device: {0}", m_deviceProperties.deviceName);
}

void Application::createLogicalDevice() {
    uint32_t queueFamilyIndex = 0; // Припускаємо, що перша сім'я черг підтримує графіку
    
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    
    VkPhysicalDeviceFeatures deviceFeatures{};
    
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device!");
    }
    vkGetDeviceQueue(m_device, queueFamilyIndex, 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, queueFamilyIndex, 0, &m_presentQueue);
    Log::GetCoreLogger()->info("Logical device and queues created.");
}

void Application::createSwapChain() {
    m_swapChainImageFormat = VK_FORMAT_B8G8R8A8_SRGB;
    m_swapChainExtent = { (uint32_t)m_width, (uint32_t)m_height };
    
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_surface;
    createInfo.minImageCount = 2;
    createInfo.imageFormat = m_swapChainImageFormat;
    createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    createInfo.imageExtent = m_swapChainExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    uint32_t imageCount;
    vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, nullptr);
    m_swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, m_swapChainImages.data());
    Log::GetCoreLogger()->info("Swap chain created.");
}

void Application::createImageViews() {
    m_swapChainImageViews.resize(m_swapChainImages.size());
    for (size_t i = 0; i < m_swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = m_swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = m_swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        if (vkCreateImageView(m_device, &createInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }
    Log::GetCoreLogger()->info("Image views created.");
}

void Application::createRenderPass() {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = m_swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
    Log::GetCoreLogger()->info("Render pass created.");
}

void Application::createFramebuffers() {
    m_swapChainFramebuffers.resize(m_swapChainImageViews.size());
    for (size_t i = 0; i < m_swapChainImageViews.size(); i++) {
        VkImageView attachments[] = { m_swapChainImageViews[i] };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = m_swapChainExtent.width;
        framebufferInfo.height = m_swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
    Log::GetCoreLogger()->info("Framebuffers created.");
}

void Application::createCommandPool() {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = 0;

    if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
    Log::GetCoreLogger()->info("Command pool created.");
}

void Application::createCommandBuffers() {
    m_commandBuffers.resize(2); // Для подвійної буферизації
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) m_commandBuffers.size();

    if (vkAllocateCommandBuffers(m_device, &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
    Log::GetCoreLogger()->info("Command buffers created.");
}

void Application::createSyncObjects() {
    m_imageAvailableSemaphores.resize(2);
    m_renderFinishedSemaphores.resize(2);
    m_inFlightFences.resize(2);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < 2; i++) {
        if (vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(m_device, &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
    Log::GetCoreLogger()->info("Sync objects created.");
}

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return this->OnWindowClose(e); });
    dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& e) { return this->OnMouseButtonPressed(e); });
    dispatcher.Dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent& e) { return this->OnMouseButtonReleased(e); });
    dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent& e) { return this->OnMouseMoved(e); });
    dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& e) { return this->OnMouseScrolled(e); });
    dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& e) { return this->OnKeyPressed(e); });

    // Передаємо подію далі, наприклад, камері
    if (!e.Handled)
    {
        m_camera->OnEvent(e);
    }
}

void Application::processCameraKeyboardInput(float deltaTime)
{
    if (m_viewportPanel && m_viewportPanel->IsHovered()) {
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
            m_camera->ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
            m_camera->ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
            m_camera->ProcessKeyboard(CameraMovement::LEFT, deltaTime);
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
            m_camera->ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
        if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
            m_camera->ProcessKeyboard(CameraMovement::UP, deltaTime);
        if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
            m_camera->ProcessKeyboard(CameraMovement::DOWN, deltaTime);
    }
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
    close();
    return true;
}

bool Application::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
    ConsolePanel::AddLog("Mouse Button Pressed: " + std::to_string(e.GetMouseButton()));

    if (m_viewportPanel && m_viewportPanel->IsHovered() && e.GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT)
    {
        m_isCameraControlActive = true;
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        double x, y;
        glfwGetCursorPos(m_window, &x, &y);
        m_lastMousePosition = {(float)x, (float)y};
        return true;
    }
    return false;
}

bool Application::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
{
    ConsolePanel::AddLog("Mouse Button Released: " + std::to_string(e.GetMouseButton()));

    if (e.GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT)
    {
        m_isCameraControlActive = false;
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        return true;
    }
    return false;
}

bool Application::OnMouseMoved(MouseMovedEvent& e)
{
    if (m_isCameraControlActive)
    {
        glm::vec2 currentMousePosition = {e.GetX(), e.GetY()};
        glm::vec2 delta = currentMousePosition - m_lastMousePosition;
        m_lastMousePosition = currentMousePosition;

        m_camera->ProcessMouseRotate(delta);
        return true;
    }
    return false;
}

bool Application::OnMouseScrolled(MouseScrolledEvent& e)
{
    if (m_viewportPanel && m_viewportPanel->IsHovered())
    {
        ConsolePanel::AddLog("Mouse Scrolled: " + std::to_string(e.GetYOffset()));
        m_camera->ProcessMouseZoom(e.GetYOffset());
        return true;
    }
    return false;
}