#include <svpch.h>
#include <Svarn/Renderer/Shader.h>

#include <glad/gl.h>
#include "Platform/OpenGL/OpenGLShader.h"
#include "Svarn/Core.h"
#include <Svarn/Renderer/Renderer.h>

namespace Svarn {

    Shader* Shader::Create() {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                SV_CORE_ASSERT(false, "RendererAPI::None currently not supported.");
            case RendererAPI::API::OpenGL:
                return new Svarn::OpenGLShader();
            case RendererAPI::API::Vulkan:
                SV_CORE_ASSERT(false, "RendererAPI::Vulkan currently not supported.");
        }

        SV_CORE_ASSERT(false, "Unknown RendererAPI.");
        return nullptr;
    };
}  // namespace Svarn
