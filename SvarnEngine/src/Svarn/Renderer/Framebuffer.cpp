#include <svpch.h>

#include <Svarn/Renderer/Framebuffer.h>
#include <Svarn/Renderer/Renderer.h>
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Svarn {
    Framebuffer* Framebuffer::Create(FramebufferSpecification spec) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                SV_CORE_ASSERT(false, "RendererAPI::None currently not supported.");
            case RendererAPI::API::OpenGL:
                return new OpenGLFramebuffer(spec);
            case RendererAPI::API::Vulkan:
                SV_CORE_ASSERT(false, "RendererAPI::Vulkan currently not supported.");
        }

        SV_CORE_ASSERT(false, "Unknown RendererAPI.");
        return nullptr;
    }

}  // namespace Svarn
