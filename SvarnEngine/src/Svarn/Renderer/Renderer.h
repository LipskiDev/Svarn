#pragma once

#include <Svarn/Renderer/RenderCommand.h>
#include "Svarn/Renderer/Mesh.h"
#include "Svarn/Renderer/Model.h"
#include "Svarn/Renderer/Shader.h"
#include "Svarn/Scene/Camera.h"
#include <Svarn/Math/Transform.h>

namespace Svarn {
    struct RenderObject {
        std::shared_ptr<VertexArray>& vertexArray;
        Transform transform;
        std::shared_ptr<Shader> m_Shader;

        RenderObject(std::shared_ptr<VertexArray> va, Transform t, std::shared_ptr<Shader> shader) : vertexArray(va), transform(t) {
            m_Shader = shader;
        }
    };

    class Renderer {
        public:
        static void BeginScene(const std::shared_ptr<Camera>& camera);
        static void EndScene();

        static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader,
                           const Transform& t = Transform::Default());

        static void Submit(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Shader>& shader, const Transform& t = Transform::Default());
        static void Submit(const std::shared_ptr<Model>& model, const std::shared_ptr<Shader>& shader, const Transform& t = Transform::Default());
        static void DrawToScreen(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Shader>& shader);

        static void Clear();

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
        static RendererAPIInfo GetAPIInfo() { return RenderCommand::GetAPIInfo(); }
    };
}  // namespace Svarn
