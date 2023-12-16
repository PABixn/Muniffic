#include "egpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace eg
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}