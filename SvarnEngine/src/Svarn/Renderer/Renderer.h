#pragma once

#include <Svarn/Renderer/RenderCommand.h>

namespace Svarn {

    class Renderer {
        public:
        static void BeginScene();
        static void EndScene();

        static void Submit(std::shared_ptr<VertexArray>& vertexArray);

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    };
}  // namespace Svarn
