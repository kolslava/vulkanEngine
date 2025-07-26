#pragma once

#include "Event.hpp"

/**
 * @class WindowCloseEvent
 * @brief An event that is triggered when the user attempts to close the window.
 *
 * This event does not carry any data. Its presence signals the application
 * that a close request has been made.
 */
class WindowCloseEvent : public Event
{
public:
    WindowCloseEvent() = default;
    EVENT_CLASS_TYPE(WindowClose)
};

/**
 * @class WindowResizeEvent
 * @brief An event that is triggered when the application window is resized.
 */
class WindowResizeEvent : public Event
{
public:
    /**
     * @brief Constructs a WindowResizeEvent.
     * @param width The new width of the window.
     * @param height The new height of the window.
     */
    WindowResizeEvent(unsigned int width, unsigned int height)
        : m_Width(width), m_Height(height) {}

    /**
     * @brief Gets the new width of the window.
     * @return The new width in pixels.
     */
    unsigned int GetWidth() const { return m_Width; }

    /**
     * @brief Gets the new height of the window.
     * @return The new height in pixels.
     */
    unsigned int GetHeight() const { return m_Height; }

    EVENT_CLASS_TYPE(WindowResize)
private:
    unsigned int m_Width, m_Height;
};
