#include "scripts/include/rotation.hpp"



void RotationScript::onUpdate(float timestep)
{

	if (!m_paused) {
		m_actor.rotation *= glm::quat(m_rotSpeed * timestep);
		m_actor.recalc();
	}
}

void RotationScript::onKeyPress(KeyPressedEvent& e)
{
	if (e.getKeyCode() == m_pauseKey) m_paused = !m_paused;
}