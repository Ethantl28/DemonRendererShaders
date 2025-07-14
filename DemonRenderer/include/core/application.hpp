/** \file application.hpp */

#pragma once
#include <glad/gl.h>

#include "core/log.hpp"
#include "core/timer.hpp"
#include "core/layer.hpp"
#include "windows/GLFWSystem.hpp"
#include "windows/GLFWWindowImpl.hpp"

#include "buffers/VAO.hpp"
#include "buffers/UBOmanager.hpp"
#include "buffers/FBO.hpp"
#include "rendering/material.hpp"
#include "rendering/camera.hpp"
#include "rendering/renderPass.hpp"
#include "rendering/renderer.hpp"



/** \class Application 
 *  \brief Provides an application with a window, OpenGL context, logger (spdlog) and a timer.
 */
class Application
{
public:
	explicit Application(const WindowProperties& winProps); //!< User defined constructor
	void run();	//!< Run the application
	glm::ivec2 getWindowSize() { return m_window.getSize(); }
protected:
	std::unique_ptr<Layer> m_layer; //!< Application layer
	GLFWWindowImpl m_window; //!< GLFW Window
private:
	void onUpdate(float timestep); //!< Update everything
	void onRender() const; //!< Do all drawing
	void onImGuiRender(); //!< Draw all ImGui wigdets
	void onClose(WindowCloseEvent& e); //!< Run when the window is closed
	void onResize(WindowResizeEvent& e); //!< Run when the window is resized
	void onKeyPressed(KeyPressedEvent& e); //!< Run when a key is press and the window is focused
	void onKeyReleased(KeyReleasedEvent& e); //!< Run when a key is released and the window is focused
	void onMousePress(MouseButtonPressedEvent& e); //!< Run when a mouse button is pressed
	void onMouseReleased(MouseButtonReleasedEvent& e); //!< Run when a Mouse Button is released
	void onMouseMoved(MouseMovedEvent& e); //!< Run when the mouse is moved
	void onMouseScrolled(MouseScrolledEvent& e); //!<< Run when the mouse wheel is scrolled
private:
	LogSystem m_logSystem;	//!< System which initialises the logger
	Timer m_timer;	//!< Basic std::chrono timer with seconds or milliseconds
	GLFWSystem m_windowsSystem; //!< System which initialises and terminated GLFW
	bool m_running{ true }; //!< Controls whether or not the application is running
	bool m_ImGuiOpen{ true }; //!< Boolean for IMGui window
};

// To be defined in users code
Application* startApplication(); //!< Function definition which provides an entry hook