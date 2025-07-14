/* \file GLFW_GL_GC.hpp*/
#pragma once
#include "windows/graphicsContext.hpp"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

/**	\class GLFW_GL_Init
 *	\brief Provides a function for initialising a GLFW windows with an OpenGL context.
 *	Could be replaced with lambdas however, this doesn't make sense due to includes and length of implementation
 */
class GLFW_GL_Init
{
public:
	/* Function which initialises an OpenGL context for a GLFW window.
	* 
	* @param nativeWindow The GLFWwindow (raw pointer to) which will have graphics context
	* @param nativeWindow The GLFWwindow (raw pointer to) which will have graphics context
	*/
	void operator()(GLFWwindow* nativeWindow, bool hostingImGui);
};

using GLFW_GL_Swapbuffers = decltype([](GLFWwindow* nativeWindow, bool hostingImGui){
	if(hostingImGui) ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(nativeWindow); 
});


using ModernGLFW_GL_GC = GraphicsContext<GLFWwindow, GLFW_GL_Init, GLFW_GL_Swapbuffers>;
