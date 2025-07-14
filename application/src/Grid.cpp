
#include "Grid.hpp"


Grid::Grid(uint32_t width, uint32_t height, uint32_t step) : m_width(width), m_height(height), m_stepSize(step)
{
    makeVertices();
    makeIndices();
}


void Grid::makeVertices() {
    for (int x = 0; x <= m_width; x++) {
        float offsetX = x * m_stepSize;
        for (int z = 0; z <= m_height; z++) {
            float offsetZ = z * m_stepSize;

            // Position in XYZ (Y is 0 for a flat surface)
            m_vertices.push_back(offsetX);       // X position
            m_vertices.push_back(0.0f);          // Y position (flat)
            m_vertices.push_back(offsetZ);       // Z position

            // Texture coordinates (u, v)
            m_vertices.push_back(static_cast<float>(x) / m_width);   // U
            m_vertices.push_back(static_cast<float>(z) / m_height);  // V

            // Store positions for depth passes
            m_vertexPositions.push_back(offsetX);
            m_vertexPositions.push_back(0.0f); 
            m_vertexPositions.push_back(offsetZ);
        }
    }

}

void Grid::makeIndices() {
    // Calculate indices for indexed draw calls

    for (int z = 0; z < m_height; z++) {
        for (int x = 0; x < m_width; x++) {
            // Calculate the indices of the four corners of each cell
            uint32_t topLeft = z * (m_width + 1) + x;
            uint32_t topRight = topLeft + 1;
            uint32_t bottomLeft = (z + 1) * (m_width + 1) + x;
            uint32_t bottomRight = bottomLeft + 1;

            // First triangle (top-left, bottom-left, top-right)
            m_indices.push_back(topLeft);
            m_indices.push_back(topRight);
            m_indices.push_back(bottomLeft);

            // Second triangle (bottom-left, top-right, bottom-right)
            m_indices.push_back(bottomLeft);
            m_indices.push_back(topRight);
            m_indices.push_back(bottomRight);
        }
    }

}


