/* \file GLFWWindowImplImpl.cpp*/
#include "windows/GLFWWindowImpl.hpp"
#include "core/log.hpp"


void GLFWWindowImpl::doOpen(const WindowProperties& properties)
{
	if (m_isFullScreen)
	{
		spdlog::error("Fullscreen not yet implemented");
	}
	else
	{
		if (!properties.isResizable) glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		m_nativeWindow = std::unique_ptr<GLFWwindow, GLFWWinDeleter>(glfwCreateWindow(properties.width, properties.height, properties.title, nullptr, nullptr));
	}

	setVSync(m_isVSync);

	glfwSetWindowUserPointer(m_nativeWindow.get(), static_cast<void*>(&handler));

	glfwSetWindowCloseCallback(m_nativeWindow.get(),
		[](GLFWwindow* window)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			auto& onClose = handler->onWinClose;

			WindowCloseEvent e;
			onClose(e);
		}
	);

	glfwSetWindowSizeCallback(m_nativeWindow.get(),
		[](GLFWwindow* window, int newWidth, int newHeight)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			auto& onResize = handler->onWinResized;

			WindowResizeEvent e(newWidth, newHeight);
			onResize(e);
		}
	);

	glfwSetWindowPosCallback(m_nativeWindow.get(),
		[](GLFWwindow* window, int posX, int posY)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			auto& onMoved = handler->onWinMoved;

			WindowMovedEvent e(posX, posY);
			onMoved(e);
		}
	);

	glfwSetWindowFocusCallback(m_nativeWindow.get(),
		[](GLFWwindow* window, int state)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));

			if (state == GLFW_TRUE) // Gained focus
			{
				auto& onFocused = handler->onWinFocused;

				WindowFocusEvent e;
				onFocused(e);
			}
			else if (state == GLFW_FALSE) // Lost focus
			{
				auto& onLostFocused = handler->onWinLostFocus;

				WindowLostFocusEvent e;
				onLostFocused(e);
			}
		}
	);

	glfwSetKeyCallback(m_nativeWindow.get(),
		[](GLFWwindow* window, int keyCode, int scancode, int action, int mods)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));

			if (action == GLFW_PRESS)
			{
				auto& onKeyPress = handler->onKeyPress;

				KeyPressedEvent e(keyCode, 0);
				onKeyPress(e);
			}
			else if (action == GLFW_REPEAT)
			{
				auto& onKeyPress = handler->onKeyPress;

				KeyPressedEvent e(keyCode, 1);
				onKeyPress(e);
			}
			else if (action == GLFW_RELEASE)
			{
				auto& onKeyRelease = handler->onKeyRelease;

				KeyReleasedEvent e(keyCode);
				onKeyRelease(e);
			}
		}
	);

	glfwSetCursorPosCallback(m_nativeWindow.get(),
		[](GLFWwindow* window, double mouseX, double mouseY)
		{
			MouseMovedEvent e(static_cast<float>(mouseX), static_cast<float>(mouseY));

			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			auto& onMouseMovedFunction = handler->onMouseMove;

			onMouseMovedFunction(e);
		}
	);

	glfwSetMouseButtonCallback(m_nativeWindow.get(),
		[](GLFWwindow* window, int button, int action, int mods)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			if (action == GLFW_PRESS)
			{
				MouseButtonPressedEvent e(button);

				auto& onMouseButtonPressedFunction = handler->onMousePress;

				onMouseButtonPressedFunction(e);
			}
			else if (action == GLFW_RELEASE)
			{
				MouseButtonReleasedEvent e(button);

				auto& onMouseButtonReleasedFunction = handler->onMouseRelease;

				onMouseButtonReleasedFunction(e);
			}
		}
	);

	glfwSetScrollCallback(m_nativeWindow.get(),
		[](GLFWwindow* window, double xOffset, double yOffset)
		{
			MouseScrolledEvent e(static_cast<float>(xOffset), static_cast<float>(yOffset));
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));

			auto& onMouseScrolledFunction = handler->onMouseScrolled;
			onMouseScrolledFunction(e);
		}
	);

	m_graphicsContext.init(m_nativeWindow.get(), isHostingImGui());
}

void GLFWWindowImpl::doOnUpdate(float timestep)
{
	glfwPollEvents();

	if (isHostingImGui()) {
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	m_graphicsContext.swapBuffers(m_nativeWindow.get(), isHostingImGui());
}

void GLFWWindowImpl::doSetVSync(bool VSync)
{
	if (m_isVSync) { glfwSwapInterval(1); }
	else { glfwSwapInterval(0); }
	
}

bool GLFWWindowImpl::doIsKeyPressed(int32_t keyCode) const
{
	auto answer = glfwGetKey(m_nativeWindow.get(), keyCode);
	return answer == GLFW_PRESS || answer == GLFW_REPEAT;
}

bool GLFWWindowImpl::doIsMouseButtonPressed(int32_t mouseButton) const
{
	auto answer = glfwGetMouseButton(m_nativeWindow.get(), mouseButton);
	return answer == GLFW_PRESS;
}

glm::vec2 GLFWWindowImpl::doGetMousePosition() const
{
	double x, y;
	glfwGetCursorPos(m_nativeWindow.get(), &x, &y);
	return glm::vec2(static_cast<float>(x), static_cast<float>(y));
}

glm::ivec2 GLFWWindowImpl::doGetSize() const
{
	int width{ 0 };
	int height{ 0 };
	glfwGetWindowSize(m_nativeWindow.get(), &width, &height);

	return glm::ivec2(width, height);	
}