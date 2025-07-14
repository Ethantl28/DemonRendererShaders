/** \file shader.hpp */
#pragma once 

#include <glad/gl.h>
#include <filesystem>
#include <unordered_map>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "buffers/UBOlayout.hpp"
#include "core/log.hpp"

/** \class ShaderType
*   \brief An enumerated class giving possible shaders which can be loaded. 
*/
enum class ShaderType
{
	uninitailised, rasterization, geometry, tesselation, tessellationAndGeometry, compute
};

/** \struct ShaderDescription
*   \brief Path to source files and shader type, used to load the shader.
*/
struct ShaderDescription
{
	std::filesystem::path vertexSrcPath = {};		//!< Vertex shader file path
	std::filesystem::path fragmentSrcPath = {};		//!< Fragment shader file path
	std::filesystem::path geometrySrcPath = {};		//!< Geometery shader file path
	std::filesystem::path controlSrcPath = {};		//!< Tessalation control shader file path
	std::filesystem::path evaluationSrcPath = {};	//!< Tessalation evaluation shader file path
	std::filesystem::path computeSrcPath = {};		//!< Compute shader file path
	ShaderType type = ShaderType::uninitailised;	//!< Type of shder must be set by user
};

/** \struct Uniforminfo
*   \brief Infomation about a unitform whcih needs to be pre-cached
*/
struct UniformInfo
{
	uint32_t location{ 0 }; //!< Location in shader
	GLenum type{ 0 }; //!< GL type (enumerated)
	uint32_t size{ 0 }; //!< Size i.e. 1 for single value n for an array
};

/** \class Shader
*	\brief A GPU side shader program
*	The class captures all type of OpenGL shader written in GLSL. 
*   A shader description must be provided to the construtor to initial the shader and compile the source code.
*/

class Shader
{
public:
	Shader() = delete; //!< No default constructor - a description MUST be provided
	explicit Shader(const ShaderDescription& desc); //!< Only contrustor, take a description
	inline ShaderType getType() const noexcept { return m_type; } //!< Returns the type of shader
	inline uint32_t getID() const noexcept { return m_ID; } //! Returns the ID of the shader on the device
	template<typename T>
	bool uploadUniform(const std::string& name, T data) const;  //!< Upload a uniform ot the shader.  The type provided must be given in UniformDataTypes and function given in UniformConsts.
	template<typename T>
	bool uploadUniform(const std::string& name, size_t count, T* data) const;  //!< Upload a uniform ot the shader.  The type provided must be given in UniformDataTypes and function given in UniformConsts.
	std::unordered_map<std::string, UniformInfo> m_uniformInfoCache; //!< Loacl storage of uniform locations
	std::unordered_map<std::string, uint32_t> m_imageBindingPoints; //!< Local storage of imge binding points
	std::vector<UBOLayout> m_UBOLayouts; //!< Collection of layouts for UBOs from shader source
private:
	void compileRateristion(const ShaderDescription& desc); //!< Compile a standard rasterisation program with frag and vertex shader source
	void compileWithGeometry(const ShaderDescription& desc); //!< Compile a geometry shader
	void compileWithTesselation(const ShaderDescription& desc); //!< Compile a tesselation shader
	void compileWithGeometryAndTesselation(const ShaderDescription& desc); //!< Compile a sahder which combines geometry and tessalation
	void compileCompute(const ShaderDescription& desc);//!< Compile a compute shader
	bool compileShader(GLuint& shaderUnit, const std::filesystem::path& sourcePath, GLuint shaderType); //!< Compile a single shader, no linking.
	// Need more of these for other types

	void dispatchUniform(uint32_t location, int data) const; //!< Upload an int uniform, this includes samplers
	void dispatchUniform(uint32_t location, size_t count, int* data) const; //!< Upload an array of int uniform, this includes samplers
	void dispatchUniform(uint32_t location, float data) const; //!< Upload an float uniform
	void dispatchUniform(uint32_t location, const glm::vec2& data) const; //!< Upload a vec2 uniform
	void dispatchUniform(uint32_t location, const glm::vec3& data) const; //!< Upload a vec3 uniform
	void dispatchUniform(uint32_t location, const glm::vec4& data) const; //!< Upload a vec4 uniform
	void dispatchUniform(uint32_t location, const glm::mat4& data) const; //!< Upload a mat4 uniform
private:
	ShaderType m_type{ShaderType::uninitailised}; //!< Type of the shader
	uint32_t m_ID; //!< Device side ID of the shader program

};


template<typename T>
inline bool Shader::uploadUniform(const std::string& name, T data) const
{
	bool result = false;
	auto it = m_uniformInfoCache.find(name);
	if (it != m_uniformInfoCache.end())
	{
		dispatchUniform(it->second.location, data);
		result = true;
	}
	return result;
}

template<typename T>
inline bool Shader::uploadUniform(const std::string& name, size_t count, T* data) const
{
	bool result = false;
	auto it = m_uniformInfoCache.find(name);
	if (it != m_uniformInfoCache.end())
	{
		dispatchUniform(it->second.location, count, data);
		result = true;
	}
	return result;
}
