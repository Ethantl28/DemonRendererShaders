/** \file SSBO.hpp */
#pragma once

#include <glad/gl.h>
#include <cstdint>
#include <vector>

/** \class SSBO
*	\brief a Shader Storage Buffer Object
*	Holds a bunch of data on the GPU whcih may or may not be structured
* */

class SSBO {
public:
	SSBO() = delete; //!< Deleted ublic constructor
	template<typename T>
	SSBO(uint32_t size, uint32_t elementCount, T* data); //!< Constructor takes size of element ( a struct), number of elements, and data
	SSBO(uint32_t size, uint32_t elementCount);          //!< overloaded Constructor, use when creating a sized, empty SSBO
	SSBO(SSBO& other) = delete; //!< Deleted copy constructor
	SSBO(SSBO&& other) = delete; //!< Deleted move constructor
	SSBO& operator=(SSBO& other) = delete; //!< Deleted copy assignment operator
	SSBO& operator=(SSBO&& other) = delete; //!< Deleted move assignment operator
	~SSBO(); //!< Destructor
	inline uint32_t getID() const { return m_ID; } //!< Returns the device ID of the SSBO
	inline uint32_t getElementCount() const { return m_elementCount; } //!< Returns the number of elements
	void bind(uint32_t unit);  // bind SSBO at binding point

	template<typename T>
	void edit(uint32_t offset, uint32_t size, T* dataPointer); //!< edit/fill data in SSBO

	template<typename T>
	std::vector<T>  writeToCPU();   //!< write SSBO to CPU

private:
	uint32_t m_ID{ 0 }; //!< Render ID
	uint32_t m_size{ 0 };        //!< Size in bytes of buffer
	uint32_t m_elementCount{ 0 };  // !< Number of elements in SSBO
};

template<typename T>
SSBO::SSBO(uint32_t size, uint32_t elementCount, T* data) : m_size(size), m_elementCount(elementCount)
{
	glCreateBuffers(1, &m_ID);
	glNamedBufferData(m_ID, m_size, data, GL_DYNAMIC_DRAW);
}

template<typename T>
void SSBO::edit(uint32_t offset, uint32_t SIZE, T* data)
{
	glNamedBufferSubData(m_ID, offset, SIZE, data);
}

template<typename T>
std::vector<T> SSBO::writeToCPU()
{

	std::vector<T> data(m_elementCount);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ID);
	void* ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);	
	if (ptr) {
		T* SSBOdata = static_cast<T*>(ptr);
		data.assign(SSBOdata, SSBOdata + m_elementCount);
		
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);   // Breaks everything if not unmapped!
	return data;
}