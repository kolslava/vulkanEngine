#include "EngineCore/Camera.hpp"
#include "EngineCore/Events/Event.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <algorithm> // for std::clamp

Camera::Camera(float fov, float aspectRatio, float nearClip, float farClip)
    : m_fov(fov), 
      m_aspectRatio(aspectRatio), 
      m_nearClip(nearClip), 
      m_farClip(farClip)
{
    recalculateMatrices();
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;

    // Calculate the camera's local axes. The 'front' vector points from the camera to the target.
    glm::vec3 front = glm::normalize(m_target - m_position);
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 0.0f, 1.0f))); // Using a fixed world up vector
    glm::vec3 up = glm::normalize(glm::cross(right, front));

    // This implements a "fly-cam" style movement where both the camera position
    // and its target point move together through the world.
    if (direction == CameraMovement::FORWARD)  { m_position += front * velocity; m_target += front * velocity; }
    if (direction == CameraMovement::BACKWARD) { m_position -= front * velocity; m_target -= front * velocity; }
    if (direction == CameraMovement::LEFT)     { m_position -= right * velocity; m_target -= right * velocity; }
    if (direction == CameraMovement::RIGHT)    { m_position += right * velocity; m_target += right * velocity; }
    if (direction == CameraMovement::UP)       { m_position += up * velocity;    m_target += up * velocity;    }
    if (direction == CameraMovement::DOWN)     { m_position -= up * velocity;    m_target -= up * velocity;    }
    
    // Recalculate the view matrix after changing the camera's position/target
    recalculateMatrices();
}

void Camera::ProcessMousePan(const glm::vec2& delta)
{
    // Calculate the camera's local axes
    glm::vec3 front = glm::normalize(m_target - m_position);
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 0.0f, 1.0f)));
    glm::vec3 up = glm::normalize(glm::cross(right, front));

    // Move the camera's position and target by the delta, scaled by pan speed
    m_target -= right * delta.x * m_panSpeed * m_distance;
    m_position -= right * delta.x * m_panSpeed * m_distance;
    m_target += up * delta.y * m_panSpeed * m_distance;
    m_position += up * delta.y * m_panSpeed * m_distance;

    recalculateMatrices();
}

void Camera::ProcessMouseRotate(const glm::vec2& delta)
{
    // Adjust yaw and pitch based on mouse delta and rotation speed
    m_yaw   -= delta.x * m_rotationSpeed;
    m_pitch += delta.y * m_rotationSpeed;

    // Clamp the pitch to avoid flipping the camera
    m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);

    recalculateMatrices();
}

void Camera::ProcessMouseZoom(float delta)
{
    // Adjust the distance from the target based on scroll delta and zoom speed
    m_distance -= delta * m_zoomSpeed;
    
    // Clamp the distance to a minimum value to prevent going through the target
    if (m_distance < 1.0f)
    {
        m_distance = 1.0f;
    }
    recalculateMatrices();
}

void Camera::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    // The camera only listens for mouse scroll events directly
    dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& e) { return this->OnMouseScrolled(e); });
}

void Camera::recalculateMatrices()
{
    // Calculate the camera's position based on spherical coordinates (yaw, pitch, distance) around the target
    float radYaw = glm::radians(m_yaw);
    float radPitch = glm::radians(m_pitch);

    m_position.x = m_target.x - cos(radYaw) * cos(radPitch) * m_distance;
    m_position.y = m_target.y - sin(radYaw) * cos(radPitch) * m_distance;
    m_position.z = m_target.z - sin(radPitch) * m_distance;
    
    // Create the view matrix looking from the new position towards the target
    // The up vector is fixed to (0, 0, 1) for a Z-up coordinate system
    m_viewMatrix = glm::lookAt(m_position, m_target, glm::vec3(0.0f, 0.0f, 1.0f));

    // Create the perspective projection matrix
    m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
    
    // Invert the Y-axis for Vulkan's coordinate system (clip space)
    m_projectionMatrix[1][1] *= -1; 
}

bool Camera::OnMouseScrolled(MouseScrolledEvent& e)
{
    // When a scroll event is received, process it as a zoom action
    ProcessMouseZoom(e.GetYOffset());
    return true; // Event handled
}
