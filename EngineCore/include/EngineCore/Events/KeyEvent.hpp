#pragma once
#include "Event.hpp"

class KeyEvent : public Event
{
public:
    int GetKeyCode() const { return m_KeyCode; }
protected:
    KeyEvent(int keycode) : m_KeyCode(keycode) {}
    int m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(int keycode) : KeyEvent(keycode) {}
    EVENT_CLASS_TYPE(KeyPressed)
};

class KeyReleasedEvent : public KeyEvent
{
public:
    KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}
    EVENT_CLASS_TYPE(KeyReleased)
};