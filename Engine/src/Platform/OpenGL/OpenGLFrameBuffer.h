#pragma once

#include "Engine/Renderer/FrameBuffer.h"

namespace eg {

	class OpenGLFrameBuffer: public	FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { 
			EG_CORE_ASSERT(index < m_ColorAttachments.size(), "Wrong Color"); 
			return m_ColorAttachments[index]; }

		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID = 0;
		FrameBufferSpecification m_Specification;

		std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecs;
		FrameBufferTextureSpecification m_DepthAttachmentSpec;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;
	};
}