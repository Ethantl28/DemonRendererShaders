#pragma once

#include "rendering/renderer.hpp"

void Renderer::addRenderPass(const RenderPass& pass)
{
	m_renderOrder.push_back(std::pair<PassType, size_t>(PassType::render, m_renderPasses.size()));
	m_renderPasses.push_back(pass);
}

void Renderer::addDepthPass(const DepthPass& depthPass)
{
	m_renderOrder.push_back(std::pair<PassType, size_t>(PassType::depth, m_depthPasses.size()));
	m_depthPasses.push_back(depthPass);
}

void Renderer::addComputePass(const ComputePass& pass)
{
	m_renderOrder.push_back(std::pair<PassType, size_t>(PassType::compute, m_computePasses.size()));
	m_computePasses.push_back(pass);
}


RenderPass& Renderer::getRenderPass(size_t index)
{
	auto& [passType, passIdx] = m_renderOrder[index];
	return m_renderPasses[passIdx];
}

DepthPass& Renderer::getDepthPass(size_t index)
{
	auto& [passType, passIdx] = m_renderOrder[index];
	return m_depthPasses[passIdx];
}

ComputePass& Renderer::getComputePass(size_t index)
{
	auto& [passType, passIdx] = m_renderOrder[index];
	return m_computePasses[passIdx];
}

void Renderer::render() const
{
	for (auto& [passType, idx] : m_renderOrder)
	{
		if (passType == PassType::render)
		{
			auto& renderPass = m_renderPasses[idx];

			renderPass.target->use();
			glViewport(renderPass.viewPort.x, renderPass.viewPort.y, renderPass.viewPort.width, renderPass.viewPort.height);

			if (renderPass.clearDepth && renderPass.clearColour) {
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}
			else if (renderPass.clearDepth) {
				glClear(GL_DEPTH_BUFFER_BIT);
			}
			else if (renderPass.clearColour) {
				glClear(GL_COLOR_BUFFER_BIT);
			}

			if (renderPass.drawInWireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			renderPass.UBOmanager.uploadCachedValues();

			// Iterate over all actors in the scene
			for (auto it = renderPass.scene->m_actors.begin(); it != renderPass.scene->m_actors.end(); ++it)
			{
				if (it->material) {
					it->material->apply();
					if (it->material->getTransformUniformName().length() > 0) it->material->m_shader->uploadUniform(it->material->getTransformUniformName(), it->transform);

					if (it->geometry) { // All geometry is indexed i.e. it uses IBOs
						glBindVertexArray(it->geometry->getID());
						glDrawElements(it->material->getPrimitive(), it->geometry->getDrawCount(), GL_UNSIGNED_INT, NULL);
					}
					else if (it->SSBOgeometry) {
						glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, it->SSBOgeometry->getID());  // binding point from material
						glDrawArrays(it->material->getPrimitive(), 0, it->SSBOgeometry->getElementCount());
					}
				}
			}


		}
		else if (passType == PassType::depth)
		{
			glCullFace(GL_FRONT);
			auto& depthPass = m_depthPasses[idx];

			depthPass.target->use();
			glViewport(depthPass.viewPort.x, depthPass.viewPort.y, depthPass.viewPort.width, depthPass.viewPort.height);

			if (depthPass.clearDepth) {
				glClear(GL_DEPTH_BUFFER_BIT);
			}

			depthPass.UBOmanager.uploadCachedValues();

			// Iterate over all actors in the scene
			for (auto it = depthPass.scene->m_actors.begin(); it != depthPass.scene->m_actors.end(); ++it)
			{
				if (it->depthMaterial) {
					it->depthMaterial->apply();
					if (it->depthMaterial->getTransformUniformName().length() > 0) it->depthMaterial->m_shader->uploadUniform(it->depthMaterial->getTransformUniformName(), it->transform);

					if (it->depthGeometry) { // All geometry is indexed i.e. it uses IBOs
						glBindVertexArray(it->depthGeometry->getID());
						glDrawElements(it->depthMaterial->getPrimitive(), it->depthGeometry->getDrawCount(), GL_UNSIGNED_INT, NULL);
					}
					else if (it->SSBOgeometry) {
						glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, it->SSBOgeometry->getID());  // binding point from material
						glDrawArrays(it->depthMaterial->getPrimitive(), 0, it->SSBOgeometry->getElementCount());
					}
				}
			}

			glCullFace(GL_BACK);
		}
		else if (passType == PassType::compute)
		{
			auto& computePass = m_computePasses[idx];

			// Bind images -- Note: Could consider an image unit manager if this becomes a hot path
			for (auto& img : computePass.images) {
				GLboolean layered = img.layer ? GL_TRUE : GL_FALSE;

				GLenum access = 0;
				switch (img.access)
				{
				case TextureAccess::ReadOnly:
					access = GL_READ_ONLY;
					break;
				case TextureAccess::WriteOnly:
					access = GL_WRITE_ONLY;
					break;
				case TextureAccess::ReadWrite:
					access = GL_READ_WRITE;
					break;
				}

				GLenum fmt = 0; // Ignoring depth for now
				if (img.texture->isHDR()) {
					if (img.texture->getChannels() == 3) fmt = GL_RGB16F;
					else fmt = GL_RGBA16F;
				}
				else {
					if (img.texture->getChannels() == 3) fmt = GL_RGB8;
					else fmt = GL_RGBA8;
				}

				// Need to deal with layers for cubemap
				glBindImageTexture(img.imageUnit, img.texture->getID(), img.mipLevel, layered, 0, access, fmt);
			}

			if (computePass.ssbo)
			{
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, computePass.ssbo->getID());
			}

			computePass.material->apply();
			auto& wg = computePass.workgroups;
			glDispatchCompute(wg.x, wg.y, wg.z);
			if (computePass.barrier != MemoryBarrier::None) glMemoryBarrier(static_cast<GLbitfield>(computePass.barrier));
		}
	}
}

