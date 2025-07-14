/** \file RBO.hpp */
#pragma once

#include <glm/glm.hpp>

#include "buffers/FBOlayout.hpp"

/** \class RBO
*	\brief A Render Buffer Object
*	Ued to store non-sampled targets of an FBO 
*/

class RBO
{
public:
	RBO() = delete; //!< Deleted deafult constructor
	RBO(AttachmentType type, glm::ivec2 size);  //!< Construtor which takes a type and size
	RBO(RBO& other) = delete; //!< Deleted copy constructor
	RBO(RBO&& other) = delete; //!< Deleted move constructor
	RBO& operator=(RBO& other) = delete; //!< Deleted copy assignment operator
	RBO& operator=(RBO&& other) = delete; //!< Delete move assignment operator
	~RBO(); //!< Destructor
	uint32_t getID() const noexcept{ return m_ID; } //!< Returns the device ID of the RBO
private: 
	uint32_t m_ID{ 0 }; //!< Device ID of the RBO
};
