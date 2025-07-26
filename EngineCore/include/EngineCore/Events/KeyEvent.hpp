#pragma once
#include "Event.hpp"

/**
 * @class KeyEvent
 * @brief Base class for all keyboard-related events.
 *
 * This class is inherited by more specific key events and provides
 * access to the key code associated with the event.
 */
class KeyEvent : public Event
{
public:
    /**
     * @brief Gets the key code for the event.
     * @return The integer key code (e.g., from GLFW).
     */
    int GetKeyCode() const { return m_KeyCode; }

protected:
    /**
     * @brief Constructs a KeyEvent.
     * @param keycode The integer code of the key.
     */
    KeyEvent(int keycode) : m_KeyCode(keycode) {}

    /// @brief The key code associated with the event.
    int m_KeyCode;
};

/**
 * @class KeyPressedEvent
 * @brief An event that is triggered when a keyboard key is pressed.
 */
class KeyPressedEvent : public KeyEvent
{
public:
    /**
     * @brief Constructs a KeyPressedEvent.
     * @param keycode The integer code of the key that was pressed.
     */
    KeyPressedEvent(int keycode) : KeyEvent(keycode) {}
    EVENT_CLASS_TYPE(KeyPressed)
};

/**
 * @class KeyReleasedEvent
 * @brief An event that is triggered when a keyboard key is released.
 */
class KeyReleasedEvent : public KeyEvent
{
public:
    /**
     * @brief Constructs a KeyReleasedEvent.
     * @param keycode The integer code of the key that was released.
     */
    KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}
    EVENT_CLASS_TYPE(KeyReleased)
};
