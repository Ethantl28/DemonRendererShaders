#include "assets/shader.hpp"
#include "core/log.hpp"
#include <fstream>
#include <array>

Shader::Shader(const ShaderDescription& desc)
{
	// Compile source file
	switch (desc.type) {
	case ShaderType::rasterization :
		compileRateristion(desc);
		break;
	case ShaderType::tesselation:
		compileWithTesselation(desc);
		break;
	case ShaderType::geometry:
		compileWithGeometry(desc);
		break;
	case ShaderType::tessellationAndGeometry:
		compileWithGeometryAndTesselation(desc);
		break;
	case ShaderType::compute:
		compileCompute(desc);
		break;
	default:
		break;
	}

	// Populate uniform cache

	int32_t count = 0;
	glGetProgramiv(m_ID, GL_ACTIVE_UNIFORMS, &count);
	
	int32_t size = 0;
	GLenum type = 0;

	const size_t bufferSize = 32;
	std::array<char, bufferSize> nameBuffer;
	int32_t bufferLengthUsed = 0;

	for (size_t i = 0; i < count; i++) {
		glGetActiveUniform(m_ID, i, bufferSize, &bufferLengthUsed, &size, &type, nameBuffer.data());
		if (type == GL_IMAGE_2D) {
			int32_t bindPoint = 0;
			glGetUniformiv(m_ID, i, &bindPoint);
			UniformInfo data;
			data.location = glGetUniformLocation(m_ID, nameBuffer.data());
			data.type = type;
			data.size = size;
			m_uniformInfoCache[std::string(nameBuffer.data())] = data;
			m_imageBindingPoints[std::string(nameBuffer.data())] = bindPoint;

		//	spdlog::info("Uniform {} {} {} bind point:{}", std::string(nameBuffer.data()), size, type, bindPoint);
		}
		else {
			UniformInfo data;
			data.location = glGetUniformLocation(m_ID, nameBuffer.data());
			data.type = type;
			data.size = size;
			m_uniformInfoCache[std::string(nameBuffer.data())] = data;
		//	spdlog::info("Uniform {} {} {}", std::string(nameBuffer.data()), size, type);
		}
	}

	glGetProgramiv(m_ID, GL_ACTIVE_UNIFORM_BLOCKS, &count);
	for (size_t i = 0; i < count; i++) {
		int32_t count2 = 0;
		int32_t bindPoint = 0;
		int32_t offset = 0;
		int32_t dataSize = 0;
		int32_t uniformCount = 0;
		GLint uniformIndices[256];

		glGetActiveUniformBlockName(m_ID, i, bufferSize, &bufferLengthUsed, nameBuffer.data());
		glGetActiveUniformBlockiv(m_ID, i, GL_UNIFORM_BLOCK_BINDING, &bindPoint);
		glGetActiveUniformBlockiv(m_ID, i, GL_UNIFORM_BLOCK_DATA_SIZE, &dataSize);
		
		UBOLayout uboDesc(nameBuffer.data(), bindPoint, dataSize);

		glGetActiveUniformBlockiv(m_ID, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniformCount);
		glGetActiveUniformBlockiv(m_ID, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, uniformIndices);
		for(size_t j = 0; j < uniformCount; j++) {
			uint32_t index = uniformIndices[j];
			glGetActiveUniform(m_ID, index, bufferSize, &bufferLengthUsed, &size, &type, nameBuffer.data());
			glGetActiveUniformsiv(m_ID, 1, &index, GL_UNIFORM_OFFSET, &offset);
			uboDesc.addElement(UBOLayoutElement(nameBuffer.data(), type, offset));
			//spdlog::info("Uniform desc {} {} {}", std::string(nameBuffer.data()), type, offset);
		}
		
		m_UBOLayouts.push_back(uboDesc);
	}
}

void Shader::compileRateristion(const ShaderDescription& desc)
{
	GLuint vertexShader, fragmentShader;
	bool vertexCompiled, fragmentCompiled;

	vertexCompiled = compileShader(vertexShader, desc.vertexSrcPath, GL_VERTEX_SHADER);
	fragmentCompiled = compileShader(fragmentShader, desc.fragmentSrcPath, GL_FRAGMENT_SHADER);

	if (vertexCompiled && fragmentCompiled)
	{
		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShader);
		glAttachShader(m_ID, fragmentShader);
		glLinkProgram(m_ID);

		GLint isLinked = 0;
		glGetProgramiv(m_ID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ID, maxLength, &maxLength, &infoLog[0]);
			spdlog::error("Shader linking error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteProgram(m_ID);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			return;
		}

		glDetachShader(m_ID, vertexShader);
		glDetachShader(m_ID, fragmentShader);

		m_type = desc.type;
	}
}

void Shader::compileWithGeometry(const ShaderDescription& desc)
{
	GLuint vertexShader, geoShader, fragmentShader;
	bool vertexCompiled, geoCompiled, fragmentCompiled;

	vertexCompiled = compileShader(vertexShader, desc.vertexSrcPath, GL_VERTEX_SHADER);
	geoCompiled = compileShader(geoShader, desc.geometrySrcPath, GL_GEOMETRY_SHADER);
	fragmentCompiled = compileShader(fragmentShader, desc.fragmentSrcPath, GL_FRAGMENT_SHADER);

	if (vertexCompiled && geoCompiled && fragmentCompiled)
	{
		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShader);
		glAttachShader(m_ID, geoShader);
		glAttachShader(m_ID, fragmentShader);
		glLinkProgram(m_ID);

		GLint isLinked = 0;
		glGetProgramiv(m_ID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ID, maxLength, &maxLength, &infoLog[0]);
			spdlog::error("Shader linking error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteProgram(m_ID);
			glDeleteShader(vertexShader);
			glDeleteShader(geoShader);;
			glDeleteShader(fragmentShader);

			return;
		}

		// detach, cant delete otherwise
		glDetachShader(m_ID, vertexShader);
		glDetachShader(m_ID, geoShader);
		glDetachShader(m_ID, fragmentShader);
		// delete source, dont need these naymore
		glDeleteShader(vertexShader);
		glDeleteShader(geoShader);
		glDeleteShader(fragmentShader);

		m_type = desc.type;
	}
}

void Shader::compileWithTesselation(const ShaderDescription& desc)
{
	GLuint vertexShader, controlShader, evalShader, fragmentShader;
	bool vertexCompiled, controlCompiled, evalCompiled, fragmentCompiled;

	vertexCompiled = compileShader(vertexShader, desc.vertexSrcPath, GL_VERTEX_SHADER);
	controlCompiled = compileShader(controlShader, desc.controlSrcPath, GL_TESS_CONTROL_SHADER);
	evalCompiled = compileShader(evalShader, desc.evaluationSrcPath, GL_TESS_EVALUATION_SHADER);
	fragmentCompiled = compileShader(fragmentShader, desc.fragmentSrcPath, GL_FRAGMENT_SHADER);

	if (vertexCompiled && controlCompiled && evalCompiled && fragmentCompiled)
	{
		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShader);
		glAttachShader(m_ID, controlShader);
		glAttachShader(m_ID, evalShader);
		glAttachShader(m_ID, fragmentShader);
		glLinkProgram(m_ID);

		GLint isLinked = 0;
		glGetProgramiv(m_ID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ID, maxLength, &maxLength, &infoLog[0]);
			spdlog::error("Shader linking error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteProgram(m_ID);
			glDeleteShader(vertexShader);
			glDeleteShader(controlShader);
			glDeleteShader(evalShader);
			glDeleteShader(fragmentShader);

			return;
		}

		// detach, cant delete otherwise
		glDetachShader(m_ID, vertexShader);
		glDetachShader(m_ID, controlShader);
		glDetachShader(m_ID, evalShader);
		glDetachShader(m_ID, fragmentShader);
		// delete source, dont need these naymore
		glDeleteShader(vertexShader);
		glDeleteShader(controlShader);
		glDeleteShader(evalShader);
		glDeleteShader(fragmentShader);

		m_type = desc.type;
	}
}

void Shader::compileWithGeometryAndTesselation(const ShaderDescription& desc)
{
	GLuint vertexShader, controlShader, evalShader, geoShader, fragmentShader;
	bool vertexCompiled, controlCompiled, evalCompiled, geoCompiled, fragmentCompiled;

	vertexCompiled = compileShader(vertexShader, desc.vertexSrcPath, GL_VERTEX_SHADER);
	controlCompiled = compileShader(controlShader, desc.controlSrcPath, GL_TESS_CONTROL_SHADER);
	evalCompiled = compileShader(evalShader, desc.evaluationSrcPath, GL_TESS_EVALUATION_SHADER);
	geoCompiled = compileShader(geoShader, desc.geometrySrcPath, GL_GEOMETRY_SHADER);
	fragmentCompiled = compileShader(fragmentShader, desc.fragmentSrcPath, GL_FRAGMENT_SHADER);

	if (vertexCompiled && controlCompiled && evalCompiled && geoCompiled && fragmentCompiled)
	{
		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShader);
		glAttachShader(m_ID, controlShader);
		glAttachShader(m_ID, evalShader);
		glAttachShader(m_ID, geoShader);
		glAttachShader(m_ID, fragmentShader);
		glLinkProgram(m_ID);

		GLint isLinked = 0;
		glGetProgramiv(m_ID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ID, maxLength, &maxLength, &infoLog[0]);
			spdlog::error("Shader linking error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteProgram(m_ID);
			glDeleteShader(vertexShader);
			glDeleteShader(controlShader);
			glDeleteShader(evalShader);
			glDeleteShader(geoShader);
			glDeleteShader(fragmentShader);

			return;
		}

		// detach, cant delete otherwise
		glDetachShader(m_ID, vertexShader);
		glDetachShader(m_ID, controlShader);
		glDetachShader(m_ID, evalShader);
		glDetachShader(m_ID, geoShader);
		glDetachShader(m_ID, fragmentShader);
		// delete source, dont need these naymore
		glDeleteShader(vertexShader);
		glDeleteShader(controlShader);
		glDeleteShader(evalShader);
		glDeleteShader(geoShader);
		glDeleteShader(fragmentShader);

		m_type = desc.type;
	}

}

void Shader::compileCompute(const ShaderDescription& desc)
{
	GLuint computeShader;
	bool computeCompiled = compileShader(computeShader, desc.computeSrcPath, GL_COMPUTE_SHADER);

	if (computeCompiled) {
		m_ID = glCreateProgram();
		glAttachShader(m_ID, computeShader);
		glLinkProgram(m_ID);
		GLint isLinked = 0;
		glGetProgramiv(m_ID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ID, maxLength, &maxLength, &infoLog[0]);
			spdlog::error("Shader linking error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteProgram(m_ID);
			glDeleteShader(computeShader);


			return;
		}
		m_type = desc.type;
	}

}

bool Shader::compileShader(GLuint& shaderUnit, const std::filesystem::path& sourcePath, GLuint shaderType)
{
	bool result = false;

	std::fstream handle(sourcePath.c_str(), std::ios::in);
	if (!handle.is_open())
	{
		spdlog::error("Failed to open shader file: {0}", sourcePath.string());
		return result;
	}

	std::stringstream sStream;
	sStream << handle.rdbuf();
	handle.close();

	shaderUnit = glCreateShader(shaderType);
	std::string tmp = sStream.str();
	const GLchar* source = tmp.c_str();

	glShaderSource(shaderUnit, 1, &source, 0);
	glCompileShader(shaderUnit);

	GLint isCompiled = 0;
	glGetShaderiv(shaderUnit, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderUnit, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shaderUnit, maxLength, &maxLength, &infoLog[0]);
		spdlog::error("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

		glDeleteShader(shaderUnit);
	}
	else {
		result = true;
	}
	

	return result;
}

void Shader::dispatchUniform(uint32_t location, const glm::mat4& data) const
{
	glUniformMatrix4fv(location, 1, false, glm::value_ptr(data));	
}

void Shader::dispatchUniform(uint32_t location, const glm::vec3& data) const
{
	glUniform3fv(location, 1, glm::value_ptr(data));
}

void Shader::dispatchUniform(uint32_t location, const glm::vec4& data) const
{
	glUniform4fv(location, 1, glm::value_ptr(data));
}

void Shader::dispatchUniform(uint32_t location, int data) const
{
	glUniform1i(location, data);
}

void Shader::dispatchUniform(uint32_t location, size_t count, int* data) const
{
	glUniform1iv(location, count, data);
}

void Shader::dispatchUniform(uint32_t location, float data) const
{
	glUniform1f(location, data);
}

void Shader::dispatchUniform(uint32_t location, const glm::vec2& data) const
{
	glUniform2fv(location, 1, glm::value_ptr(data));
}
