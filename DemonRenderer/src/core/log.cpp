/** \file log.cpp */

#include "core/log.hpp"

LogSystem::LogSystem() noexcept
{
    // Logging set up, set pattern and appropriate debug level
    spdlog::set_pattern("%^[%T]: %v%$");        // Set logger pattern to something simple
#ifndef NDEBUG
    spdlog::set_level(spdlog::level::debug);    // Set level to debug i.e. will log everything
#else
    spdlog::set_level(spdlog::level::off);      // Turn off logging for release builds
#endif
}

std::ostream& operator<<(std::ostream& os, const EventType& ev)
{
	switch (ev) {
		case EventType::None:
			os << "EventType::None";
			break;
		case EventType::KeyPressed:
			os << "EventType::KeyPressed";
			break;
		case EventType::KeyReleased:
			os << "EventType::KeyReleased";
			break;
		case EventType::KeyTyped:
			os << "EventType::KeyTyped";
		case EventType::MouseButtonPressed:
			os << "EventType::MouseButtonPressed";
			break;
		case EventType::MouseButtonReleased:
			os << "EventType::MouseButtonReleased";
			break;
		case EventType::MouseMoved:
			os << "EventType::MouseMoved";
			break;
		case EventType::MouseScrolled:
			os << "EventType::MouseScrolled";
			break;
		case EventType::WindowClose:
			os << "EventType::WindowClose";
			break;
		case EventType::WindowMoved:
			os << "EventType::WindowMoved";
			break;
		case EventType::WindowResize:
			os << "EventType::WindowResize";
			break;
		case EventType::WindowFocus:
			os << "EventType::WindowFocus";
			break;
		case EventType::WindowLostFocus:
			os << "EventType::WindowLostFocus";
			break;
		default:
			os << "Unknown EventType";
	}
	return os;
}


std::ostream& operator<<(std::ostream& os, const glm::ivec2& iv2)
{
	os << "(" << iv2.x << ", " << iv2.y << ")";
	return os;
}