/** \file UBO.hpp */
#pragma once

#include <memory>
#include <unordered_map>

#include "buffers/UBOLayout.hpp"


/** \class UBO
*	\brief A Uniform buffer obkect
*	Holds a group of data (uniforms) which can be accessed by mulitple shader programs
*/
class UBO
{
public:
	UBO(const UBOLayout& layout); //!< Need a layout to make a buffer
	UBO(UBO& other) = delete; //!< Deleted copy constructor
	UBO(UBO&& other) = default; //!< Move assignment operator, required for emplace back
	UBO& operator=(UBO& other) = delete; //!< Deleted copy assignment operator
	UBO& operator=(UBO&& other) = default; //!< Move assignment operator, be aware of destructor behavoiur
	~UBO(); //!< Destructor
	inline uint32_t getID() { return m_ID; } //!< Get the GPU ID of this buffer
	inline UBOLayout getLayout() { return m_layout; } //!< Get the layout of this buffer
	bool uploadData(const std::string& uniformName, void* data); //!< Upload a single uniform value to this UBO
private:
	UBOLayout m_layout; //!< Uniform Buffer layout
	std::unordered_map<std::string, std::pair<uint32_t, uint32_t>> m_uniformCache; //!< Stores uniform names with offsets and sizes
	uint32_t m_ID; //!< OpenGL ID
};
