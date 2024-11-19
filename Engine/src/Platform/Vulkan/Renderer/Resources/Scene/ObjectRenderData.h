#pragma once
#include <vector>
namespace eg {
	class VulkanVertexBuffer;
	class VulkanIndexBuffer;
	class VulkanShaderStorageBuffer;
	enum class RenderUpdate : uint32_t {
		None = 0,
		Created = 1 << 0,
		VerticesSizeChanged = 1 << 1,
		MatrixValueChanged = 1 << 2
	} ;
	inline RenderUpdate operator|(RenderUpdate a, RenderUpdate b) {
		return static_cast<RenderUpdate>(static_cast<std::underlying_type<RenderUpdate>::type>(a) |
			static_cast<std::underlying_type<RenderUpdate>::type>(b));
	}

	inline RenderUpdate operator&(RenderUpdate a, RenderUpdate b) {
		return static_cast<RenderUpdate>(static_cast<std::underlying_type<RenderUpdate>::type>(a) &
			static_cast<std::underlying_type<RenderUpdate>::type>(b));
	}

	inline RenderUpdate& operator|=(RenderUpdate& a, RenderUpdate b) {
		a = a | b;
		return a;
	}

	inline RenderUpdate& operator&=(RenderUpdate& a, RenderUpdate b) {
		a = a & b;
		return a;
	}
	
	inline RenderUpdate operator~(RenderUpdate a) {
		return static_cast<RenderUpdate>(~static_cast<std::underlying_type<RenderUpdate>::type>(a));
	}

	inline bool to_bool(RenderUpdate a) {
		return a != RenderUpdate::None;
	}

	struct ObjectRenderData {
		RenderUpdate m_Update;
		uint32_t m_VerticesCount;
		uint32_t m_IndicesCount;

		VulkanVertexBuffer* m_VertexBuffer;
		uint64_t m_VertexBufferOffset;
		VulkanIndexBuffer* m_IndexBuffer;
		uint64_t m_IndexBufferOffset;
		VulkanShaderStorageBuffer* m_MatricesBuffer;
		uint64_t m_MatricesBufferOffset;

	};

}