#pragma once
#include "Event.hpp"

/**
 * @class MouseMovedEvent
 * @brief An event that is triggered when the mouse cursor is moved.
 */
class MouseMovedEvent : public Event
{
public:
    /**
     * @brief Constructs a MouseMovedEvent.
     * @param x The new x-coordinate of the mouse cursor.
     * @param y The new y-coordinate of the mouse cursor.
     */
    MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {}

    /**
     * @brief Gets the new x-coordinate of the mouse.
     * @return The x-coordinate.
     */
    float GetX() const { return m_MouseX; }

    /**
     * @brief Gets the new y-coordinate of the mouse.
     * @return The y-coordinate.
     */
    float GetY() const { return m_MouseY; }

    EVENT_CLASS_TYPE(MouseMoved)
private:
    float m_MouseX, m_MouseY;
};

/**
 * @class MouseScrolledEvent
 * @brief An event that is triggered when the mouse wheel is scrolled.
 */
class MouseScrolledEvent : public Event
{
public:
    /**
     * @brief Constructs a MouseScrolledEvent.
     * @param xOffset The scroll offset on the x-axis.
     * @param yOffset The scroll offset on the y-axis.
     */
    MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

    /**
     * @brief Gets the horizontal scroll offset.
     * @return The x-offset.
     */
    float GetXOffset() const { return m_XOffset; }

    /**
     * @brief Gets the vertical scroll offset.
     * @return The y-offset.
     */
    float GetYOffset() const { return m_YOffset; }

    EVENT_CLASS_TYPE(MouseScrolled)
private:
    float m_XOffset, m_YOffset;
};

/**
 * @class MouseButtonEvent
 * @brief Base class for all mouse button-related events.
 */
class MouseButtonEvent : public Event
{
public:
    /**
     * @brief Gets the mouse button code for the event.
     * @return The integer button code (e.g., from GLFW).
     */
    int GetMouseButton() const { return m_Button; }
    
protected:
    /**
     * @brief Constructs a MouseButtonEvent.
     * @param button The integer code of the mouse button.
     */
    MouseButtonEvent(int button) : m_Button(button) {}
    
    /// @brief The button code associated with the event.
    int m_Button;
};

/**
 * @class MouseButtonPressedEvent
 * @brief An event that is triggered when a mouse button is pressed.
 */
class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    /**
     * @brief Constructs a MouseButtonPressedEvent.
     * @param button The integer code of the button that was pressed.
     */
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}
    EVENT_CLASS_TYPE(MouseButtonPressed)
};

/**
 * @class MouseButtonReleasedEvent
 * @brief An event that is triggered when a mouse button is released.
 */
class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    /**
     * @brief Constructs a MouseButtonReleasedEvent.
     * @param button The integer code of the button that was released.
     */
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}
    EVENT_CLASS_TYPE(MouseButtonReleased)
};
