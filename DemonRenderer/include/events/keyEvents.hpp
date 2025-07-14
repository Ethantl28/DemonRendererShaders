/** \file keyEvents.hpp*/
#pragma once
#include "events/event.hpp"

/**
 *\class KeyEvent
 * \brief Base class from which all key event classes are derived.
 */
class KeyEvent : public Event
{
protected:
	KeyEvent(int32_t keycode) : m_keyCode{ keycode } {} //!< Constructor with values
    int32_t m_keyCode{ 0 }; //!< Library specific code for the key
public:
	[[nodiscard]] inline int32_t getKeyCode() const { return m_keyCode; } //!< Returns the library specific key code
};

/**
 * \class KeyPressedEvent
 * \brief Event which captures information about a key being pressed
 */
class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(int32_t keycode, int32_t repeatCount) : KeyEvent{ keycode }, m_repeatCount{ repeatCount } {} //!< constructor that takes in the keycode and repeat count 
	[[nodiscard]] inline int32_t getRepeatCount() const { return m_repeatCount; } //!< gets if the key pressed has been repeated 
	[[nodiscard]] inline EventType getEventType() const noexcept override { return EventType::KeyPressed; } //!< returns the EventType::KeyPressed 
protected:
	int32_t m_repeatCount; //!< Hold repreat count.  This will be postive on repeat. Will only hold actual count for some window implementations.
};

/**
 * \class KeyTypedEvent
 * \brief Event which captures information about a key being typed.  Keys being typed are rare occurances and require a typing widget of some kind.
 */
class KeyTypedEvent : public KeyEvent
{
public:
	explicit KeyTypedEvent(int32_t keycode) : KeyEvent{ keycode } {} //!< constructor that takes in keycode 
	[[nodiscard]] inline EventType getEventType() const noexcept override { return EventType::KeyTyped; } //!< returns the EventType::KeyTyped
};
/**
 * \class KeyReleasedEvent
 * \brief Event which captures information about a key being released
 */
class KeyReleasedEvent : public KeyEvent
{
public:
    explicit KeyReleasedEvent(int32_t keycode) : KeyEvent{ keycode } {} //!< constructor for key released that takes in keycode
	[[nodiscard]] inline EventType getEventType() const noexcept override { return EventType::KeyReleased; } //!< returns the EventType::KeyRelease
};
