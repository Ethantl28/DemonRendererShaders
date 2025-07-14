#include <glad/gl.h>
#include "buffers/VAO.hpp"

#include "core/log.hpp"

VAO::VAO(const std::vector<uint32_t>& indices)
{
	// NEEDS A PRECONDITION
	// THINK MORE ABOUT OWNERSHIP OF BUFFERS AND CONSTRUCTORS, UNIQUE_PTR MIGHT BE BETTER
	glCreateVertexArrays(1, &m_ID);

	m_IBO.init(indices);
	glVertexArrayElementBuffer(m_ID, m_IBO.getID());
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &m_ID);
}

void VAO::addVertexBuffer(const std::vector<float> vertices, const VBOLayout& layout)
{
	m_vertexBuffer.emplace_back();
	m_vertexBuffer.back().init(vertices, layout);
	const auto& vbo = m_vertexBuffer.back();
	glVertexArrayVertexBuffer(m_ID, 0, vbo.getID(), 0, layout.getStride());

	for (const auto& element : layout)
	{
		uint32_t normalised = GL_FALSE;
		if (element.m_normalised) { normalised = GL_TRUE; }
		glEnableVertexArrayAttrib(m_ID, m_attributeIndex);
		glVertexArrayAttribFormat(m_ID, m_attributeIndex, element.m_componentCount, element.m_dataType, normalised, element.m_offset);
	   // spdlog::info("{} {} {} {} {} {}", m_ID, m_attributeIndex, element.m_componentCount, element.m_dataType, normalised, element.m_offset);
		glVertexArrayAttribBinding(m_ID, m_attributeIndex, 0);
		m_attributeIndex++;
	}
}

void VAO::addVertexBuffer(void* vertices, uint32_t size, const VBOLayout& layout)
{
	m_vertexBuffer.emplace_back();
	m_vertexBuffer.back().init(vertices, size, layout);
}

void VAO::editVertices(size_t index, const std::vector<float> vertices, uint32_t offset)
{
	m_vertexBuffer.at(index).edit(vertices, offset);
}

void VAO::editVertices(size_t index, void* vertices, uint32_t size, uint32_t offset)
{
	m_vertexBuffer.at(index).edit(vertices, size, offset);
}


