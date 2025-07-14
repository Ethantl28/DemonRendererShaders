#include "buffers/VBOlayout.hpp"

void VBOLayout::addElement(const VBOLayoutElement& element)
{
	m_elements.push_back(element);
	calcStrideAndOffset();
}

void VBOLayout::calcStrideAndOffset()
{
	uint32_t offset = 0;

	for (auto& element : m_elements)
	{
		element.m_offset = offset;
		offset += element.m_componentCount * sizeof(float); // Need GL_Type size look up
	}

	m_stride = offset;
}
