/* \file graphicsContext.hpp*/
#pragma once

/**
*	\struct GraphicsContext
*	\brief Provides a templated init, swapbuffer allowing a range of graphics context to be implemented with a range of native windows types
*/

template<typename Native, typename Init, typename SwapBuffers>
struct GraphicsContext
{
	void init(Native* nativeWindow, bool hostingImGui) { Init{}(nativeWindow, hostingImGui); } //!< Initialise the graphics context
	void swapBuffers(Native* nativeWindow, bool hostingImGui) { SwapBuffers{}(nativeWindow, hostingImGui); } //!< Swapchain implementation
};


