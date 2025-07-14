/** \file IBO.hpp */
#pragma once

#include <vector>

/**	\class IBO
*	\brief An Indexed Buffer Object
*	Hold indices of vertices (held in a VBO) linked via a VAO.
*	Eliminates the need to repeated vertices.
*	Must be ound for indexed draw calls.
*/

class IBO
{
public:
	IBO() = default; //!< Default construtor, empty IBO
	void init(const std::vector<uint32_t>& indices); //!< Create by init call
	IBO(IBO& other) = delete; //!< Deleted copy constructor
	IBO(IBO&& other) = delete; //!< Deleted move constructor
	IBO& operator=(IBO& other) = delete; //!< Deleted copy assignment operator
	IBO& operator=(IBO&& other) = delete; //!< Delete move assignment operator
	~IBO(); //!< Destructor
	inline uint32_t getID() const noexcept { return m_ID; } //!< Returns the device ID of the IBO
	inline uint32_t getCount() const noexcept { return m_count; } //!< Returns the number of indiced held
	void edit(const std::vector<uint32_t>&, uint32_t offset); //!< Edit the contents of the IBO
private:
	uint32_t m_ID{ 0 }; //!< Render ID
	uint32_t m_count{ 0 }; //!< Effective draw count
};
