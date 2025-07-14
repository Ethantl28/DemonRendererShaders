/** \file texture.hpp */
#pragma once
#include <glad/gl.h>
#include "assets/managedTexture.hpp"


/** \struct TextureDescription
*   \brief Description of a text, it's size, number of channels, whether of not it is HDR and pixel data.
*/
struct TextureDescription
{
	uint32_t width{ 0 };
	uint32_t height{ 0 };
	uint32_t channels{ 0 };
	bool isHDR{ false };
	unsigned char* data{ nullptr };
};

/** \class Texture
*   \brief A texture held on the GPU.
*/
class Texture : public ManagedTexture
{
public:
	Texture() = delete; //!< Deleted default constructor
	explicit Texture(const char* filepath); //!< Constructor which takes a path to an image file to be loaded
	explicit Texture(const TextureDescription& desc); //!< Constructor which takes a TextureDescription
	Texture(Texture& other) = delete; //!< Deleted copy constructor
	Texture(Texture&& other) = delete; //!< Deleted move constructor
	Texture& operator=(Texture& other) = delete; //!< Deleted copy assignment operator
	Texture& operator=(Texture&& other) = delete; //!< Delete move assignment operator
	~Texture();  //!< Destructor which removes the texture from the GPU
	void edit(uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height, unsigned char* data); //!< Edit the textures data on the GPU
	inline uint32_t [[nodiscard]] getID() noexcept { return m_ID; } //!< Get the ID
	inline uint32_t [[nodiscard]] getWidth() noexcept { return m_width; } //!< Get the width in pixels
	inline uint32_t [[nodiscard]] getHeight() noexcept { return m_height; } //!< Get the height in pixels
	inline uint32_t [[nodiscard]] getWidthf() noexcept { return static_cast<float>(m_width); } //!< Get the width in pixels as a floating point number
	inline uint32_t [[nodiscard]] getHeightf() noexcept { return static_cast<float>(m_height); } //!< Get the height in pixels as a floating point number
	inline uint32_t [[nodiscard]] getChannels() noexcept { return m_channels; } //!< Get the number of channels in the texture
	inline bool [[nodiscard]] isHDR() noexcept { return m_isHDR; } //!< Get whether or not the texture is high dynamic range

private:
	uint32_t m_width{ 0 }; //!< Width in pixels
	uint32_t m_height{ 0 }; //!< Height in pixels
	uint32_t m_channels{ 0 }; //!< Number of channels
	bool m_isHDR{ false }; //!< Is the texture high dynamic range
private:
	void init(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data, bool isHDR); //!< Initialise the texture
};
