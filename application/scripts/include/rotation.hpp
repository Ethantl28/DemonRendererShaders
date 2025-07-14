#pragma once

#include "DemonRenderer.hpp"
#include <iostream>

class RotationScript : public Script
{
public:
	RotationScript(Actor& actor, const glm::vec3& rotSpeed, uint32_t pauseKeyMapping) : 
		Script(actor),
		m_rotSpeed(rotSpeed),
		m_pauseKey(pauseKeyMapping)
	{}
	virtual void onUpdate(float timestep) override;
	virtual void onKeyPress(KeyPressedEvent& e) override;
	virtual void onKeyRelease(KeyReleasedEvent& e) override {};
private:
	glm::vec3 m_rotSpeed{0.f, 0.f, 0.f };
	uint32_t m_pauseKey{ GLFW_KEY_P };
	bool m_paused{ false };
};


