/** \file material.hpp */
#pragma once

#include "rendering/uniformDataTypes.hpp"
#include "assets/shader.hpp"
#include "assets/texture.hpp"
#include "assets/cubeMap.hpp"

#include "core/log.hpp"

/**	\class Material
*	\brief Describes how a piece of geometry is shaded.
*	Holds a shader and all the data and setting assocaited with it.
*/

class Material
{
public:
	Material() = delete; //!< Deleted default constructor
	Material(std::shared_ptr<Shader> shader, const std::string& transformUniformName = "u_model"); //!< Constructor whcih takes a shader and optionally the name of the transform uniform in the shader
	std::shared_ptr<Shader> m_shader{ nullptr }; //!< The materials shader
	template<typename T>
	void setValue(const std::string& name, T value); //!< Set a value to be passed to the shader
	template<typename T>
	void setValue(const std::string& name, const T* value, size_t count); //"< setValue() overloaded for arrays - User modification
	void unsetValue(const std::string& name); //!< Disable a uniform, so it will not be passed to the shader
	void apply(); //!< BInd the shader and upload all data
	const std::string& getTransformUniformName() const { return m_transformUniformName; } //!< Returns the name of the transform uniform
	inline uint32_t getPrimitive() const { return m_primitive; } //!< Returns the rendering primitive
	void setPrimitive(uint32_t primitive) { m_primitive = primitive; } //!< Sets the rendering primitive
private:
	std::unordered_map <std::string, UniformData> dataCache; //!< Internal data storage for uniforms
	std::string m_transformUniformName{ "" };//!< The name of the transform uniform
	uint32_t m_primitive{ GL_TRIANGLES }; //!< Primitive for the draw call
};

template<typename T>
void Material::setValue(const std::string& name, T value) {
	auto it = dataCache.find(name);
	if(it != dataCache.end()) { 
		auto& data = dataCache[name];
		data.data = std::unwrap_reference_t<T>(value);
		data.enabled = true;
	}
	else { spdlog::error("Could not set material value: {}. Not found in shader data cache.", name); }
}

template<typename T>
void Material::setValue(const std::string& name, const T* value, size_t count) {
	auto it = dataCache.find(name);
	if (it != dataCache.end()) {
		auto& data = it->second;

		GLint location = glGetUniformLocation(m_shader->getID(), name.c_str());
		if constexpr (std::is_same_v<T, glm::vec4>) {
			glUniform4fv(location, static_cast<GLsizei>(count), reinterpret_cast<const GLfloat*>(value));
		}
		else {
			spdlog::error("Unsupported type for uniform array: {}", name);
		}

		data.enabled = true;
	}
	else {
		spdlog::error("Could not set material value: {}. Not found in shader data cache.", name);
	}
}
