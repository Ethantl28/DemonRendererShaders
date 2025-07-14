/** \file eventHandler.hpp*/
#pragma once
#include <functional>
#include "events/events.hpp"


/**
 * \struct EventHandler
 * \brief Struct which gold all callback functions for a window
 */
struct EventHandler
{
	std::function<void(WindowCloseEvent& e)> onWinClose{ [](WindowCloseEvent& e) {} }; //!< Called on window closing
	std::function<void(WindowResizeEvent& e)> onWinResized{ [](WindowResizeEvent& e) {} }; //!< Called on window resize
	std::function<void(WindowFocusEvent& e)> onWinFocused{ [](WindowFocusEvent& e) {} }; //!< Called on window focus
	std::function<void(WindowLostFocusEvent& e)> onWinLostFocus{ [](WindowLostFocusEvent& e) {} }; //!< Called on window lost focus
	std::function<void(WindowMovedEvent& e)> onWinMoved{ [](WindowMovedEvent& e) {} }; //!< Called on window moved
	std::function<void(KeyPressedEvent& e)> onKeyPress{ [](KeyPressedEvent& e) {} }; //!< Called on key press
	std::function<void(KeyReleasedEvent& e)> onKeyRelease{ [](KeyReleasedEvent& e) {} }; //!< Called on key release
	std::function<void(KeyTypedEvent& e)> onKeyTyped{ [](KeyTypedEvent& e) {} }; //!< Called on key type
	std::function<void(MouseButtonPressedEvent& e)> onMousePress{ [](MouseButtonPressedEvent& e) {} }; //!< Called on mouse button press
	std::function<void(MouseButtonReleasedEvent& e)> onMouseRelease{ [](MouseButtonReleasedEvent& e) {} }; //!< Called on mouse release
	std::function<void(MouseMovedEvent& e)> onMouseMove{ [](MouseMovedEvent& e) {} }; //!< Called on mouse move
	std::function<void(MouseScrolledEvent& e)> onMouseScrolled{ [](MouseScrolledEvent& e) {} }; //!< Called on mouse scroll wheel
};


