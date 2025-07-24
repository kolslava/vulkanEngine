#pragma once

#include <string>
#include <functional>

enum class EventType
{
    None = 0,
    WindowClose, WindowResize,
    KeyPressed, KeyReleased,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

// Макрос для спрощення реалізації віртуальних методів
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
                                virtual EventType GetEventType() const override { return GetStaticType(); }\
                                virtual const char* GetName() const override { return #type; }

class Event
{
public:
    virtual ~Event() = default;
    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;

    bool Handled = false;
};

// Диспетчер, що викликає функцію-обробник для конкретного типу події
class EventDispatcher
{
public:
    EventDispatcher(Event& event) : m_Event(event) {}

    template<typename T, typename F>
    bool Dispatch(const F& func)
    {
        if (m_Event.GetEventType() == T::GetStaticType())
        {
            m_Event.Handled = func(static_cast<T&>(m_Event));
            return true;
        }
        return false;
    }
private:
    Event& m_Event;
};