/** \file renderable.hpp */
#pragma once

#include <memory>
#include "buffers/VAO.hpp"
#include "rendering/material.hpp"
#include "buffers/SSBO.hpp"

/** 
*	\struct Renderable
*	\brief A thing which is renderable
*	The simplest Renderable thing will have some geometry and a material.
*	Depth versions of these can be use which only give vertex positions and only render depth.
*	All geometry may take the form of a VAO or SSBO.
*/
struct Renderable
{
	std::shared_ptr<Material> material{ nullptr };
	std::shared_ptr<VAO> geometry{ nullptr };
	std::shared_ptr<SSBO> SSBOgeometry{ nullptr };
	std::shared_ptr<Material> depthMaterial{ nullptr };
	std::shared_ptr<VAO> depthGeometry{ nullptr };
	std::shared_ptr<SSBO> SSBOdepthGeometry{ nullptr };
};