/** \file transformable.hpp */
#pragma once
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

/** \struct Transformable
*	\brief A thing which can be transformed i.e. transalted, rotated or scaled */
struct Transformable
{
	glm::mat4 transform{ glm::mat4(1.f) }; //!< Overall transform
	glm::vec3 translation{ glm::vec3(0.f) }; //!< Translation i.e. position
	glm::quat rotation{ glm::quat(glm::vec3(0.f)) }; //!< Orientation as a quaternion
	glm::vec3 scale{ glm::vec3(1.f) }; //!< Scale

	Transformable() = default; //!< Default constructor

	/** Constructor with values */
	Transformable(const glm::vec3& t, const glm::vec3& r, const glm::vec3 s) :
		translation(t),
		rotation(glm::quat(r)),
		scale(s)
	{
		recalc();
	}

	/** Recalculate the transform.  Must be done when T, R or S change*/
	void recalc()
	{
		glm::mat4 t = glm::translate(glm::mat4(1.f), translation);
		glm::mat4 r = glm::toMat4(rotation);
		glm::mat4 s = glm::scale(glm::mat4(1.f), scale);

		transform = t * r * s;
	}
	/** Recalculate the transform relative to some parent transform.  Must be done when T, R or S change*/
	void recalc(const glm::mat4& parent)
	{
		glm::mat4 t = glm::translate(glm::mat4(1.f), translation);
		glm::mat4 r = glm::toMat4(rotation);
		glm::mat4 s = glm::scale(glm::mat4(1.f), scale);

		transform = parent * t * r * s;
	}
};