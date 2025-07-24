#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include <string>
#include "imgui.h"
#include <glm/glm.hpp>

// Структура, що відповідає uniform-буферу в шейдері
struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
};

class Renderer
{
public:
    Renderer(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, uint32_t framesInFlight, VkExtent2D swapChainExtent);
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void Render(uint32_t currentFrame); // Тепер приймає індекс кадру
    void SetViewProjection(const glm::mat4& view, const glm::mat4& projection);
    ImTextureID GetImGuiTextureId() const { return m_sceneTextureId; }

    // Геттери для доступу до властивостей з UI
    glm::vec3& GetCubePosition() { return m_cubePosition; }
    glm::vec3& GetCubeRotation() { return m_cubeRotation; }
    glm::vec3& GetCubeScale() { return m_cubeScale; }
    // -------------------------

    void OnResize(VkExtent2D newSize);

private:
    void createRenderPass();
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    void createFramebuffer();
    void createCubeBuffers();
    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();

    void updateUniformBuffer(uint32_t currentFrame);

    // Допоміжні функції
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    VkShaderModule createShaderModule(const std::vector<char>& code);
    static std::vector<char> ReadFile(const std::string& filename);

    VkDevice m_device;
    VkPhysicalDevice m_physicalDevice;
    VkCommandPool m_commandPool;
    VkQueue m_graphicsQueue;
    VkExtent2D m_sceneExtent;
    uint32_t m_framesInFlight;

    // ... ресурси для рендерингу в текстуру ...
    VkImage m_sceneImage = VK_NULL_HANDLE;
    VkDeviceMemory m_sceneImageMemory = VK_NULL_HANDLE;
    VkImageView m_sceneImageView = VK_NULL_HANDLE;
    VkSampler m_sceneSampler = VK_NULL_HANDLE;
    VkFramebuffer m_sceneFramebuffer = VK_NULL_HANDLE;
    VkRenderPass m_sceneRenderPass = VK_NULL_HANDLE;
    ImTextureID m_sceneTextureId = 0;
    
    // Графічний конвеєр
    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
    VkPipeline m_graphicsPipeline = VK_NULL_HANDLE;

    // Буфери для куба
    VkBuffer m_vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory m_vertexBufferMemory = VK_NULL_HANDLE;
    VkBuffer m_indexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory m_indexBufferMemory = VK_NULL_HANDLE;

    // Ресурси для Uniform Buffer
    VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;
    VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> m_descriptorSets;
    std::vector<VkBuffer> m_uniformBuffers;
    std::vector<VkDeviceMemory> m_uniformBuffersMemory;

    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    // Властивості трансформації нашого куба
    glm::vec3 m_cubePosition = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_cubeRotation = { 0.0f, 0.0f, 0.0f }; // В градусах
    glm::vec3 m_cubeScale = { 1.0f, 1.0f, 1.0f };
    // -------------------------
};