#include "egpch.h"
#include "Renderer2D.h"

#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/MSDFData.h"
#include "UniformBuffer.h"
#include "../Engine-Editor/src/Panels/LayersPanel.h"

#include <glm/gtc/matrix_transform.hpp>

namespace eg {
	

	//static Renderer2DData s_RendererData;


	void Renderer2D::LayerInfo::Create() {

		// SPRITES
		#pragma region sprites
		this->QuadVertexArray = VertexArray::Create();
		this->QuadVertexBuffer = VertexBuffer::Create(s_RendererData.MaxVertices * sizeof(QuadVertex));
		this->QuadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float, "a_TexIndex"},
			{ShaderDataType::Float, "a_TilingFactor"},
			{ShaderDataType::Int, "a_EntityID"}
			});
		this->QuadVertexArray->AddVertexBuffer(this->QuadVertexBuffer);
		this->QuadVertexBufferBase = new QuadVertex[s_RendererData.MaxVertices];
		uint32_t* quadIndices = new uint32_t[s_RendererData.MaxIndices];
		UINT32 offset = 0;
		for (uint32_t i = 0; i < s_RendererData.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		Ref<IndexBuffer> quadIB;
		quadIB = IndexBuffer::Create(quadIndices, s_RendererData.MaxIndices);
		this->QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;
		#pragma endregion
		
		// CIRCLES
		#pragma region circles

		this->CircleVertexArray = VertexArray::Create();

		this->CircleVertexBuffer = VertexBuffer::Create(s_RendererData.MaxVertices * sizeof(CircleVertex));
		this->CircleVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_WorldPosition" },
			{ ShaderDataType::Float3, "a_LocalPosition" },
			{ ShaderDataType::Float4, "a_Color"         },
			{ ShaderDataType::Float,  "a_Thickness"     },
			{ ShaderDataType::Float,  "a_Fade"          },
			{ ShaderDataType::Int,    "a_EntityID"      }
			});
		this->CircleVertexArray->AddVertexBuffer(this->CircleVertexBuffer);
		this->CircleVertexArray->SetIndexBuffer(quadIB); // Use quad IB
		this->CircleVertexBufferBase = new CircleVertex[s_RendererData.MaxVertices];
		#pragma endregion

		// LINES
		#pragma region lines
		this->LineVertexArray = VertexArray::Create();

		this->LineVertexBuffer = VertexBuffer::Create(s_RendererData.MaxVertices * sizeof(LineVertex));
		this->LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"      },
			{ ShaderDataType::Float4, "a_Color"         },
			{ ShaderDataType::Int,    "a_EntityID"      }
			});
		this->LineVertexArray->AddVertexBuffer(this->LineVertexBuffer);
		this->LineVertexBufferBase = new LineVertex[s_RendererData.MaxVertices];
		#pragma endregion

		// TEXT
		#pragma region text
		this->TextVertexArray = VertexArray::Create();

		this->TextVertexBuffer = VertexBuffer::Create(s_RendererData.MaxVertices * sizeof(TextVertex));
		this->TextVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"      },
			{ ShaderDataType::Float4, "a_Color"         },
			{ ShaderDataType::Float2, "a_TexCoord"      },
			{ ShaderDataType::Int,    "a_EntityID"      }
			});
		this->TextVertexArray->AddVertexBuffer(this->TextVertexBuffer);
		this->TextVertexArray->SetIndexBuffer(quadIB); // Use quad IB
		this->TextVertexBufferBase = new TextVertex[s_RendererData.MaxVertices];
		#pragma endregion


		this->WhiteTexture = Texture2D::Create(TextureSpecification());
		uint32_t whiteTextureData = 0xffffffff;
		this->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_RendererData.MaxTextureSlots];
		for (uint32_t i = 0; i < s_RendererData.MaxTextureSlots; i++)
			samplers[i] = i;

		this->QuadShader = Shader::Create("assets/shaders/Renderer2D_Quad.glsl");
		this->CircleShader = Shader::Create("assets/shaders/Renderer2D_Circle.glsl");
		this->LineShader = Shader::Create("assets/shaders/Renderer2D_Line.glsl");
		this->TextShader = Shader::Create("assets/shaders/Renderer2D_Text.glsl");

		s_RendererData.TextureSlots[0] = this->WhiteTexture;

		s_RendererData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_RendererData.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_RendererData.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_RendererData.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
		

	}

	void Renderer2D::LayerInfo::Delete() {
		this->QuadIndexCount = 0;
		this->QuadVertexBufferPtr = this->QuadVertexBufferBase;
	}

	void Renderer2D::LayerInfo::Flush() {
		EG_PROFILE_FUNCTION();
		if(!this->isVisible)
			return;

		if (this->QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)this->QuadVertexBufferPtr - (uint8_t*)this->QuadVertexBufferBase);
			this->QuadVertexBuffer->SetData(this->QuadVertexBufferBase, dataSize);

			for (uint32_t i = 0; i < s_RendererData.TextureSlotIndex; i++)
				s_RendererData.TextureSlots[i]->Bind(i);

			this->QuadShader->Bind();
			RenderCommand::DrawIndexed(this->QuadVertexArray, this->QuadIndexCount);
			s_RendererData.Stats.DrawCalls++;
		}

		if (this->CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)this->CircleVertexBufferPtr - (uint8_t*)this->CircleVertexBufferBase);
			this->CircleVertexBuffer->SetData(this->CircleVertexBufferBase, dataSize);

			this->CircleShader->Bind();
			RenderCommand::DrawIndexed(this->CircleVertexArray, this->CircleIndexCount);
			s_RendererData.Stats.DrawCalls++;

		}

		if (this->LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)this->LineVertexBufferPtr - (uint8_t*)this->LineVertexBufferBase);
			this->LineVertexBuffer->SetData(this->LineVertexBufferBase, dataSize);

			this->LineShader->Bind();
			RenderCommand::SetLineThickness(s_RendererData.LineThickness);
			RenderCommand::DrawLines(this->LineVertexArray, this->LineVertexCount);
			s_RendererData.Stats.DrawCalls++;

		}

		if (this->TextIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)this->TextVertexBufferPtr - (uint8_t*)this->TextVertexBufferBase);
			this->TextVertexBuffer->SetData(this->TextVertexBufferBase, dataSize);

			s_RendererData.FontAtlasTexture->Bind(0);

			this->TextShader->Bind();
			RenderCommand::DrawIndexed(this->TextVertexArray, this->TextIndexCount);
			s_RendererData.Stats.DrawCalls++;
		}
	}

	void Renderer2D::LayerInfo::StartBatch() {
		this->QuadIndexCount = 0;
		this->QuadVertexBufferPtr = this->QuadVertexBufferBase;

		this->CircleIndexCount = 0;
		this->CircleVertexBufferPtr = this->CircleVertexBufferBase;

		this->LineVertexCount = 0;
		this->LineVertexBufferPtr = this->LineVertexBufferBase;

		this->TextIndexCount = 0;
		this->TextVertexBufferPtr = this->TextVertexBufferBase;
	}



	
	void Renderer2D::CreateLayer() {
		EG_PROFILE_FUNCTION();
		s_RendererData.layers.push_back(Renderer2D::LayerInfo());
		s_RendererData.layers[s_RendererData.layers.size()].Create();
	}

	void Renderer2D::DeleteLayer(int index) {
		EG_PROFILE_FUNCTION();
		if (s_RendererData.layers.size() > 1) {
			s_RendererData.layers[index].Delete();
			s_RendererData.layers.erase(s_RendererData.layers.begin() + index);
		}
	}

	void Renderer2D::Init() {
		EG_PROFILE_FUNCTION();
		CreateLayer();
		s_RendererData.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer2D::Shutdown() {
		EG_PROFILE_FUNCTION();
		
		s_RendererData.TextureSlotIndex = 1;
	}


	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		EG_PROFILE_FUNCTION();

		s_RendererData.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_RendererData.CameraUniformBuffer->SetData(&s_RendererData.CameraBuffer, sizeof(Renderer2DData::CameraData));
		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		EG_PROFILE_FUNCTION();
		
		s_RendererData.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_RendererData.CameraUniformBuffer->SetData(&s_RendererData.CameraBuffer, sizeof(Renderer2DData::CameraData));
		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		EG_PROFILE_FUNCTION();
		s_RendererData.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
		s_RendererData.CameraUniformBuffer->SetData(&s_RendererData.CameraBuffer, sizeof(Renderer2DData::CameraData));
		StartBatch();

	}

	void Renderer2D::EndScene() {
		EG_PROFILE_FUNCTION();
		for (auto& layer : s_RendererData.layers) {
			uint32_t dataSize = (uint8_t*)layer.QuadVertexBufferPtr - (uint8_t*)layer.QuadVertexBufferBase;
			layer.QuadVertexBuffer->SetData(layer.QuadVertexBufferBase, dataSize);

		}
		Flush();
	}

	void Renderer2D::Flush() {
		for (int i = s_RendererData.layers.size(); i > 0;i--) {
			Flush(i);
		}
	}

	void Renderer2D::Flush(int index) {
		s_RendererData.layers[index].Flush();
	}

	void Renderer2D::StartBatch()
	{
		for (auto& layer : s_RendererData.layers) {
			layer.StartBatch();
		}
		s_RendererData.TextureSlotIndex = 1;
	}

	void Renderer2D::StartBatch(int index) {
		s_RendererData.layers[index].StartBatch();
	}

	void Renderer2D::FlushAndReset() {
		EG_PROFILE_FUNCTION();
		EndScene();
		StartBatch();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, int layerIndex) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color, layerIndex);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int layerIndex) {
		EG_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const glm::vec2 textureCoords[] = { {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f}, {0.0f,1.0f} };

		if (s_RendererData.layers[layerIndex].QuadIndexCount >= Renderer2DData::MaxIndices) 
			FlushAndReset();

		const float texIndex = 0.0f; // White Texture
		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (uint32_t i = 0; i < quadVertexCount; i++)
		{
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->Position = transform * s_RendererData.QuadVertexPositions[i];
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->Color = color;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TexIndex = texIndex;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr++;
		}

		s_RendererData.layers[layerIndex].QuadIndexCount += 6;

		s_RendererData.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int layerIndex) {
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor, layerIndex);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int layerIndex) {
		EG_PROFILE_FUNCTION();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor, layerIndex);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor, int layerIndex) {
		DrawQuad({ position.x, position.y, 0.0f }, size, subTexture, tilingFactor, tintColor, layerIndex);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor, int layerIndex) {
		EG_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, subTexture, tilingFactor, tintColor, layerIndex);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID, int layerIndex) {
		EG_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const glm::vec2 textureCoords[] = { {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f}, {0.0f,1.0f} };

		if (s_RendererData.layers[layerIndex].QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		const float texIndex = 0.0f; // White Texture
		const float tilingFactor = 1.0f;

		for (uint32_t i = 0; i < quadVertexCount; i++)
		{
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->Position = transform * s_RendererData.QuadVertexPositions[i];
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->Color = color;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TexIndex = texIndex;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->EntityID = entityID;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr++;
		}

		s_RendererData.layers[layerIndex].QuadIndexCount += 6;

		s_RendererData.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID, int layerIndex) {
		EG_PROFILE_FUNCTION();
		constexpr size_t quadVertexCount = 4;
		const glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };
		const glm::vec2 textureCoords[] = { {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f}, {0.0f,1.0f} };

		if (s_RendererData.layers[layerIndex].QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		float texIndex = 0.0f;
		for(uint32_t i = 1; i < s_RendererData.TextureSlotIndex; i++)
			if (*s_RendererData.TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}

		if (texIndex == 0.0f) {
			if(s_RendererData.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				FlushAndReset();

			texIndex = (float)s_RendererData.TextureSlotIndex;
			s_RendererData.TextureSlots[(int)s_RendererData.TextureSlotIndex] = texture;
			s_RendererData.TextureSlotIndex++;
		}

		for (uint32_t i = 0; i < quadVertexCount; i++)
		{
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->Position = transform * s_RendererData.QuadVertexPositions[i];
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->Color = color;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TexIndex = texIndex;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->EntityID = entityID;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr++;
		}

		s_RendererData.layers[layerIndex].QuadIndexCount += 6;

		s_RendererData.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor, int entityID, int layerIndex) {
		EG_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };
		const glm::vec2* textureCoords = subTexture->GetTexCoords();
		const Ref<Texture2D> texture = subTexture->GetTexture();

		if (s_RendererData.layers[layerIndex].QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		float texIndex = 0.0f;

		for (uint32_t i = 1; i < s_RendererData.TextureSlotIndex; i++)
			if (*s_RendererData.TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}

		if (texIndex == 0.0f) {
			texIndex = (float)s_RendererData.TextureSlotIndex;
			s_RendererData.TextureSlots[(int)s_RendererData.TextureSlotIndex] = texture;
			s_RendererData.TextureSlotIndex++;
		}

		for (uint32_t i = 0; i < quadVertexCount; i++)
		{
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->Position = transform * s_RendererData.QuadVertexPositions[i];
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->Color = color;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TexIndex = texIndex;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->EntityID = entityID;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr++;
		}

		s_RendererData.layers[layerIndex].QuadIndexCount += 6;

		s_RendererData.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color, int layerIndex) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color, layerIndex);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color, int layerIndex) {
		EG_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const glm::vec2 textureCoords[] = { {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f}, {0.0f,1.0f} };

		if (s_RendererData.layers[layerIndex].QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		const float texIndex = 0.0f; // White Texture
		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (uint32_t i = 0; i < quadVertexCount; i++)
		{
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->Position = transform * s_RendererData.QuadVertexPositions[i];
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->Color = color;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TexIndex = texIndex;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr++;
		}

		s_RendererData.layers[layerIndex].QuadIndexCount += 6;

		s_RendererData.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int layerIndex) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor, layerIndex);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int layerIndex)
	{
		EG_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };
		const glm::vec2 textureCoords[] = { {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f}, {0.0f,1.0f} };

		if (s_RendererData.layers[layerIndex].QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		float texIndex = 0.0f;

		for (uint32_t i = 1; i < s_RendererData.TextureSlotIndex; i++)
			if (*s_RendererData.TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}

		if (texIndex == 0.0f) {
			texIndex = (float)s_RendererData.TextureSlotIndex;
			s_RendererData.TextureSlots[(int)s_RendererData.TextureSlotIndex] = texture;
			s_RendererData.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (uint32_t i = 0; i < quadVertexCount; i++)
		{
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->Position = transform * s_RendererData.QuadVertexPositions[i];
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->Color = color;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TexIndex = texIndex;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr++;
		}

		s_RendererData.layers[layerIndex].QuadIndexCount += 6;

		s_RendererData.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor, int layerIndex) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subTexture, tilingFactor, tintColor, layerIndex);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor, int layerIndex) {
		constexpr size_t quadVertexCount = 4;
		const glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };
		const glm::vec2* textureCoords = subTexture->GetTexCoords();
		const Ref<Texture2D> texture = subTexture->GetTexture();

		if (s_RendererData.layers[layerIndex].QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		float texIndex = 0.0f;

		for (uint32_t i = 1; i < s_RendererData.TextureSlotIndex; i++)
			if (*s_RendererData.TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}

		if (texIndex == 0.0f) {
			texIndex = (float)s_RendererData.TextureSlotIndex;
			s_RendererData.TextureSlots[(int)s_RendererData.TextureSlotIndex] = texture;
			s_RendererData.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (uint32_t i = 0; i < quadVertexCount; i++)
		{
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->Position = transform * s_RendererData.QuadVertexPositions[i];
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->Color = color;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TexIndex = texIndex;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_RendererData.layers[layerIndex].QuadVertexBufferPtr++;
		}

		s_RendererData.layers[layerIndex].QuadIndexCount += 6;

		s_RendererData.Stats.QuadCount++;
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID, int layerIndex)
	{
		EG_PROFILE_FUNCTION();


		// TODO: implement for circles
		// if (s_RendererData.CircleIndexCount >= Renderer2DData::MaxIndices)
		// 	FlushAndReset();
		if (s_RendererData.layers[layerIndex].QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		const float texIndex = 0.0f; // White Texture
		const float tilingFactor = 1.0f;

		for (uint32_t i = 0; i < 4; i++)
		{
			s_RendererData.layers[layerIndex].CircleVertexBufferPtr->WorldPosition = transform * s_RendererData.QuadVertexPositions[i];
			s_RendererData.layers[layerIndex].CircleVertexBufferPtr->LocalPosition = s_RendererData.QuadVertexPositions[i] * 2.0f;
			s_RendererData.layers[layerIndex].CircleVertexBufferPtr->Color = color;
			s_RendererData.layers[layerIndex].CircleVertexBufferPtr->Fade = fade;
			s_RendererData.layers[layerIndex].CircleVertexBufferPtr->Thickness = thickness;
			s_RendererData.layers[layerIndex].CircleVertexBufferPtr->EntityID = entityID;
			s_RendererData.layers[layerIndex].CircleVertexBufferPtr++;
		}

		s_RendererData.layers[layerIndex].CircleIndexCount += 6;

		s_RendererData.Stats.QuadCount++;
	}

	void Renderer2D::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, int entityID, int layerIndex)
	{
		s_RendererData.layers[layerIndex].LineVertexBufferPtr->Position = start;
		s_RendererData.layers[layerIndex].LineVertexBufferPtr->Color = color;
		s_RendererData.layers[layerIndex].LineVertexBufferPtr->EntityID = entityID;
		s_RendererData.layers[layerIndex].LineVertexBufferPtr++;

		s_RendererData.layers[layerIndex].LineVertexBufferPtr->Position = end;
		s_RendererData.layers[layerIndex].LineVertexBufferPtr->Color = color;
		s_RendererData.layers[layerIndex].LineVertexBufferPtr->EntityID = entityID;
		s_RendererData.layers[layerIndex].LineVertexBufferPtr++;

		s_RendererData.layers[layerIndex].LineVertexCount += 2;
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID, int layerIndex)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color, entityID, layerIndex);
		DrawLine(p1, p2, color, entityID, layerIndex);
		DrawLine(p2, p3, color, entityID, layerIndex);
		DrawLine(p3, p0, color, entityID, layerIndex);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID, int layerIndex)
	{

		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
		{
			lineVertices[i] = transform * s_RendererData.QuadVertexPositions[i];
		}

		DrawLine(lineVertices[0], lineVertices[1], color, entityID, layerIndex);
		DrawLine(lineVertices[1], lineVertices[2], color, entityID, layerIndex);
		DrawLine(lineVertices[2], lineVertices[3], color, entityID, layerIndex);
		DrawLine(lineVertices[3], lineVertices[0], color, entityID, layerIndex);
	}

	void Renderer2D::DrawString(const std::string& text, Ref<Font> font, const glm::mat4& transform, const TextParams& textParams, int entityID, int layerIndex)
	{
		const auto& fontGeometry = font->GetData()->FontGeometry;
		const auto& metrics = fontGeometry.getMetrics();
		Ref<Texture2D> atlas = font->GetAtlasTexture();

		s_RendererData.FontAtlasTexture = atlas;

		double x = 0.0;
		double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
		double y = 0.0;
		const float spaceGlyphAdvance = fontGeometry.getGlyph(' ')->getAdvance();

		for (size_t i = 0; i < text.size(); i++)
		{

			char character = text[i];

			if (character == '\n')
			{
				x = 0.0;
				y -= fsScale * metrics.lineHeight + textParams.LineSpacing;
				continue;
			}

			if (character == ' ')
			{
				float advance = spaceGlyphAdvance;
				if (i < text.size() - 1)
				{
					char nextCharacter = text[i + 1];
					double dAdvance;
					fontGeometry.getAdvance(dAdvance, character, nextCharacter);
					advance = (float)dAdvance;
				}

				x += fsScale * advance + textParams.Kerning;
				continue;
			}
			if (character == '\t')
			{
				x += fsScale * (metrics.ascenderY - metrics.descenderY) * 4;
				continue;
			}

			auto glyph = fontGeometry.getGlyph(character);
			if (!glyph)
				glyph = fontGeometry.getGlyph('?');
			if (!glyph)
				return;

			double al, ab, ar, at;
			glyph->getQuadAtlasBounds(al, ab, ar, at);
			glm::vec2 texCoordMin = { (float)al, (float)ab };
			glm::vec2 texCoordMax = { (float)ar, (float)at };

			double pl, pb, pr, pt;
			glyph->getQuadPlaneBounds(pl, pb, pr, pt);
			glm::vec2 quadMin = { (float)pl, (float)pb };
			glm::vec2 quadMax = { (float)pr, (float)pt };

			quadMin *= fsScale, quadMax *= fsScale;
			quadMin += glm::vec2(x, y);
			quadMax += glm::vec2(x, y);

			float texelWidth = 1.0f / atlas->GetWidth();
			float texelHeight = 1.0f / atlas->GetHeight();
			texCoordMin *= glm::vec2(texelWidth, texelHeight);
			texCoordMax *= glm::vec2(texelWidth, texelHeight);

			//render here
			s_RendererData.layers[layerIndex].TextVertexBufferPtr->Position = transform * glm::vec4(quadMin.x, quadMin.y, 0.0f, 1.0f);
			s_RendererData.layers[layerIndex].TextVertexBufferPtr->Color = textParams.Color;
			s_RendererData.layers[layerIndex].TextVertexBufferPtr->TexCoord = texCoordMin;
			s_RendererData.layers[layerIndex].TextVertexBufferPtr->EntityID = entityID;
			s_RendererData.layers[layerIndex].TextVertexBufferPtr++;

			s_RendererData.layers[layerIndex].TextVertexBufferPtr->Position = transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f);
			s_RendererData.layers[layerIndex].TextVertexBufferPtr->Color = textParams.Color;
			s_RendererData.layers[layerIndex].TextVertexBufferPtr->TexCoord = { texCoordMin.x, texCoordMax.y };
			s_RendererData.layers[layerIndex].TextVertexBufferPtr->EntityID = entityID;
			s_RendererData.layers[layerIndex].TextVertexBufferPtr++;

			s_RendererData.layers[layerIndex].TextVertexBufferPtr->Position = transform * glm::vec4(quadMax.x, quadMax.y, 0.0f, 1.0f);
			s_RendererData.layers[layerIndex].TextVertexBufferPtr->Color = textParams.Color;
			s_RendererData.layers[layerIndex].TextVertexBufferPtr->TexCoord = texCoordMax;
			s_RendererData.layers[layerIndex].TextVertexBufferPtr->EntityID = entityID;
			s_RendererData.layers[layerIndex].TextVertexBufferPtr++;

			s_RendererData.layers[layerIndex].TextVertexBufferPtr->Position = transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f);
			s_RendererData.layers[layerIndex].TextVertexBufferPtr->Color = textParams.Color;
			s_RendererData.layers[layerIndex].TextVertexBufferPtr->TexCoord = { texCoordMax.x, texCoordMin.y };
			s_RendererData.layers[layerIndex].TextVertexBufferPtr->EntityID = entityID;
			s_RendererData.layers[layerIndex].TextVertexBufferPtr++;

			s_RendererData.layers[layerIndex].TextIndexCount += 6;

			s_RendererData.Stats.QuadCount++;

			if (i < text.size() - 1)
			{
				double advance = glyph->getAdvance();
				char nextCharacter = text[i + 1];
				fontGeometry.getAdvance(advance, character, nextCharacter);
				x += fsScale * advance + textParams.Kerning;
			}
		}
	}

	void Renderer2D::DrawString(const std::string& string, const glm::mat4& transform, const TextComponent& component, int entityID, int layerIndex)
	{
		DrawString(string, component.FontAsset, transform, { component.Color, component.Kerning, component.LineSpacing }, entityID, layerIndex);
	}

	float Renderer2D::GetLineThickness()
	{
		return s_RendererData.LineThickness;
	}

	void Renderer2D::SetLineThickness(float thickness)
	{
		s_RendererData.LineThickness = thickness;
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID, int layerIndex)
	{
		if(src.Texture)
			DrawQuad(transform, src.Texture, src.TilingFactor, src.Color, entityID, layerIndex);
		else
			DrawQuad(transform, src.Color, entityID, layerIndex);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererSTComponent& src, int entityID, int layerIndex)
	{
		if(src.SubTexture->GetTexture())
			DrawQuad(transform, src.SubTexture, src.TilingFactor, src.Color, entityID, layerIndex);
		else
			DrawQuad(transform, src.Color, entityID, layerIndex);
	}

	void Renderer2D::ResetStats() {
		memset(&s_RendererData.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats() {
		return s_RendererData.Stats;
	}

	void Renderer2D::SetLayerName(int index, const std::string& name) {
		s_RendererData.layers[index].name = name;
	}
	std::string Renderer2D::GetLayerName(int index) {
		return s_RendererData.layers[index].name;
	}
	void Renderer2D::SetLayerVisibility(int index, bool isVisible) {
		s_RendererData.layers[index].isVisible = isVisible;
	}
	bool Renderer2D::GetLayerVisibility(int index) {
		return s_RendererData.layers[index].isVisible;
	}
	void Renderer2D::SwitchLayerVisibility(int index) {
		s_RendererData.layers[index].isVisible = !s_RendererData.layers[index].isVisible;
	}


	void Renderer2D::MoveLayerDown(int index) {
		if (index < s_RendererData.layers.size() - 1) {
			std::swap(s_RendererData.layers[index], s_RendererData.layers[index + 1]);
		}
	}
	void Renderer2D::MoveLayerUp(int index) {
		if (index > 0) {
			std::swap(s_RendererData.layers[index], s_RendererData.layers[index - 1]);
		}
	}

}