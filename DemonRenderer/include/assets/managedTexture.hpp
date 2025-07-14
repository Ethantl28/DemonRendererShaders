/** \file managedTexture.hpp */
#pragma once

/** \class ManagedTexture
*   \brief A managed texture is one whose device binding point is managed by the texture unit manager.
*   You should inherit this class into any texture you wish who binding point you want managed.
*	It is not a standalone class, hence the protected constructor.
*/

#include "assets/textureUnitManager.hpp"

class ManagedTexture
{
public:
	uint32_t [[nodiscard]] getUnit(); //!< Get the texture unit, don't worry about flushing etc.
	/** Get the texture unit.
	* @param flushNotification Set to true when the texture unit manager is full.
	* @param flushIfRequired Sets whether the texture unit will be flushed automatically if full.
	*/
	uint32_t [[nodiscard]] getUnit(bool& flushNotification, bool flushIfRequired = true);
protected:
	ManagedTexture() = default; //!< Default constructor, required by subclasses
	uint32_t m_ID{ 0 }; //!< Hold the GPU ID for the texture
private:
	/**
	* Manages which slots (32 in total) textures are bound to.  
	* Static as we only want one record of this for all managed textures.
	*/
	inline static TextureUnitManager m_textureUnitManager = TextureUnitManager(32);
};