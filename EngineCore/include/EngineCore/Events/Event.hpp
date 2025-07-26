#pragma once

#include <string>
#include <functional>

/**
 * @enum EventType
 * @brief Defines all possible types of events within the engine.
 */
enum class EventType
{
    None = 0,
    WindowClose, WindowResize,
    KeyPressed, KeyReleased,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

/**
 * @def EVENT_CLASS_TYPE(type)
 * @brief A utility macro to implement the necessary type information methods for an event class.
 *
 * This macro reduces boilerplate code in derived event classes by automatically generating:
 * - GetStaticType(): A static method to get the event type without an object instance.
 * - GetEventType(): A virtual method that returns the event's type.
 * - GetName(): A virtual method that returns the event's name as a string.
 *
 * @param type The enum member from EventType corresponding to the class (e.g., WindowClose).
 */
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
                                virtual EventType GetEventType() const override { return GetStaticType(); }\
                                virtual const char* GetName() const override { return #type; }

/**
 * @class Event
 * @brief The abstract base class for all events in the engine.
 *
 * All specific event types (e.g., KeyPressedEvent) must inherit from this class.
 * It provides a common interface for querying event type and name, and for tracking
 * whether an event has been handled.
 */
class Event
{
public:
    virtual ~Event() = default;

    /**
     * @brief Gets the type of this specific event instance.
     * @return The event's type from the EventType enum.
     */
    virtual EventType GetEventType() const = 0;

    /**
     * @brief Gets the name of this event.
     * @return A C-style string representing the event's name.
     */
    virtual const char* GetName() const = 0;

    /// @brief A flag to indicate if the event has been handled by a listener.
    /// If true, subsequent listeners should typically ignore this event.
    bool Handled = false;
};

/**
 * @class EventDispatcher
 * @brief A utility to dispatch events to the correct handler functions based on their type.
 *
 * The dispatcher is created with a specific event instance. The `Dispatch` method
 * can then be called with a handler function. If the event's type matches the
 * template type of the `Dispatch` call, the handler is executed.
 */
class EventDispatcher
{
public:
    /**
     * @brief Constructs an EventDispatcher for a given event.
     * @param event The event to be dispatched.
     */
    EventDispatcher(Event& event) : m_Event(event) {}

    /**
     * @brief Attempts to dispatch the event to a given handler function.
     *
     * If the event's type matches the template parameter `T`, the event is cast
     * to `T&` and passed to the provided function `func`. The `Handled` flag of
     * the event is set to the return value of the function.
     *
     * @tparam T The specific event type to handle (e.g., KeyPressedEvent).
     * @tparam F The type of the handler function (usually a lambda).
     * @param func The function that will handle the event. It should take a `T&`
     *             as a parameter and return a bool (true if handled, false otherwise).
     * @return True if the event was dispatched to the function, false otherwise.
     */
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
