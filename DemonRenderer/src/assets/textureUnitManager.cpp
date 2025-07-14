/** \file textureUnitManager.cpp **/

#include "assets/textureUnitManager.hpp"
#include "core/log.hpp"

TextureUnitManager::TextureUnitManager(size_t capacity)
{
	// Resize the tetxure IDs
	m_textureIDs.resize(capacity);
	// Fill with max value
	std::fill(m_textureIDs.begin(), m_textureIDs.end(), std::numeric_limits<uint32_t>::max());
	// Create to space in map, not nessecary but likely to improve early performance
	m_textureUnits.reserve(capacity * 2);
}

void TextureUnitManager::clear()
{
	std::fill(m_textureIDs.begin(), m_textureIDs.end(), std::numeric_limits<uint32_t>::max());
	m_textureUnits.clear();
	m_nextUnit = 0;
}

bool TextureUnitManager::getUnit(uint32_t textureID, uint32_t& unit)
{
	// Check if this texture is already bound
	auto unitItr = m_textureUnits.find(textureID);
	if (unitItr != m_textureUnits.end())
	{
		// Found texture in map to return the unit it is currently bound to
		unit = unitItr->second;
		return false;
	}
	// Check if buffer is already full
	if (full())
	{
		unit = -1;
		//spdlog::error("Texture Unit Manager already full when binding {}", textureID);
		return true;
	}
	else
	{
		//spdlog::info("Using texture Unit {}", m_nextUnit);
		// note current unit
		unit = m_nextUnit;

		// Insert the new textureID and m_nextUnit
		m_textureIDs[m_nextUnit] = textureID;
		m_textureUnits[textureID] = m_nextUnit;

		// Increment m_nextUnit
		m_nextUnit++;

		// Return 
		return true;
	}
}