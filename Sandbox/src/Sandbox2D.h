#pragma once

#include "Engine.h"

class Sandbox2D : public eg::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(eg::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(eg::Event& e) override;
private:
	eg::OrthographicCameraController m_CameraController;

	// Temp
	eg::Ref<eg::VertexArray> m_SquareVA;
	eg::Ref<eg::Shader> m_FlatColorShader;

	eg::Ref<eg::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};