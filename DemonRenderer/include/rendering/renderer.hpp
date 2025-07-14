/** \file renderer.hpp */
#pragma once

#include "rendering/renderPass.hpp"
#include "rendering/depthOnlyPass.hpp"
#include "rendering/computePass.hpp"

/**	\class Renderer
*	\brief Holds and executes a series of render passes
*/

class Renderer
{
public:
	Renderer() = default; //!< Default constructor
	void addRenderPass(const RenderPass& renderPass); //!< Add a render pass
	void addDepthPass(const DepthPass& renderPass); //!< Add a depth only pass
	void addComputePass(const ComputePass& renderPass);  //!< Add a compute pass
	RenderPass& getRenderPass(size_t index); //!< Get a render pass
	DepthPass& getDepthPass(size_t index);  //!< Get a depth only pass
	ComputePass& getComputePass(size_t index);  //!< Get a compute pass
	size_t [[nodiscard]] getRenderPassCount() noexcept { return m_renderPasses.size(); } //!< Returns number of renderpasses
	size_t [[nodiscard]] getDepthPassCount() noexcept { return m_depthPasses.size(); } //!< Returns number of renderpasses
	size_t [[nodiscard]] getComputePassCount() noexcept { return m_computePasses.size(); } //!< Returns number of renderpasses
	void render() const; //!< Execute all render passes
private:
	/** \enum PassType 
	*	Type of render pass
	*/
	enum class PassType {compute, depth, render}; 
	std::vector<RenderPass> m_renderPasses; //!< Internal storage for render passes
	std::vector<DepthPass> m_depthPasses; //!< Internal storage for depth only passes
	std::vector<ComputePass> m_computePasses; //!< Internal storage for compute passes
	std::vector<std::pair<PassType, size_t>> m_renderOrder; //!< Internal storage or order of passes, similar to a sparse set
};