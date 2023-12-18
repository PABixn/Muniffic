#pragma once
#include "RendererAPI.h"
namespace eg {

	class RenderCommand
	{
	public:
		inline static void Init()
		{
			EG_PROFILE_FUNCTION();
			s_RendererAPI->Init();
		}

		inline static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_RendererAPI->SetViewPort(x, y, width, height); }

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		inline static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
		{
			s_RendererAPI->DrawLines(vertexArray, vertexCount);
		}

		inline static void SetLineThickness(float thickness)
		{
			s_RendererAPI->SetLineThickness(thickness);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};

}