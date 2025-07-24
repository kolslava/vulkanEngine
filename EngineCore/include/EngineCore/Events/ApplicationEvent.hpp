#pragma once

#include "Event.hpp"

class WindowCloseEvent : public Event
{
public:
    WindowCloseEvent() = default;
    EVENT_CLASS_TYPE(WindowClose)
};

class WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(unsigned int width, unsigned int height)
        : m_Width(width), m_Height(height) {}

    unsigned int GetWidth() const { return m_Width; }
    unsigned int GetHeight() const { return m_Height; }

    EVENT_CLASS_TYPE(WindowResize)
private:
    unsigned int m_Width, m_Height;
};