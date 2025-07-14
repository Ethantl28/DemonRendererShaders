/** \file UBOlayout.hpp */
#pragma once
#include <glad/gl.h>
#include <vector>
#include <string>
#include <unordered_map>

namespace UBOConsts
{
	const std::unordered_map<uint32_t, size_t> STD140DatatypeSizes = { 
		{GL_SAMPLER_2D, 4}, 
		{GL_FLOAT,		4},
		{GL_FLOAT_VEC3, 16}, 
		{GL_FLOAT_VEC4, 16}, 
		{GL_FLOAT_MAT4, 64} 
	};
}

/** \struct UBOLayoutElement
*	\brief A set of data needed to define one element of a UBO */
struct UBOLayoutElement
{
public:
	UBOLayoutElement() {} //!< Default constructor
	/** Constructor with all data */
	UBOLayoutElement(const std::string& name, uint32_t dataType, uint32_t offset) :
		m_name(name),
		m_dataType(dataType),
		m_size(UBOConsts::STD140DatatypeSizes.at(dataType)),
		m_offset(offset)
	{}
	std::string m_name {""}; //!< Name of the uniform
	uint32_t m_dataType{ 0 }; //!< OpenGL data type of the element
	uint32_t m_size{ 0 }; //!< Size of aligned data type in STD140
	uint32_t m_offset{ 0 }; //!< Offset from the beginning of the buffer
};

/** \class UBOLayout
*	\brief A collection of layout element which together give a full description of a UBO */
class UBOLayout
{
public:
	UBOLayout() = default; //!< Deafult constructor
	/** Initialiser list constructor */
	UBOLayout(const std::initializer_list<UBOLayoutElement>& elements, const std::string& blockName, uint32_t binding, uint32_t size) :
		m_elements(elements), 
		m_blockName(blockName),
		m_size(size),
		m_binding(binding)
	{ checkSize(); }
	/** Populated data constructor */
	UBOLayout(const std::string& blockName, uint32_t binding, uint32_t size) :
		m_blockName(blockName),
		m_size(size),
		m_binding(binding)
	{}
	inline const std::string& getBlockName() const { return m_blockName; }  //!< Returns the name of the UBO block
	inline uint32_t getSize() const { return m_size; } //!< Returns the size of the UBO conforming to STD140 layout
	inline uint32_t getBindingPoint() const { return m_binding; } //!< Returns the binding point of the UBO
	void addElement(const UBOLayoutElement& element); //!< Add an element
	inline std::vector<UBOLayoutElement>::iterator begin() { return m_elements.begin(); } //!< Element begin iterator
	inline std::vector<UBOLayoutElement>::iterator end() { return m_elements.end(); }  //!< Element end iterator
	inline std::vector<UBOLayoutElement>::const_iterator begin() const { return m_elements.begin(); } // !< Element const begin iterator
	inline std::vector<UBOLayoutElement>::const_iterator end() const { return m_elements.end(); } //!< Element const end iterator
	bool operator==(const UBOLayout& other) const; //!< Equality operator
private:
	std::vector<UBOLayoutElement> m_elements; //!< Buffer elements
	std::string m_blockName; //!< Name of UBO
	uint32_t m_size; //!< Width is bytes of the a buffer line
	uint32_t m_binding; //!< Binding point for the UBO 
	void checkSize(); //!< Calculate stride and offsets based on elements
};