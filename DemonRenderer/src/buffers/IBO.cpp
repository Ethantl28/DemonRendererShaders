#include <glad/gl.h>

#include "buffers/IBO.hpp"
#include "core/log.hpp"

void IBO::init(const std::vector<uint32_t>& indices)
{
	if (!m_ID) {
		m_count = indices.size();
		glCreateBuffers(1, &m_ID);
		glNamedBufferStorage(m_ID, sizeof(uint32_t) * m_count, indices.data(), GL_DYNAMIC_STORAGE_BIT);
	}
	else spdlog::error("IBO reinitilisation attempted on IBO with ID {}", m_ID);
}

IBO::~IBO()
{
	if(m_ID) glDeleteBuffers(1, &m_ID);
}

void IBO::edit(const std::vector<uint32_t>& indices, uint32_t offset)
{
	if (m_ID) glNamedBufferSubData(m_ID, offset, sizeof(uint32_t) * m_count, indices.data());
}
