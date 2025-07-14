#include <glad/gl.h>
#include "buffers/FBO.hpp"
#include "core/log.hpp"

FBO::FBO(glm::ivec2 size, FBOLayout layout) : 
	m_layout(layout),
	m_size(size)
{
	glCreateFramebuffers(1, &m_ID);

	uint32_t colourAttachementCount = 0;
	uint32_t thisAttachment{ 0 };

	for (auto& [type, isSampled] : m_layout) // Using structured bindings
	{
		if (isSampled)
		{
			TextureDescription td;
			td.width = m_size.x;
			td.height = m_size.y;

			switch (type)
			{
			case AttachmentType::Colour:
				td.channels = 3;
				m_sampledTargets.push_back(std::make_shared<Texture>(td));
				thisAttachment = GL_COLOR_ATTACHMENT0 + colourAttachementCount;
				glNamedFramebufferTexture(m_ID, thisAttachment, m_sampledTargets.back()->getID(), 0);
				m_colAttachments.push_back(thisAttachment);
				colourAttachementCount++;
				break;
			case AttachmentType::ColourHDR:
				td.channels = 3;
				td.isHDR = true;
				m_sampledTargets.push_back(std::make_shared<Texture>(td));
				thisAttachment = GL_COLOR_ATTACHMENT0 + colourAttachementCount;
				glNamedFramebufferTexture(m_ID, GL_COLOR_ATTACHMENT0 + colourAttachementCount, m_sampledTargets.back()->getID(), 0);
				m_colAttachments.push_back(thisAttachment);
				colourAttachementCount++;
				break;
			case AttachmentType::Depth:
				td.channels = 0;
				m_sampledTargets.push_back(std::make_shared<Texture>(td));
				glNamedFramebufferTexture(m_ID, GL_DEPTH_ATTACHMENT, m_sampledTargets.back()->getID(), 0);
				break;
			default:
				spdlog::error("Unsupported FBO sampled attachment type: {}", static_cast<int>(type));
				break;
			}
		}
		else
		{
			switch (type)
			{
			case AttachmentType::Colour:
				m_nonSampledTargets.push_back(std::make_shared<RBO>(type, size));
				glNamedFramebufferRenderbuffer(m_ID, GL_COLOR_ATTACHMENT0 + colourAttachementCount, GL_RENDERBUFFER, m_nonSampledTargets.back()->getID());
				colourAttachementCount++;
				break;
			case AttachmentType::Depth:
				m_nonSampledTargets.push_back(std::make_shared<RBO>(type, size));
				glNamedFramebufferRenderbuffer(m_ID, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_nonSampledTargets.back()->getID());
				break;
			case AttachmentType::DepthAndStencil:
				m_nonSampledTargets.push_back(std::make_shared<RBO>(type, size));
				glNamedFramebufferRenderbuffer(m_ID, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_nonSampledTargets.back()->getID());
				break;
			default:
				spdlog::error("Unsupported FBO non-sampled attachment type: {}", static_cast<int>(type));
				break;
			}
		}
	}

	if (colourAttachementCount == 0)
	{
		glNamedFramebufferDrawBuffer(m_ID,GL_NONE);
		glNamedFramebufferReadBuffer(m_ID,GL_NONE);
	}

	if (colourAttachementCount > 1) {   //MTR
		glNamedFramebufferDrawBuffers(m_ID, colourAttachementCount, m_colAttachments.data());
	}

	if (glCheckNamedFramebufferStatus(m_ID, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		spdlog::error("Framebuffer is not complete!");

}

FBO::~FBO()
{
	m_sampledTargets.clear();
	m_nonSampledTargets.clear();
	if(m_ID > 0) glDeleteFramebuffers(1, &m_ID);
}

void FBO::use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
}

void FBO::onResize(WindowResizeEvent& e)
{
	// TO DO
	// Destroy and remake framebuffer and targets, sampled and non-sampled
}

std::shared_ptr<Texture> FBO::getTarget(uint32_t index)
{
	std::shared_ptr<Texture> result = nullptr;

	if (index < m_sampledTargets.size()) result = m_sampledTargets[index];

	return result;
}


