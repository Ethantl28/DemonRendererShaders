/** \file VBOLayout.hpp */

#pragma once
#include <vector>

/** \struct VBOLayoutElement
*	\brief A set of data needed to define one element of a VBO */
struct VBOLayoutElement
{
public:
	VBOLayoutElement() {} //!< Default constructor
	/** Constructor with all data */
	VBOLayoutElement(uint32_t dataType, uint32_t componentCount, uint32_t instanceDivisor = 0, bool normalised = false) :
		m_dataType(dataType),
		m_componentCount(componentCount),
		m_offset(0),
		m_instanceDivisor(instanceDivisor),
		m_normalised(normalised)
	{}

	uint32_t m_dataType = 0; //!< OpenGL data type of the element
	uint32_t m_componentCount = 0; //!< Number of components i.e. vec2 == 2 vec3 == 3
	uint32_t m_offset = 0; //!< Offset from the buffer start in bytes
	bool m_normalised = false; //!< Is this a normalised variable
	uint32_t m_instanceDivisor = 0; //!< Is this an instanced variable, if == 0 it is not
};

/** \class VBOLayout
*	\brief A collection of layout element which together give a full description of a VBO */
class VBOLayout
{
public:
	VBOLayout() = default; //!< Default constructor
	/** Initialiser list constructor */
	VBOLayout(const std::initializer_list<VBOLayoutElement>& element, uint32_t stride = 0) : m_elements(element), m_stride(stride) { calcStrideAndOffset(); }
	inline uint32_t getStride() const noexcept { return m_stride; } //!< Returns the stride of the VBO
	void addElement(const VBOLayoutElement& element); //!< Add an element tot he layout
	inline std::vector<VBOLayoutElement>::iterator begin() { return m_elements.begin(); } //!< Element begin iterator
	inline std::vector<VBOLayoutElement>::iterator end() { return m_elements.end(); } //!< Element end iterator
	inline std::vector<VBOLayoutElement>::const_iterator begin() const { return m_elements.begin(); } //!< Element const begin iterator
	inline std::vector<VBOLayoutElement>::const_iterator end() const { return m_elements.end(); }//!< Element const end iterator
private:
	std::vector<VBOLayoutElement> m_elements; //!< Buffer elements
	uint32_t m_stride = 0; //!< Width is bytes of the a buffer line
	void calcStrideAndOffset(); //!< Calculate stride and offsets based on elements
};

