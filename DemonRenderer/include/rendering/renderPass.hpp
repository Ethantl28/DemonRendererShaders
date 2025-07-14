#pragma once
#include "rendering/depthOnlyPass.hpp"

/**	\struct RenderPass
*	\brief A render pass which only performs rasterisation
*/
struct RenderPass
{
	UBOManager UBOmanager; //!< Holds all UBOs required for this pass
	Camera camera; //!< Camera for the pass
	std::shared_ptr<FBO> target; //!< Render target
	std::shared_ptr<Scene> scene; //!< Scene being rendered
	ViewPort viewPort; //!< Portion of the render target being rendered too
	bool clearColour{ true };//!< Should the colour buffer be cleared by this parse?
	bool clearDepth{ true }; //!< Should the depth buffer be cleared by this parse?
	bool drawInWireFrame{ false }; //!< Should this pass be rendered in wireFrame?
	bool writeToDepthBuffer{ true };


	void parseScene(); //!< Populate variable based on the scene
};

