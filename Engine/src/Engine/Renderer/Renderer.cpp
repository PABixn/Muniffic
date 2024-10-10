#include "egpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Engine/Renderer/Renderer2D.h"
#include "../Engine-Editor/src/Panels/ConsolePanel.h"

namespace eg {
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		EG_PROFILE_FUNCTION();
		RenderCommand::Init();
		Renderer2D::Init();
		ConsolePanel::Log("File: Renderer.cpp - Renderer Initialized", ConsolePanel::LogType::Info);
	}

	void Renderer::Shutdown()
	{
        EG_PROFILE_FUNCTION();
		ConsolePanel::Log("File: Renderer.cpp - Renderer Shutdown", ConsolePanel::LogType::Info);
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
        EG_PROFILE_FUNCTION();
		RenderCommand::SetViewPort(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
        EG_PROFILE_FUNCTION();
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjection();

	}

	void Renderer::EndScene()
	{
        EG_PROFILE_FUNCTION();
	}

	void Renderer::Submit(const Ref<Shader>& shader,const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
        EG_PROFILE_FUNCTION();
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}
