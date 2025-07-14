#include "buffers/UBOmanager.hpp"

bool UBOManager::addUBO(const UBOLayout& layout)
{
	bool result = false;
	if (layout.getBindingPoint() < m_UBOs.size())
	{
		bool sameUBO = m_UBOs[layout.getBindingPoint()]->getLayout() == layout;
		if (!sameUBO) result = false;
		// Same UBO, we've already added it!
	}
	else
	{
		m_UBOs.push_back(std::make_shared<UBO>(layout));
		m_UBOMap[layout.getBlockName()] = m_UBOs.size() - 1;
	}
	return result;
}

void UBOManager::uploadCachedValues() const
{
	for (auto& dataPair : m_UBODataCache)
	{
		auto& blockName = dataPair.first;
		auto& uniformName = dataPair.second.first;
		auto& info = dataPair.second.second;

		switch (info.type)
		{
		case GL_FLOAT:
			{
			float _float = std::get<float>(info.data);
			uploadUniform(blockName, uniformName, &_float);
			}
			break;
		case GL_FLOAT_VEC3:
			glm::vec3 vec3 = std::get<glm::vec3>(info.data);
			uploadUniform(blockName, uniformName, glm::value_ptr(vec3));
			break;
		case GL_FLOAT_MAT4:
			glm::mat4 mat4 = std::get<glm::mat4>(info.data);
			uploadUniform(blockName, uniformName, glm::value_ptr(mat4));
			break;
		}
	}
}
