#pragma once

#include <Svarn/Renderer/RenderCommand.h>
#include "Svarn/Renderer/Shader.h"
#include "Svarn/Scene/Camera.h"

namespace Svarn {

    class Renderer {
        public:
        static void BeginScene(const std::shared_ptr<Camera>& camera);
        static void EndScene();

        static void Submit(std::shared_ptr<VertexArray>& vertexArray, std::shared_ptr<Shader>& shader);

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
        static RendererAPIInfo GetAPIInfo() { return RenderCommand::GetAPIInfo(); }
    };
}  // namespace Svarn
