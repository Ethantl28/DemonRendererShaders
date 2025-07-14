/* \file layer.hpp */
#pragma once

#include "events/events.hpp"
#include "windows/GLFWWindowImpl.hpp"

/**
\class Layer
\brief A convient abstraction of rendering and events into a layer which can be placed in the application.
Intended for use as a base class. onRender and onUpdate must be implemented, thats why they are pure virtual functions.
Event handling is optional, it is possible to just use the event handler some user input and other action may not be required.
ImGUI renderering is also optional.
*/
class Layer
{
public:
	Layer(GLFWWindowImpl& win) : m_winRef(win) {}; //!< Constructor

	virtual void onRender() const = 0; //!< Runs when layer is renderered
	virtual void onImGUIRender() = 0; //!< Runs when layer is renderered
	virtual void onUpdate(float timestep) = 0;//!< Runs every frame

	virtual void onClose(WindowCloseEvent& e) {}; //!< Run when the window is closed
	virtual void onResize(WindowResizeEvent& e) {}; //!< Run when the window is resized
	virtual void onKeyPressed(KeyPressedEvent& e) {}; //!< Run when a key is press and the window is focused
	virtual void onKeyReleased(KeyReleasedEvent& e) {}; //!< Run when a key is released and the window is focused
	virtual void onMousePress(MouseButtonPressedEvent& e) {}; //!< Run when a mouse button is pressed
	virtual void onMouseReleased(MouseButtonReleasedEvent& e) {}; //!< Run when a Mouse Button is released
	virtual void onMouseMoved(MouseMovedEvent& e) {}; //!< Run when the mouse is moved
	virtual void onMouseScrolled(MouseScrolledEvent& e) {}; //!<< Run when the mouse wheel is scrolled

protected:
	GLFWWindowImpl& m_winRef; //!< Reference to the window where this layer is rendered
};
