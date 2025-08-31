#pragma once

#include <Svarn/Renderer/RenderCommand.h>
#include "Svarn/Renderer/Mesh.h"
#include "Svarn/Renderer/Model.h"
#include "Svarn/Renderer/Shader.h"
#include "Svarn/Scene/Camera.h"

namespace Svarn {

    class Renderer {
        public:
        static void BeginScene(const std::shared_ptr<Camera>& camera);
        static void EndScene();

        static void Submit(std::shared_ptr<VertexArray>& vertexArray, std::shared_ptr<Shader>& shader);
        static void Submit(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Shader>& shader);
        static void Submit(std::shared_ptr<Model>& model, std::shared_ptr<Shader>& shader);
        static void DrawToScreen(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Shader>& shader);

        static void Clear();

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
        static RendererAPIInfo GetAPIInfo() { return RenderCommand::GetAPIInfo(); }
    };
}  // namespace Svarn
