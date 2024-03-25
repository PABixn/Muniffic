#pragma once

#include "Engine/Core/Core.h"


namespace eg {

	enum class FrameBufferTextureFormat {
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,
		//RGBA16F,
		//RGBA32F,

		// Depth/Stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FrameBufferTextureSpecification {
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat format)
			: TextureFormat(format) {}

		FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;
		//TODO: Filtering/WRapping
	};

	struct FrameBufferAttachmentSpecification {
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FrameBufferTextureSpecification> Attachments;
	};

	struct FrameBufferSpecification {
		unsigned int Width, Height;
		FrameBufferAttachmentSpecification Attachments;
		unsigned int Samples = 1;

		bool SwapChainTarget = false;
	};

	class FrameBuffer {
	public:
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual const FrameBufferSpecification& GetSpecification() const = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		/*void attachTexture(unsigned int texture, unsigned int attachment);
		void attachRenderBuffer(unsigned int renderBuffer, unsigned int attachment);

		void checkStatus();

		void clear();

		void setDrawBuffers(unsigned int count, unsigned int* buffers);

		void setReadBuffer(unsigned int buffer);*/

		//unsigned int getID() const { return m_RendererID; }
	};
}