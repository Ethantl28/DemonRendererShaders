/** \file uniformDataTypes.hpp */
#pragma once

#include <memory>
#include <unordered_map>
#include <variant>

#include "assets/shader.hpp"
#include "assets/texture.hpp"
#include "assets/cubeMap.hpp"

using UniformDataTypes = std::variant<bool, int32_t, uint32_t, int32_t*, uint32_t*, float, glm::vec2, glm::ivec2, glm::vec3, glm::ivec3, glm::vec4, glm::ivec4, glm::mat4, std::shared_ptr<Texture>, std::shared_ptr<CubeMap>>;

/**	\struct UniformData
*	\brief Single instance of data, i.e. a data for a single uniform
* */
struct UniformData {
	UniformData() = default; //!< Default constructor with no data
	UniformData(GLenum p_type, int32_t p_size) : type(p_type), size(p_size) {}; //!< Constructor with data and type
	UniformDataTypes data{ 0 }; //!< Data storage using std::variant
	bool enabled{ false }; //!< Enabled in shader
	GLenum type{ 0 }; //!< Type of data held
	int32_t size{ -1 }; //!< Size of data held, 1 for single value n for array
};

// Dispatch table for uniform data, to be add to as required
namespace UniformConsts
{
	const std::unordered_map<GLenum, std::function<void(std::shared_ptr<Shader>, const std::string&, const UniformData&)>>  UDT =
	{
		{GL_FLOAT , [](std::shared_ptr<Shader> shader, const std::string& name, const UniformData& matInfo) {shader->uploadUniform<float>(name, (float)std::get<float>(matInfo.data)); } },
		{GL_FLOAT_VEC2 , [](std::shared_ptr<Shader> shader, const std::string& name, const UniformData& matInfo) {shader->uploadUniform<glm::vec2>(name, std::get<glm::vec2>(matInfo.data)); } },
		{GL_FLOAT_VEC3 , [](std::shared_ptr<Shader> shader, const std::string& name, const UniformData& matInfo) {shader->uploadUniform<glm::vec3>(name, std::get<glm::vec3>(matInfo.data)); } },
		{GL_FLOAT_VEC4 , [](std::shared_ptr<Shader> shader, const std::string& name, const UniformData& matInfo) {shader->uploadUniform<glm::vec4>(name, std::get<glm::vec4>(matInfo.data)); } },
		{GL_SAMPLER_2D , [](std::shared_ptr<Shader> shader, const std::string& name, const UniformData& matInfo) {
			if(matInfo.size == 1) shader->uploadUniform<int>(name, std::get<std::shared_ptr<Texture>>(matInfo.data)->getUnit()); 
			else shader->uploadUniform<int>(name, matInfo.size, std::get<int*>(matInfo.data));
			} 
		},
		{GL_FLOAT_MAT4 , [](std::shared_ptr<Shader> shader, const std::string& name, const UniformData& matInfo) {shader->uploadUniform<glm::mat4>(name, std::get<glm::mat4>(matInfo.data)); } },
		{GL_SAMPLER_CUBE , [](std::shared_ptr<Shader> shader, const std::string& name, const UniformData& matInfo) {shader->uploadUniform<int>(name, std::get<std::shared_ptr<CubeMap>>(matInfo.data)->getUnit()); } }
	};
}