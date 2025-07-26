#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include "imgui.h"

#include <vector>
#include <memory>
#include <string>

/**
 * @struct UniformBufferObject
 * @brief Defines the structure of the uniform buffer that will be sent to the vertex shader.
 * It contains the model, view, and projection matrices for 3D transformation.
 */
struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

/**
 * @struct Vertex
 * @brief Defines the structure of a single vertex.
 * It contains the position and color attributes.
 */
struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
};

/**
 * @class Renderer
 * @brief Manages all rendering operations for the 3D scene.
 *
 * This class is responsible for setting up the Vulkan graphics pipeline,
 * managing buffers (vertex, index, uniform), and rendering the scene
 * to an offscreen framebuffer (texture). This texture can then be displayed
 * in the UI, for example, within an ImGui window.
 */
class Renderer
{
public:
    /**
     * @brief Constructs the Renderer object.
     * @param device The logical Vulkan device.
     * @param physicalDevice The physical Vulkan device (GPU).
     * @param commandPool The command pool for creating command buffers.
     * @param graphicsQueue The queue for submitting graphics commands.
     * @param framesInFlight The number of frames to be processed concurrently.
     * @param swapChainExtent The initial extent (size) of the scene.
     */
    Renderer(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, uint32_t framesInFlight, VkExtent2D swapChainExtent);
    
    /**
     * @brief Destroys the Renderer object and cleans up all Vulkan resources.
     */
    ~Renderer();

    // Renderers are resource-heavy and shouldn't be copied.
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    /**
     * @brief Renders the scene for a specific frame.
     * @param currentFrame The index of the current frame in flight.
     */
    void Render(uint32_t currentFrame);

    /**
     * @brief Updates the view and projection matrices from the camera.
     * @param view The new view matrix.
     * @param projection The new projection matrix.
     */
    void SetViewProjection(const glm::mat4& view, const glm::mat4& projection);

    /**
     * @brief Gets the ImGui texture ID for the rendered scene.
     * @return The ImTextureID that can be used with ImGui::Image().
     */
    ImTextureID GetImGuiTextureId() const { return m_sceneTextureId; }

    /**
     * @brief Handles window resize events.
     * @param newSize The new size of the viewport.
     */
    void OnResize(VkExtent2D newSize);

    // --- Scene Object Getters (for UI manipulation) ---
    glm::vec3& GetCubePosition() { return m_cubePosition; }
    glm::vec3& GetCubeRotation() { return m_cubeRotation; }
    glm::vec3& GetCubeScale() { return m_cubeScale; }

private:
    // --- Initialization Flow ---
    void createRenderPass();
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    void createFramebuffer();
    void createCubeBuffers();
    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();

    /**
     * @brief Updates the uniform buffer for the current frame with the latest matrices.
     * @param currentFrame The index of the current frame in flight.
     */
    void updateUniformBuffer(uint32_t currentFrame);

    // --- Vulkan Helper Functions ---
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    VkFormat findDepthFormat();
    VkShaderModule createShaderModule(const std::vector<char>& code);
    static std::vector<char> ReadFile(const std::string& filename);

    // --- Core Vulkan Handles (owned by Application, used by Renderer) ---
    VkDevice m_device;
    VkPhysicalDevice m_physicalDevice;
    VkCommandPool m_commandPool;
    VkQueue m_graphicsQueue;
    
    // --- State ---
    VkExtent2D m_sceneExtent;
    uint32_t m_framesInFlight;

    // --- Offscreen Rendering Resources ---
    VkImage m_sceneImage = VK_NULL_HANDLE;
    VkDeviceMemory m_sceneImageMemory = VK_NULL_HANDLE;
    VkImageView m_sceneImageView = VK_NULL_HANDLE;
    VkSampler m_sceneSampler = VK_NULL_HANDLE;
    VkFramebuffer m_sceneFramebuffer = VK_NULL_HANDLE;
    VkRenderPass m_sceneRenderPass = VK_NULL_HANDLE;
    ImTextureID m_sceneTextureId = 0;

    // --- Depth Buffer Resources ---
    VkImage m_depthImage = VK_NULL_HANDLE;
    VkDeviceMemory m_depthImageMemory = VK_NULL_HANDLE;
    VkImageView m_depthImageView = VK_NULL_HANDLE;
    
    // --- Graphics Pipeline ---
    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
    VkPipeline m_graphicsPipeline = VK_NULL_HANDLE;

    // --- Mesh Buffers (for the cube) ---
    VkBuffer m_vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory m_vertexBufferMemory = VK_NULL_HANDLE;
    VkBuffer m_indexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory m_indexBufferMemory = VK_NULL_HANDLE;

    // --- Uniform Buffer Resources ---
    VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;
    VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> m_descriptorSets;
    std::vector<VkBuffer> m_uniformBuffers;
    std::vector<VkDeviceMemory> m_uniformBuffersMemory;

    // --- Matrices ---
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    // --- Scene Object Properties (for the cube) ---
    glm::vec3 m_cubePosition = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_cubeRotation = { 0.0f, 0.0f, 0.0f }; // In degrees
    glm::vec3 m_cubeScale = { 1.0f, 1.0f, 1.0f };
};
