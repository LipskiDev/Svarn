#include <svpch.h>
#include <Svarn/Renderer/Buffer.h>
#include <Svarn/Renderer/Renderer.h>
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Svarn {

    VertexBuffer* VertexBuffer::Create(const float* vertices, uint32_t size) {
        switch (GetRenderer().GetAPI()) {
            case RendererAPI::API::None:
                SV_CORE_ASSERT(false, "RendererAPI::None currently not supported.");
            case RendererAPI::API::OpenGL:
                return new Svarn::OpenGLVertexBuffer(vertices, size);
            case RendererAPI::API::Vulkan:
                SV_CORE_ASSERT(false, "RendererAPI::Vulkan currently not supported.");
        }

        SV_CORE_ASSERT(false, "Unknown RendererAPI.");
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(const uint32_t* vertices, uint32_t count) {
        switch (GetRenderer().GetAPI()) {
            case RendererAPI::API::None:
                SV_CORE_ASSERT(false, "RendererAPI::None currently not supported.");
            case RendererAPI::API::OpenGL:
                return new Svarn::OpenGLIndexBuffer(vertices, count);
            case RendererAPI::API::Vulkan:
                SV_CORE_ASSERT(false, "RendererAPI::Vulkan currently not supported.");
        }

        SV_CORE_ASSERT(false, "Unknown RendererAPI.");
        return nullptr;
    }
}  // namespace Svarn
