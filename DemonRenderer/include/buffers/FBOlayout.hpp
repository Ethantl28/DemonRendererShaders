/** \file FBOlayout.hpp*/
#pragma once

#include <vector>

/** \enum AttachmentType
*	Possible attachments for a frame buffer */
enum class AttachmentType { Colour, ColourHDR, Depth, Stencil, DepthAndStencil };

using Attachment = std::pair<AttachmentType, bool>;

/** \class FBOLayout
*	\brief Provides the structure which describes an FBO */

class FBOLayout
{
public:
	FBOLayout() {}; //!< Default constructor
	/** Initializer list construtor i.e.
	*	FBOLayout layout = {
	*		{{AttachmentType::ColourHDR, true},
	*		{ AttachmentType::Depth, false }
	*	}};
	*/
	FBOLayout(const std::initializer_list<Attachment>& attachements, uint32_t stride = 0) : m_attachments(attachements) {}

	/** Add a single attachment to the layout */
	void addAttachment(AttachmentType type, bool sampled = false) {
		m_attachments.push_back(Attachment(type, sampled));
	}
	std::vector<Attachment>::iterator begin() { return m_attachments.begin(); } //!< Begin iterator
	std::vector<Attachment>::iterator end() { return m_attachments.end(); } //!< End iterator
	std::vector<Attachment>::const_iterator begin() const { return m_attachments.begin(); }//!< Begin const iterator
	std::vector<Attachment>::const_iterator end() const { return m_attachments.end(); } //!< End const iterator
private:
	std::vector<Attachment> m_attachments; //!< Container for attachments
};