/**\file windowEvents.hpp*/
#pragma once
#include "event.hpp"
#include <glm/glm.hpp>

/**
 * \class WindowCloseEvent
 * \brief Event which captures a window close request
 */

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() = default; //!< Default constructor
	[[nodiscard]] inline EventType getEventType() const noexcept override { return EventType::WindowClose; } //!< returns the EventType::WindowClose
};

/**
 * \class WindowResizeEvent
 * \brief Event which captures information about the new window size
 */

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(int32_t width, int32_t height) : m_width(width), m_height(height){} //!< Constructor with width and height
	[[nodiscard]] inline EventType getEventType() const noexcept override { return EventType::WindowResize; } //!< returns the EventType::WindowResize
	[[nodiscard]] inline int32_t getWidth() const noexcept { return m_width;} //!< Return window width
	[[nodiscard]] inline int32_t getHeight() const noexcept { return m_height;} //!< Returns window height
	[[nodiscard]] inline glm::ivec2 getSize() const noexcept { return { m_width, m_height }; } //!< Returns window size
					
private:
	int32_t m_width{ -1 }; //!< New window width
	int32_t m_height{ -1 }; //!< New window height
};

/**
 * \class WindowFocusEvent
 * \brief Event which captures a window gaining focus
 */
class WindowFocusEvent : public Event
{
public:
	WindowFocusEvent() = default; //!< Default constructor
	[[nodiscard]] inline EventType getEventType() const noexcept override { return EventType::WindowFocus; } //!< returns the EventType::WindowFocus
};

/**
 * \class WindowLostFocusEvent
 * \brief Event which captures a window losing focus
 */
class WindowLostFocusEvent : public Event
{
public:
	WindowLostFocusEvent() = default; //!< Default constructor
	[[nodiscard]] inline EventType getEventType() const noexcept override { return EventType::WindowLostFocus; } //!< returns the EventType::WindowLostFocus
};

/**
 * \class WindowMovedEvent
 * \brief Event which captures information about
 */

class WindowMovedEvent : public Event
{
public:
	WindowMovedEvent(int32_t x, int32_t y) : m_xPos(x), m_yPos(y) {} //!< Constructor with z and y values
	[[nodiscard]] inline EventType getEventType() const noexcept override { return EventType::WindowMoved; } //!< returns the EventType::WindowMoved
	[[nodiscard]] inline int32_t getXPos() const noexcept { return m_xPos; } //!< Returns window position in x
	[[nodiscard]] inline int32_t getYPos() const noexcept { return m_yPos; } //!< Returns window position in y
	[[nodiscard]] inline glm::ivec2 getPos() const noexcept { return { m_xPos, m_yPos }; } //!< Returns window position
private:
	int32_t m_xPos{ 0 }; //!< Position in x
	int32_t m_yPos{ 0 }; //!< Position in y
};

