/** \file cubeMap.hpp */
#pragma once

#include <array>
#include "assets/managedTexture.hpp"

/**
* \class CubeMap
* \brief Creates a cubemap from texture files.
*/
class CubeMap : public ManagedTexture
{
public:
	CubeMap() = delete; //!< Deleted default constructor
	CubeMap(const std::array<const char*, 6>& filepaths, bool isHDR = false, bool flipUvs = true); //!< Cubemap built from 6 seperate files, is it HR and should UVs be flipped
	CubeMap(CubeMap& other) = delete; //!< Deleted copy constructor
	CubeMap(CubeMap&& other) = delete; //!< Deleted move constructor
	CubeMap& operator=(CubeMap& other) = delete; //!< Deleted copy assignment operator
	CubeMap& operator=(CubeMap&& other) = delete; //!< Delete move assignment operator
	~CubeMap(); //!< Destructor which removes the cubemap from the GPU
	inline bool [[nodiscard]] isHDR() const noexcept { return m_isHDR; } //!< Is this cubemap HDR
private:
	bool m_isHDR{ false }; //!< Holds whether or not this cubemap high dynamic range
};
