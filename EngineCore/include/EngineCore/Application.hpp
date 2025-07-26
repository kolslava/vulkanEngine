#pragma once

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

// Forward declarations to avoid including heavy headers like <vulkan/vulkan.h> and <GLFW/glfw3.h>
// in every file that includes Application.hpp. This speeds up compilation time.
struct GLFWwindow;

/**
 * @class Application
 * @brief The main class of the engine, responsible for initializing all systems,
 * managing the main loop, and handling events.
 */
class Application
{
public:
    /**
     * @brief Constructs the Application object.
     */
    Application();

    /**
     * @brief Destroys the Application object and cleans up resources.
     */
    ~Application();

    /**
     * @brief Starts the main application loop.
     */
    void run();

    /**
     * @brief Signals the application to close.
     */
    void close();

    /**
     * @brief Dispatches events to the appropriate handlers.
     * @param e The event to be processed.
     */
    void OnEvent(Event& e);

private:
    // --- Initialization and Cleanup ---

    /**
     * @brief Initializes all engine systems.
     */
    void init();

    /**
     * @brief Initializes the GLFW window.
     */
    void initWindow();

    /**
     * @brief Initializes the Vulkan renderer.
     */
    void initVulkan();

    /**
     * @brief Initializes the ImGui user interface.
     */
    void initImGui();

    /**
     * @brief The main application loop where frames are rendered.
     */
    void mainLoop();

    /**
     * @brief Cleans up all allocated resources.
     */
    void cleanup();

    // --- Vulkan Initialization Helpers ---

    /**
     * @brief Creates the Vulkan instance.
     */
    void createInstance();

    /**
     * @brief Creates the window surface for rendering.
     */
    void createSurface();

    /**
     * @brief Selects a suitable physical device (GPU).
     */
    void pickPhysicalDevice();

    /**
     * @brief Creates the logical device for interacting with the GPU.
     */
    void createLogicalDevice();

    /**
     * @brief Creates the swap chain for presenting images to the screen.
     */
    void createSwapChain();

    /**
     * @brief Creates image views for the swap chain images.
     */
    void createImageViews();

    /**
     * @brief Creates the render pass.
     */
    void createRenderPass();

    /**
     * @brief Creates framebuffers for the swap chain image views.
     */
    void createFramebuffers();

    /**
     * @brief Creates the command pool for allocating command buffers.
     */
    void createCommandPool();

    /**
     * @brief Creates the command buffers for recording rendering commands.
     */
    void createCommandBuffers();

    /**
     * @brief Creates synchronization objects (semaphores and fences).
     */
    void createSyncObjects();

    // --- Rendering ---

    /**
     * @brief Renders a single frame.
     */
    void drawFrame();

    /**
     * @brief Recreates the swap chain when the window is resized.
     */
    void recreateSwapChain();

    /**
     * @brief Cleans up the old swap chain before recreation.
     */
    void cleanupSwapChain();

    // --- Event Handlers ---

    /**
     * @brief Handles the window close event.
     * @param e The window close event.
     * @return True if the event was handled, false otherwise.
     */
    bool OnWindowClose(WindowCloseEvent& e);

    /**
     * @brief Handles mouse button press events.
     * @param e The mouse button pressed event.
     * @return True if the event was handled, false otherwise.
     */
    bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

    /**
     * @brief Handles mouse button release events.
     * @param e The mouse button released event.
     * @return True if the event was handled, false otherwise.
     */
    bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);

    /**
     * @brief Handles mouse move events.
     * @param e The mouse moved event.
     * @return True if the event was handled, false otherwise.
     */
    bool OnMouseMoved(MouseMovedEvent& e);

    /**
     * @brief Handles mouse scroll events.
     * @param e The mouse scrolled event.
     * @return True if the event was handled, false otherwise.
     */
    bool OnMouseScrolled(MouseScrolledEvent& e);

    /**
     * @brief Handles key press events.
     * @param e The key pressed event.
     * @return True if the event was handled, false otherwise.
     */
    bool OnKeyPressed(KeyPressedEvent& e);

    // --- Camera ---

    /**
     * @brief Processes keyboard input for camera movement.
     * @param deltaTime The time since the last frame.
     */
    void processCameraKeyboardInput(float deltaTime);

    // --- Window and State ---
    GLFWwindow* m_window = nullptr;       ///< Pointer to the GLFW window.
    const int m_width = 1280;             ///< The width of the window.
    const int m_height = 720;             ///< The height of the window.
    bool m_framebufferResized = false;    ///< Flag indicating if the framebuffer was resized.

    // --- Vulkan Objects ---
    VkInstance m_instance;                ///< The Vulkan instance.
    VkSurfaceKHR m_surface;               ///< The window surface.
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE; ///< The physical device (GPU).
    VkDevice m_device;                    ///< The logical device.
    VkQueue m_graphicsQueue;              ///< The graphics queue.
    VkQueue m_presentQueue;               ///< The presentation queue.
    VkSwapchainKHR m_swapChain;           ///< The swap chain.
    std::vector<VkImage> m_swapChainImages; ///< The images of the swap chain.
    VkFormat m_swapChainImageFormat;      ///< The image format of the swap chain.
    VkExtent2D m_swapChainExtent;         ///< The extent (resolution) of the swap chain.
    std::vector<VkImageView> m_swapChainImageViews; ///< The image views of the swap chain.
    std::vector<VkFramebuffer> m_swapChainFramebuffers; ///< The framebuffers for the swap chain.
    VkRenderPass m_renderPass;            ///< The render pass.
    VkDescriptorPool m_descriptorPool;    ///< The descriptor pool for ImGui.
    VkCommandPool m_commandPool;          ///< The command pool.
    std::vector<VkCommandBuffer> m_commandBuffers; ///< The command buffers.
    VkPhysicalDeviceProperties m_deviceProperties; ///< The properties of the physical device.

    // --- Synchronization ---
    std::vector<VkSemaphore> m_imageAvailableSemaphores; ///< Signals when an image is available for rendering.
    std::vector<VkSemaphore> m_renderFinishedSemaphores; ///< Signals when rendering is finished.
    std::vector<VkFence> m_inFlightFences; ///< Fences to prevent rendering to an image that is already in use.
    uint32_t m_currentFrame = 0;          ///< The index of the current frame.

    // --- Engine Systems ---
    std::unique_ptr<Renderer> m_renderer; ///< The main renderer.
    std::unique_ptr<Camera> m_camera;     ///< The main camera.

    // --- UI ---
    std::vector<std::unique_ptr<UIPanel>> m_UIPanels; ///< A list of all UI panels.
    ViewportPanel* m_viewportPanel = nullptr; ///< Pointer to the viewport panel for specific interactions.

    // --- Camera Control ---
    glm::vec2 m_lastMousePosition{0.0f, 0.0f}; ///< The last recorded mouse position.
    bool m_isCameraControlActive = false;      ///< Flag indicating if camera control is active.
    float m_lastFrameTime = 0.0f;              ///< The time of the last frame, used for calculating delta time.
};
