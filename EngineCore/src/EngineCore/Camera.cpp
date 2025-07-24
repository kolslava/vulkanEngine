#include "EngineCore/Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm> // для std::clamp

Camera::Camera(float fov, float aspectRatio, float nearClip, float farClip)
    : m_fov(fov), m_aspectRatio(aspectRatio), m_nearClip(nearClip), m_farClip(farClip)
{
    recalculateMatrices();
}

void Camera::ProcessMousePan(const glm::vec2& delta)
{
    // Цю функціональність ми реалізуємо пізніше
}

void Camera::ProcessMouseRotate(const glm::vec2& delta)
{
    float yawSign = (m_pitch < 90.0f && m_pitch > -90.0f) ? 1.0f : -1.0f;
    m_yaw -= yawSign * delta.x * 0.5f; // Чутливість обертання
    m_pitch += delta.y * 0.5f;

    // Обмежуємо нахил, щоб уникнути перевертання камери
    m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);

    recalculateMatrices();
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = 2.5f * deltaTime; // Швидкість руху

    // Розраховуємо напрямки на основі поточної орієнтації
    glm::vec3 front = glm::normalize(m_target - m_position);
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 0.0f, 1.0f)));
    glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);

    if (direction == CameraMovement::FORWARD)
    {
        m_position += front * velocity;
        m_target += front * velocity;
    }
    if (direction == CameraMovement::BACKWARD)
    {
        m_position -= front * velocity;
        m_target -= front * velocity;
    }
    if (direction == CameraMovement::LEFT)
    {
        m_position -= right * velocity;
        m_target -= right * velocity;
    }
    if (direction == CameraMovement::RIGHT)
    {
        m_position += right * velocity;
        m_target += right * velocity;
    }
    if (direction == CameraMovement::UP)
    {
        m_position += up * velocity;
        m_target += up * velocity;
    }
    if (direction == CameraMovement::DOWN)
    {
        m_position -= up * velocity;
        m_target -= up * velocity;
    }
    
    // Перераховуємо матрицю виду після зміни позиції
    recalculateMatrices();
}

void Camera::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& e) { return this->OnMouseScrolled(e); });
}

bool Camera::OnMouseScrolled(MouseScrolledEvent& e)
{
    ProcessMouseZoom(e.GetYOffset());
    return true;
}

void Camera::ProcessMouseZoom(float delta)
{
    m_distance -= delta * 0.5f; // Чутливість масштабування
    if (m_distance < 1.0f)
    {
        m_distance = 1.0f;
    }
    recalculateMatrices();
}

void Camera::recalculateMatrices()
{
    // Розрахунок позиції камери на основі кутів та відстані від цілі
    float radYaw = glm::radians(m_yaw);
    float radPitch = glm::radians(m_pitch);

    m_position.x = m_target.x - cos(radYaw) * cos(radPitch) * m_distance;
    m_position.y = m_target.y - sin(radYaw) * cos(radPitch) * m_distance;
    m_position.z = m_target.z - sin(radPitch) * m_distance;
    
    // Створюємо матрицю виду, що дивиться з нової позиції на ціль
    m_viewMatrix = glm::lookAt(m_position, m_target, glm::vec3(0.0f, 0.0f, 1.0f));

    // Створюємо матрицю проекції
    m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
    m_projectionMatrix[1][1] *= -1; // Інвертуємо Y-вісь для Vulkan
}