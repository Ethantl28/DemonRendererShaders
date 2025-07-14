/** \file VBO.hpp */
#pragma once

#include <vector>
#include "VBOlayout.hpp"

/** /class VBO 
*	\brief Vertex Buffer Object, holds vertices on the device
*/
class VBO
{
public:
	VBO() = default; //!< Default contructor, does allocate anythign on the device
	VBO(VBO& other) = delete; //!< Deleted copy constructor
	VBO(VBO&& other) = default; //!< Move assignment operator, required for emplace back
	VBO& operator=(VBO& other) = delete; //!< Deleted copy assignment operator
	VBO& operator=(VBO&& other) = default; //!< Move assignment operator, be aware of destructor behavoiur
	~VBO(); //!< Destructor
	void init(const std::vector<float> vertices, const VBOLayout& layout); //!< Init the VBO with floats for vertex data
	void init(void* vertices, uint32_t size, const VBOLayout& layout); //!< Init the VBO with something else for vertex data
	void edit(const std::vector<float> vertices, uint32_t offset); //!< Edit the VBO with floats for vertex data
	void edit(void* vertices, uint32_t size, uint32_t offset); //!< Edit the VBO with something else for vertex data
	inline uint32_t getID() const noexcept{ return m_ID; } //!< Return the device ID of the VBO
	inline const VBOLayout& getLayout() const noexcept { return m_layout; } //!< Return the layour of the VBO
private:
	uint32_t m_ID{ 0 }; //!< Open GL ID
	VBOLayout m_layout{}; //!< Layout
};
