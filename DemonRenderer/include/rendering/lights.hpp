/** \file lights.hpp */
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

/**	\struct DirectionalLight
*	\brief Holds data for a directional light
*/
struct DirectionalLight
{
	glm::vec3 direction { 0.f,-1.f,0.f }; //!< Direction light is pointing
	glm::vec3 colour { 1.f }; //!< Colour of the light
};

/**	\struct PointLight
*	\brief Holds data for a point light
*/
struct PointLight
{
	glm::vec3 colour{ 1.f }; //!< Colour of the light
	glm::vec3 position{ 0.f }; //!< Position of the light
	glm::vec3 constants{1.f, 0.1f, 0.01f}; // Constant, linear, exponetial
};

/**	\struct SpotLight
*	\brief Holds data for a spot light
*/
struct SpotLight
{
	glm::vec3 colour{ 1.f }; //!< Colour of the light
	glm::vec3 position{ 0.f }; //!< Position of the light
	glm::vec3 direction{ 0.f,0.f,-1.f }; //!< Direction light is pointing
	glm::vec3 constants{ 1.f, 0.1f, 0.01f }; // Constant, linear, exponetial
	float cutOffCosine = cosf(glm::quarter_pi<float>()); //!< Inner cut off angle, begin fade
	float outerCutOffCosine = cosf(glm::half_pi<float>()); //!< Outer cut off angle, complete fade
};