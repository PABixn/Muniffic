#pragma once
#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/VertexArray.h"
#include "SubTexture2D.h"
#include "Camera.h"
#include "UniformBuffer.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/EditorCamera.h"
#include "Engine/Scene/Components.h"
#include "Engine/Renderer/Font.h"


namespace eg {
	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void EndScene();
		static void Flush();
		static void Flush(int index);
		static void StartBatch();
		static void StartBatch(int index);

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, int layerIndex = 0);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int layerIndex = 0);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int layerIndex = 0);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int layerIndex = 0);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int layerIndex = 0);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int layerIndex = 0);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1, int layerIndex = 0);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1, int layerIndex = 0);
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1, int layerIndex = 0);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color, int layerIndex = 0);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color, int layerIndex = 0);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int layerIndex = 0);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int layerIndex = 0);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int layerIndex = 0);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int layerIndex = 0);

		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1, int layerIndex = 0);

		static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, int entityID = -1, int layerIndex = 0);

		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID = -1, int layerIndex = 0);
		static void DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID = -1, int layerIndex = 0);

		struct TextParams
		{
			glm::vec4 Color{ 1.0f };
			float Kerning = 0.0f;
			float LineSpacing = 0.0f;
		};
		
		static void DrawString(const std::string& text, Ref<Font> font, const glm::mat4& transform, const TextParams& textParams, int entityID = -1, int layerIndex = 0);
		static void DrawString(const std::string& text, const glm::mat4& transform, const TextComponent& component, int entityID = -1, int layerIndex = 0);

		static float GetLineThickness();
		static void SetLineThickness(float thickness);

		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID = -1, int layerIndex = 0);
		static void DrawSprite(const glm::mat4& transform, SpriteRendererSTComponent& src, int entityID = -1, int layerIndex = 0);

		// Stats
		struct Statistics {
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();
		// Layers
		struct QuadVertex {
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float TexIndex;
			float TilingFactor;

			// Editor-only
			int EntityID = -1;
		};

		struct CircleVertex {
			glm::vec3 WorldPosition;
			glm::vec3 LocalPosition;
			glm::vec4 Color;
			float Thickness;
			float Fade;

			// Editor-only
			int EntityID = -1;
		};

		struct LineVertex {
			glm::vec3 Position;
			glm::vec4 Color;

			// Editor-only
			int EntityID = -1;
		};

		struct TextVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;

			// TODO: bg color for outline/bg

			// Editor-only
			int EntityID = -1;
		};

		struct LayerInfo
		{
			bool isVisible = true;
			int index;
			std::string name = "Layer";

			Ref<VertexArray> QuadVertexArray;
			Ref<VertexBuffer> QuadVertexBuffer;
			Ref<Shader> QuadShader;
			Ref<Texture2D> WhiteTexture;

			Ref<VertexArray> CircleVertexArray;
			Ref<VertexBuffer> CircleVertexBuffer;
			Ref<Shader> CircleShader;

			Ref<VertexArray> LineVertexArray;
			Ref<VertexBuffer> LineVertexBuffer;
			Ref<Shader> LineShader;

			Ref<VertexArray> TextVertexArray;
			Ref<VertexBuffer> TextVertexBuffer;
			Ref<Shader> TextShader;

			uint32_t QuadIndexCount = 0;
			QuadVertex* QuadVertexBufferBase = nullptr;
			QuadVertex* QuadVertexBufferPtr = nullptr;

			uint32_t CircleIndexCount = 0;
			CircleVertex* CircleVertexBufferBase = nullptr;
			CircleVertex* CircleVertexBufferPtr = nullptr;

			uint32_t LineVertexCount = 0;
			LineVertex* LineVertexBufferBase = nullptr;
			LineVertex* LineVertexBufferPtr = nullptr;

			uint32_t TextIndexCount = 0;
			TextVertex* TextVertexBufferBase = nullptr;
			TextVertex* TextVertexBufferPtr = nullptr;

			void Create();
			void Delete();
			void Flush();
			void StartBatch();
		};
		static void CreateLayer();
		static void DeleteLayer(int index);
		static void SetLayerVisibility(int index, bool isVisible);
		static bool GetLayerVisibility(int index);
		static void SwitchLayerVisibility(int index);
		static void SetLayerName(int index, const std::string& name);
		static std::string GetLayerName(int index);

		static void MoveLayerUp(int index);
		static void MoveLayerDown(int index);



		struct Renderer2DData {
			static const uint32_t MaxQuads = 10000;
			static const uint32_t MaxVertices = MaxQuads * 4;
			static const uint32_t MaxIndices = MaxQuads * 6;
			static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps
			std::vector<Renderer2D::LayerInfo> layers;
			glm::mat4 ViewProjectionMatrix;
			float LineThickness = 2.0f;
			std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
			uint32_t TextureSlotIndex = 1; // 0 = white texture
			Ref<Texture2D> FontAtlasTexture;
			glm::vec4 QuadVertexPositions[4];
			Renderer2D::Statistics Stats;
			struct CameraData
			{
				glm::mat4 ViewProjection;
			};
			CameraData CameraBuffer;
			Ref<UniformBuffer> CameraUniformBuffer;
		};
		static Renderer2DData s_RendererData;
		static Renderer2DData GetRendererData() { return s_RendererData; }
		

		private:
			static void FlushAndReset();
			static void EndBatch();
			friend class LayersPanel;
			//static void NextBatch();
	};
}