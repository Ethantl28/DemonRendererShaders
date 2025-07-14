#include <glad/gl.h>
#include "buffers/VBO.hpp"
#include "core/log.hpp"

void VBO::init(const std::vector<float> vertices, const VBOLayout& layout)
{
	if (!m_ID) {
		m_layout = layout;
		glCreateBuffers(1, &m_ID);
		glNamedBufferStorage(m_ID, sizeof(float) * vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);
	}
	else spdlog::error("VBO reinitilisation attempted on VBO with ID {}", m_ID);
}
void VBO::init(void* vertices, uint32_t size, const VBOLayout& layout)
{
	if (!m_ID) {
		m_layout = layout;
		glCreateBuffers(1, &m_ID);
		glNamedBufferStorage(m_ID, size, vertices, GL_DYNAMIC_STORAGE_BIT);
	}
	else spdlog::error("VBO reinitilisation attempted on VBO with ID {}", m_ID);
}

VBO::~VBO()
{
	if(m_ID) glDeleteBuffers(1, &m_ID);
}

void VBO::edit(const std::vector<float> vertices, uint32_t offset)
{
	glNamedBufferSubData(m_ID, offset, sizeof(float) * vertices.size(), vertices.data());
}

void VBO::edit(void* vertices, uint32_t size, uint32_t offset)
{
	glNamedBufferSubData(m_ID, offset, size, vertices);
}
