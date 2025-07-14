/** \file FBO.hpp */
#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "buffers/FBOlayout.hpp"
#include "buffers/RBO.hpp"
#include "assets/texture.hpp"
#include "events/windowEvents.hpp"

/** \class FBO
*	\brief A frambuffer object.
*	A collection of target for colour and depth whcih can be sampled or non-sampled.
*	OnResize needs completing and buffers remaking.
*	Provide targets for render passes/
* */

class FBO
{
public:
	FBO() { m_ID = 0; }; // Default framebuffer
	FBO(glm::ivec2 size, FBOLayout layout);
	FBO(FBO& other) = delete; //!< Deleted copy constructor
	FBO(FBO&& other) = delete; //!< Deleted move constructor
	FBO& operator=(FBO& other) = delete; //!< Deleted copy assignment operator
	FBO& operator=(FBO&& other) = delete; //!< Delete move assignment operator
	~FBO();
	void use(); //!< Bind this framebuffer
	void onResize(WindowResizeEvent& e); // On resize function
	uint32_t getID() { return m_ID; } //!< Get the API specific render ID
	std::shared_ptr<Texture> getTarget(uint32_t index);
protected:
	uint32_t m_ID{ 0 };
	glm::ivec2 m_size{ glm::ivec2(0,0)}; //!< Size of the framebuffer
	FBOLayout m_layout; //! Layout of FBO attachements
	std::vector<std::shared_ptr<Texture>> m_sampledTargets; //!< Sampled targets, colour or depth
	std::vector<uint32_t> m_colAttachments; //!< Sampled colour targets ( no depth) 
	std::vector<std::shared_ptr<RBO>> m_nonSampledTargets; //!< Non sample targets
};