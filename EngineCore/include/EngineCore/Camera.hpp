#pragma once

#include <glm/glm.hpp>

#include "EngineCore/Events/Event.hpp"
#include "EngineCore/Events/MouseEvent.hpp"

// Додайте цей enum перед оголошенням класу Camera
enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera
{
public:
    Camera(float fov, float aspectRatio, float nearClip, float farClip);

    // Оновлення на основі вводу миші
    void ProcessMousePan(const glm::vec2& delta);
    void ProcessMouseRotate(const glm::vec2& delta);
    void ProcessMouseZoom(float delta);
    
    // Геттери для матриць
    const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
    const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
    
    void OnEvent(Event& e);

    void ProcessKeyboard(CameraMovement direction, float deltaTime);

private:
    void recalculateMatrices();

    bool OnMouseScrolled(MouseScrolledEvent& e);

    // Матриці
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    // Параметри проекції
    float m_fov;
    float m_aspectRatio;
    float m_nearClip;
    float m_farClip;

    // Параметри позиції та орієнтації
    glm::vec3 m_position;
    glm::vec3 m_target{0.0f, 0.0f, 0.0f};
    float m_distance = 5.0f;
    float m_yaw = -90.0f; // Обертання навколо осі Z
    float m_pitch = 45.0f;   // Нахил
};