/** \file event.hpp*/
#pragma once
#include <inttypes.h>

/**
 * \enum EventType
 * \brief Different types of event which can be captured and cause a callback to run.
 */
enum class EventType
{
	None,
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

/**
 * \class Event
 * \brief Base class from which event classes are derived. Not to be used polymorphically.
 */
class Event
{
public:
	Event() noexcept = default; //!< Default constructor
	explicit Event(EventType type) noexcept : m_eventType(type) {} //!< Constructor which sets the event type
	virtual EventType getEventType() const = 0; //!< gets the type of event 
	[[nodiscard]] inline bool handled() const noexcept { return m_handled; }; //!< has the event been handled 
	inline void handle() noexcept { m_handled = true; } //!< Handle the event
protected:
	EventType m_eventType{EventType::None}; //!< Holds event type
	bool m_handled{false}; //!< Holds whether or not the event has been handled
};



