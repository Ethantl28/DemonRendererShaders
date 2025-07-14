#include "scripts/include/camera.hpp"

void CameraScript::onUpdate(float timestep)
{
	bool recalc = false;

	glm::vec3 right = { m_actor.transform[0][0], m_actor.transform[0][1], m_actor.transform[0][2] };
	glm::vec3 forward = { -m_actor.transform[2][0], -m_actor.transform[2][1], -m_actor.transform[2][2] };

	if (m_winRef.doIsKeyPressed(GLFW_KEY_W)) { m_actor.translation += forward * m_movementSpeed.z * timestep; recalc = true; }
	if (m_winRef.doIsKeyPressed(GLFW_KEY_A)) { m_actor.translation -= right * m_movementSpeed.x * timestep; recalc = true; }
	if (m_winRef.doIsKeyPressed(GLFW_KEY_S)) { m_actor.translation -= forward* m_movementSpeed.z * timestep; recalc = true; }
	if (m_winRef.doIsKeyPressed(GLFW_KEY_D)) { m_actor.translation += right * m_movementSpeed.x * timestep; recalc = true; }
	if (m_winRef.doIsKeyPressed(GLFW_KEY_UP)) { m_actor.translation.y += m_movementSpeed.y * timestep; recalc = true; }
	if (m_winRef.doIsKeyPressed(GLFW_KEY_DOWN)) { m_actor.translation.y -= m_movementSpeed.y * timestep; recalc = true; }


	if (m_winRef.doIsKeyPressed(GLFW_KEY_LEFT)) {
		m_actor.rotation = glm::rotate(m_actor.rotation, m_turnSpeed * timestep, glm::vec3(0, 1, 0));
		recalc = true;
	}

	if (m_winRef.doIsKeyPressed(GLFW_KEY_RIGHT)) {
		m_actor.rotation = glm::rotate(m_actor.rotation, -m_turnSpeed * timestep, glm::vec3(0, 1, 0));
		recalc = true;
	}

	if (recalc) m_actor.recalc();
}

