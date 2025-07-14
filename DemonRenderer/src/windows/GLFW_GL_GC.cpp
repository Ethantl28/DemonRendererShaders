/* \file GLFW_GL_GC.cpp*/
#include <glad/gl.h>
#include "windows/GLFW_GL_GC.hpp"
#include "core/log.hpp"

void GLFW_GL_Init::operator()(GLFWwindow* nativeWindow, bool hostingImGui)
{
	glfwMakeContextCurrent(nativeWindow);
	auto result = gladLoadGL(glfwGetProcAddress);
	if (!result) spdlog::error("Could not create OpenGL context for current GLFW window: {0}", result);
	else spdlog::info("OpenGL context successfully created for current GLFW window");

	if(hostingImGui){ 
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows


		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(nativeWindow, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
		ImGui_ImplOpenGL3_Init();
	}

	// Enable OpenGL debug with a callback through spdlog
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(
		[](
			GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam
			)
		{
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:
				spdlog::error(message);
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				spdlog::warn(message);
				break;
			case GL_DEBUG_SEVERITY_LOW:
				spdlog::info(message);
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				spdlog::trace(message);
				break;
			}
		}
	, nullptr);
}