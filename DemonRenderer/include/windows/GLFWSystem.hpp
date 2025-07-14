/* \file glfwSystem.hpp */
#pragma once

#include <GLFW/glfw3.h>
#include "windows/windowsSystem.hpp"
#include "core/log.hpp"

using GLFWSystemCStor = decltype(
	[]() {
		auto errorCode = glfwInit();
		if (!errorCode) spdlog::error("Cannot init GLFW: {0}", errorCode);
		else spdlog::info("GLFW initialised.");
	}
);

using GLFWSystemDStor = decltype(
	[]() {
		glfwTerminate();
	}
);

using GLFWSystem = WindowsSystem<GLFWSystemCStor, GLFWSystemDStor>;
