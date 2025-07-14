/** \file scene.hpp */
#pragma once

#include "rendering/lights.hpp"
#include "gameObjects/actor.hpp"

/** \struct Scene
*	\brief Holds everything which makes up a scene
*/
struct Scene
{
	std::vector<DirectionalLight> m_directionalLights; //!< Directional lights
	std::vector<PointLight> m_pointLights; //!< Point lights
	std::vector<SpotLight> m_spotLights; //!< Spot lights
	std::vector<Actor> m_actors; //!< Actors
};