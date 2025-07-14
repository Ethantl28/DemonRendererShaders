#include <glad/gl.h>
#include "buffers/RBO.hpp"

RBO::RBO(AttachmentType type, glm::ivec2 size)
{
	glCreateRenderbuffers(1, &m_ID);

	switch (type)
	{
	case AttachmentType::Colour:
		glNamedRenderbufferStorage(m_ID, GL_RGB8, size.x, size.y);
		break;
	case AttachmentType::ColourHDR:
		glNamedRenderbufferStorage(m_ID, GL_RGB16F, size.x, size.y);
		break;
	case AttachmentType::Depth:
		glNamedRenderbufferStorage(m_ID, GL_DEPTH_COMPONENT32, size.x, size.y);
		break;
	case AttachmentType::DepthAndStencil:
		glNamedRenderbufferStorage(m_ID, GL_DEPTH24_STENCIL8, size.x, size.y);
		break;
	}
}

RBO::~RBO()
{
	glDeleteRenderbuffers(1, &m_ID);
}
