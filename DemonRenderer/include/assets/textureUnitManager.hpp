/** \file textureUnitManager.hpp */
#pragma once

#include <vector>
#include <unordered_map>

/** \class TextureUnitManager
*  \brief A class which controls which textures are bound to which GPU texture slot.
*/

class TextureUnitManager
{
public:
	TextureUnitManager(size_t capacity); //!< Constructor with capacity
	bool [[nodiscard]] getUnit(uint32_t textureID, uint32_t& unit); //!< Give the texture unit that textureID is bound to. Returns true if binding is required.
	inline bool [[nodiscard]] full() { return m_nextUnit == m_textureIDs.capacity(); }; //!< Are all texture unit in use already
	void clear(); //!< Clear all data 
private:
	std::vector<uint32_t> m_textureIDs; //!< Id of texture bound to a unit
	std::unordered_map<uint32_t, uint32_t> m_textureUnits; //!< Map of texture IDs to texture units
	size_t m_nextUnit = 0; //!< Next position to bind to
};
