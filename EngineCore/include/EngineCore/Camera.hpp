#pragma once

#include "EngineCore/Events/Event.hpp"
#include "EngineCore/Events/MouseEvent.hpp"

#include <glm/glm.hpp>

/**
 * @enum CameraMovement
 * @brief Defines possible camera movement directions.
 */
enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

/**
 * @class Camera
 * @brief Represents a 3D camera with orbit, pan, and zoom controls.
 *
 * This class manages the view and projection matrices required for 3D rendering.
 * It can be controlled via mouse and keyboard input to navigate the scene.
 */
class Camera
{
public:
    /**
     * @brief Constructs a Camera object.
     * @param fov Field of View (in degrees).
     * @param aspectRatio The aspect ratio of the viewport.
     * @param nearClip The near clipping plane.
     * @param farClip The far clipping plane.
     */
    Camera(float fov, float aspectRatio, float nearClip, float farClip);

    /**
     * @brief Processes keyboard input to move the camera.
     * @param direction The direction of movement.
     * @param deltaTime The time elapsed since the last frame.
     */
    void ProcessKeyboard(CameraMovement direction, float deltaTime);

    /**
     * @brief Pans the camera based on mouse movement.
     * @param delta The change in mouse position.
     */
    void ProcessMousePan(const glm::vec2& delta);

    /**
     * @brief Rotates (orbits) the camera around the target point based on mouse movement.
     * @param delta The change in mouse position.
     */
    void ProcessMouseRotate(const glm::vec2& delta);

    /**
     * @brief Zooms the camera in or out based on mouse scroll wheel movement.
     * @param delta The scroll wheel offset.
     */
    void ProcessMouseZoom(float delta);
    
    /**
     * @brief Handles incoming events, specifically mouse scroll events.
     * @param e The event to be processed.
     */
    void OnEvent(Event& e);

    /**
     * @brief Gets the camera's view matrix.
     * @return A constant reference to the view matrix.
     */
    const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }

    /**
     * @brief Gets the camera's projection matrix.
     * @return A constant reference to the projection matrix.
     */
    const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }

private:
    /**
     * @brief Recalculates the view and projection matrices based on current camera parameters.
     */
    void recalculateMatrices();

    /**
     * @brief Handles the mouse scrolled event.
     * @param e The mouse scrolled event.
     * @return True if the event was handled, false otherwise.
     */
    bool OnMouseScrolled(MouseScrolledEvent& e);

    // --- Matrices ---
    glm::mat4 m_viewMatrix;         ///< The camera's view matrix.
    glm::mat4 m_projectionMatrix;   ///< The camera's projection matrix.

    // --- Projection Parameters ---
    float m_fov;                    ///< Field of View (in degrees).
    float m_aspectRatio;            ///< Viewport aspect ratio.
    float m_nearClip;               ///< Near clipping plane.
    float m_farClip;                ///< Far clipping plane.

    // --- Camera Transform ---
    glm::vec3 m_position;           ///< The position of the camera in world space.
    glm::vec3 m_target{0.0f, 0.0f, 0.0f}; ///< The point the camera is looking at.
    float m_distance = 5.0f;        ///< The distance from the camera to its target.
    float m_yaw = -90.0f;           ///< Yaw angle (rotation around the Y-axis).
    float m_pitch = 45.0f;          ///< Pitch angle (rotation around the X-axis).

    // --- Control Sensitivity ---
    float m_movementSpeed = 5.0f;   ///< Speed of camera movement with keyboard.
    float m_panSpeed = 0.005f;      ///< Speed of camera panning with mouse.
    float m_rotationSpeed = 0.4f;   ///< Speed of camera rotation with mouse.
    float m_zoomSpeed = 0.5f;       ///< Speed of camera zooming with mouse scroll.
};
