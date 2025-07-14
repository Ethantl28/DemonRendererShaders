#include "rendering/depthOnlyPass.hpp"

void DepthPass::parseScene()
{
	for (auto& actor : scene->m_actors)
	{
		if (actor.material) {
			if (actor.depthMaterial) {
				for (auto& UBOlayout : actor.depthMaterial->m_shader->m_UBOLayouts) {
					UBOmanager.addUBO(UBOlayout);
				}
			}
		}
	}
}
