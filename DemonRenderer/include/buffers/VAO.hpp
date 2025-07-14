/** \file VAO.hpp */
#pragma once

#include "IBO.hpp"
#include "VBO.hpp"

/**	\class VAO
*	\brief A Vertex array object. Linke VBOs to an IBO
* */
class VAO
{
public:
	VAO() = delete; //!< Deleted default constructor
	explicit VAO(const std::vector<uint32_t>& indices); //!< Constructor whichs takes index data
	VAO(VAO& other) = delete; //!< Deleted copy constructor
	VAO(VAO&& other) = delete; //!< Deleted move constructor
	VAO& operator=(VAO& other) = delete; //!< Deleted copy assignment operator
	VAO& operator=(VAO&& other) = delete; //!< Deleted move assignment operator
	~VAO();

	void addVertexBuffer(const std::vector<float> vertices, const VBOLayout& layout); //!< Add a vertex buffer to the VAO as floats
	void addVertexBuffer(void* vertices, uint32_t size, const VBOLayout& layout); //!< Add a vertex buffer to the VAO as something else
	void editVertices(size_t index, const std::vector<float> vertices, uint32_t offset); //!< Edit a vertex buffer to the VAO as floats
	void editVertices(size_t index, void* vertices, uint32_t size, uint32_t offset); //!< Edit a vertex buffer to the VAO as something else
	inline void overrideDrawCount(uint32_t drawCount) { m_overridenDrawCount = std::min(m_IBO.getCount(), drawCount); } // !< Override the drawcount, allow partial drawing
	inline void resetDrawCount() { m_overridenDrawCount = std::numeric_limits<uint32_t>::max(); } //!< Resets draw count
	inline uint32_t getID() const noexcept { return m_ID; } //!< Returns the device ID of the VAO
	inline uint32_t getDrawCount() const noexcept { return std::min(m_IBO.getCount(), m_overridenDrawCount); } //!< Returns the index count for the VAO
private:
	std::vector<VBO> m_vertexBuffer; //!< Container for VBOs
	IBO m_IBO; //!< IBO
	uint32_t m_ID{ 0 }; //!< Render ID
	uint32_t m_attributeIndex{ 0 }; //!< Attribute index
	uint32_t m_overridenDrawCount{ std::numeric_limits<uint32_t>::max() };
};