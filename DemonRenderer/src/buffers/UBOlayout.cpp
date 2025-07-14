#include "buffers/UBOlayout.hpp"
#include "core/log.hpp"

void UBOLayout::addElement(const UBOLayoutElement& element)
{
	m_elements.push_back(element);
}

bool UBOLayout::operator==(const UBOLayout& other) const
{
	if (m_binding != other.getBindingPoint()) return false;
	if (m_blockName != other.getBlockName()) return false;
	if (m_size != other.getSize()) return false;
	
	if(std::distance(begin(), end()) != std::distance(other.begin(), other.end())) return false;

	auto it = begin();
	auto otherIt = other.begin();
	for (; it != end() && otherIt != other.end(); ++it, ++otherIt)
	{
		if (it->m_dataType != otherIt->m_dataType) return false;
		if (it->m_name != otherIt->m_name) return false;
		if (it->m_offset != otherIt->m_offset) return false;
		if (it->m_dataType != otherIt->m_dataType) return false;
	}
	
	return true;
}

void UBOLayout::checkSize()
{
	uint32_t l_size = 0;

	for (auto& element : m_elements) {
		l_size += element.m_size;
	}

	if (l_size != m_size) spdlog::error("UBO layout size does not match size on disk for UBO {}. Size on disk {} size from layout {}",  m_blockName, m_size,l_size);
}
