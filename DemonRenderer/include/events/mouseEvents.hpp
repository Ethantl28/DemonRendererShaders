/** \file keyEvents.hpp*/
#pragma once
#include "event.hpp"
#include <glm/glm.hpp>


/**
 * \class MouseMovedEvent
 * \brief Event which captures information about the mouse being moved
 */
class MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(float x, float y) : m_mouseX{ x }, m_mouseY{ y } {} //!< Constructor with values
    [[nodiscard]] inline EventType getEventType() const noexcept override { return EventType::MouseMoved; } //!< returns the EventType::MouseMoved
    [[nodiscard]] inline float getX() const noexcept { return m_mouseX; } //!< Returns the distance moved in x
    [[nodiscard]] inline float getY() const noexcept { return m_mouseY; } //!< Returns the distance moved in y
    [[nodiscard]] inline glm::vec2 getPos() const noexcept { return glm::vec2(m_mouseX, m_mouseY); } //!< Returns the distance moved
private:
    float m_mouseX{ 0 }; //!< Distance moved in x
    float m_mouseY{ 0 }; //!< Distance moved in y
};

/**
 * \class MouseScrolledEvent
 * \brief Event which captures information about the mouse wheel being scrolled.  The z offset will always be zero unless a track ball is used.
 */

class MouseScrolledEvent : public Event
{
public:
    MouseScrolledEvent(float x, float y) : m_offsetX(x), m_offsetY(y) {} //!< Constructor with values
    [[nodiscard]] inline EventType getEventType() const noexcept override { return EventType::MouseScrolled; } //!< returns the EventType::MouseScrolled
    [[nodiscard]] inline float getX() const noexcept { return m_offsetX; } //!< Returns the scroll amount in x
    [[nodiscard]] inline float getY() const noexcept { return m_offsetY; } //!< Returns the scroll amount in x
    [[nodiscard]] inline glm::vec2 getPos() const noexcept { return glm::vec2(m_offsetX, m_offsetY); } //!< Returns the scroll amount
private:
    float m_offsetX{ 0 }; //!< Scroll amount in x
    float m_offsetY{ 0 }; //!< Scroll amount in y
};

/**
 * \class MouseButtonPressedEvent
 * \brief Event which captures information about which mouse button was pressed
 */

class MouseButtonPressedEvent : public Event
{
public:
    explicit MouseButtonPressedEvent(int32_t buttoncode) : m_buttonCode(buttoncode) {} //!< Constructor with values
    [[nodiscard]] inline EventType getEventType() const noexcept override { return EventType::MouseButtonPressed; } //!< returns the EventType::MouseButtonPressed
    [[nodiscard]] inline int32_t getButton() const noexcept { return m_buttonCode; } //!< Returns the button code
private:
    int32_t m_buttonCode{ -1 }; //!< Library specific code of the button pressed
};

/**
 * \class MouseButtonReleasedEvent
 * \brief Event which captures information about which mouse button was released
 */

class MouseButtonReleasedEvent : public Event
{
public:
    explicit MouseButtonReleasedEvent(int32_t buttoncode) : m_buttonCode(buttoncode) {} //!< Constructor with values
    [[nodiscard]] inline EventType getEventType() const noexcept override { return EventType::MouseButtonReleased; } //!< returns the EventType::MouseButtonReleased
    [[nodiscard]] inline int32_t getButton() const noexcept { return m_buttonCode; } //!< Returns the button code
private:
    int32_t m_buttonCode{ -1 }; //!< Library specific code of the button released
};


