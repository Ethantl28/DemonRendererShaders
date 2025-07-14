#pragma once
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "windows/window.hpp"
#include "windows/GLFW_GL_GC.hpp"


using GLFWWinDeleter = decltype(
	[](GLFWwindow* window) {
		// Probably temp to be moved
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		// End temp
		glfwDestroyWindow(window);
	}
);

/** \class GLFWWindowImpl
 * \brief A GLFW window implementation.
 */
class GLFWWindowImpl : public IWindow<GLFWwindow, GLFWWinDeleter, ModernGLFW_GL_GC>
{
public:
	GLFWWindowImpl() = default; //!< Default constructor. Does not create a window but remains in a valid state.
	void doOpen(const WindowProperties& properties) override; //!< Open a window with properties
	void doOnUpdate(float timestep) override; //!< Update the window
	void doSetVSync(bool VSync) override; //!< Set the VSync
	[[nodiscard]] virtual bool doIsKeyPressed(int32_t keyCode) const override; //!< Function acts on is key pressed
	[[nodiscard]] virtual bool doIsMouseButtonPressed(int32_t mouseButton) const override; //!< Function acts on is mouse button pressed
	[[nodiscard]] virtual glm::vec2 doGetMousePosition() const override; //!< Function acts on get mouse position
	[[nodiscard]] virtual glm::ivec2 doGetSize() const override; //!< Virtual function acts on get the window size
};

