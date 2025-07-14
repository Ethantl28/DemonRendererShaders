/** \file Camera.hpp */
#pragma once

#include <glm/glm.hpp>

/** \struct Camera
*	\brief Basic camera with a view and a projection */
struct Camera
{
	glm::mat4 view{ glm::mat4(1.f) }; //!< View matrix
	glm::mat4 projection{ glm::mat4(1.f) }; //!Projection Matrix
	void updateView(const glm::mat4& cameraTransform) //!< Update the view matrix based on the camera transform
	{
		view = glm::inverse(cameraTransform);
	}
};
