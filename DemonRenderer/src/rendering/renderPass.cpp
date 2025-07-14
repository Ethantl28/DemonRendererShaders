#include "rendering/renderPass.hpp"

void RenderPass::parseScene()
{
	for (auto& actor : scene->m_actors)
	{
		if (actor.material) {
			if (actor.material) {
				for (auto& UBOlayout : actor.material->m_shader->m_UBOLayouts) {
					UBOmanager.addUBO(UBOlayout);
				}
			}
		}
	}
}
