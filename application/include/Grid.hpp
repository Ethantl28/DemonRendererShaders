#pragma once
#include <DemonRenderer.hpp>

/*

 Create width * height 2D grid of vertices
 Initailly flat and considerably more vertices than we need to draw a quad. However, we will use these vertices in later labs

*/

class Grid
{
public:
	Grid(uint32_t width = 10, uint32_t height = 10, uint32_t step = 10); // Constructor takes width, height and size of each cell. Default is 10 for each. 

	Grid(Grid& other) = delete; //!< Deleted copy constructor
	Grid(Grid&& other) = delete; //!< Deleted move constructor
	Grid& operator=(Grid& other) = delete; //!< Deleted copy assignment operator
	Grid& operator=(Grid&& other) = delete; //!< Deleted move assignment operator

	~Grid() = default; //!< Destructor

	inline const std::vector<float>& getVertices() const { return m_vertices; } //!< Returns the vertices of Grid
	inline const std::vector<unsigned int>& getIndices() const { return m_indices; } //!< Returns the indices of Grid
	inline const std::vector<float>& getVertexPositions() const { return m_vertexPositions; } //!< Returns the Vertex Positions of Grid

private:
	void makeVertices();
	void makeIndices();


	std::vector<float> m_vertices;
	std::vector<float> m_vertexPositions; // only positions of vertices ( not UVs, etc)
	std::vector<unsigned int> m_indices;
	// unitialised because default in constructor
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_stepSize;
};