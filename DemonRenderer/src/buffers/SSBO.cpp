#include "buffers/SSBO.hpp"



SSBO::SSBO(uint32_t size, uint32_t elementCount) : m_size(size), m_elementCount(elementCount)
{
   glCreateBuffers(1, &m_ID);
   glNamedBufferData(m_ID, m_size, nullptr, GL_DYNAMIC_DRAW);
}

SSBO::~SSBO()
{
	glDeleteBuffers(1, &m_ID);
}

void SSBO::bind(uint32_t unit)
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, unit, m_ID);
}


