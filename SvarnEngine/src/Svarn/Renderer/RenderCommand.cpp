#include <svpch.h>

#include <Svarn/Renderer/RenderCommand.h>
#include <Platform/OpenGL/OpenGLRendererAPI.h>

namespace Svarn {

    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

}
