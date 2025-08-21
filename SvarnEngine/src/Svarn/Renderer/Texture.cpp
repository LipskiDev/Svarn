#include <svpch.h>

#include <Svarn/Renderer/Texture.h>
#include <Svarn/Renderer/Renderer.h>
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Svarn {
    Texture* Texture::Create(std::string texturePath) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                SV_CORE_ASSERT(false, "RendererAPI::None currently not supported.");
            case RendererAPI::API::OpenGL:
                return new Svarn::OpenGLTexture(texturePath);
            case RendererAPI::API::Vulkan:
                SV_CORE_ASSERT(false, "RendererAPI::Vulkan currently not supported.");
        }

        SV_CORE_ASSERT(false, "Unknown RendererAPI.");
        return nullptr;
    }
}  // namespace Svarn
