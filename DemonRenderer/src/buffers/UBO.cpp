#include "buffers/UBO.hpp"

UBO::UBO(const UBOLayout& layout) : m_layout(layout)
{
	for (auto& elem : m_layout)
	{
		m_uniformCache[elem.m_name] = std::pair<uint32_t, uint32_t>(elem.m_offset, elem.m_size);
	}

	glCreateBuffers(1, &m_ID);
	glNamedBufferStorage(m_ID, layout.getSize(), NULL, GL_DYNAMIC_STORAGE_BIT);

	glBindBufferRange(GL_UNIFORM_BUFFER, m_layout.getBindingPoint(), m_ID, 0, m_layout.getSize());
}

UBO::~UBO()
{
	glDeleteBuffers(1, &m_ID);
}

bool UBO::uploadData(const std::string& uniformName, void* data)
{
	bool result = false;
	auto it = m_uniformCache.find(uniformName);
	if (it != m_uniformCache.end())
	{
		auto& [offset, size] = it->second;
		glNamedBufferSubData(m_ID, offset, size, data);
		result = true;
	}
	return result;
}
