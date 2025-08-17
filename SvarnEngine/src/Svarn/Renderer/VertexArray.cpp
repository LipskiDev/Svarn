#include <svpch.h>

#include <Svarn/Renderer/VertexArray.h>

#include <Platform/OpenGL/OpenGLVertexArray.h>

#include <Svarn/Renderer/Renderer.h>
#include "Svarn/Core.h"

namespace Svarn {

    VertexArray* VertexArray::Create() {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                SV_CORE_ASSERT(false, "RendererAPI::None not supported.");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return new OpenGLVertexArray();
            case RendererAPI::API::Vulkan:
                SV_CORE_ASSERT(false, "RendererAPI::Vulkan not supported.");
                return nullptr;
        }

        SV_CORE_ASSERT(false, "Unknown RendererAPI.");
        return nullptr;
    }

}  // namespace Svarn
