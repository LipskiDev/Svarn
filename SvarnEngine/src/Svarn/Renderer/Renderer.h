#pragma once

#include <Svarn/Renderer/RenderCommand.h>
#include "Svarn/Renderer/Mesh.h"
#include "Svarn/Renderer/Model.h"
#include "Svarn/Renderer/Shader.h"
#include "Svarn/Scene/Camera.h"
#include "Svarn/Scene/DirectionalLight.h"
#include <Svarn/Math/Transform.h>
#include <queue>

namespace Svarn {
    struct RenderObject {
        std::shared_ptr<VertexArray> vertexArray;
        Transform transform;
        std::shared_ptr<Shader> m_Shader;
        Material material;

        RenderObject(std::shared_ptr<VertexArray> va, Transform t, std::shared_ptr<Shader> shader, Material mat = Material::New())
            : transform(t), material(mat) {
            m_Shader = shader;
            vertexArray = va;
            this->material = mat;
        }
    };

    class Renderer {
        public:
        static void BeginScene(const std::shared_ptr<Camera>& camera);
        static void EndScene();

        static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const Material& m,
                           const Transform& t = Transform::Default());

        static void Submit(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Shader>& shader, const Transform& t = Transform::Default());
        static void Submit(const std::shared_ptr<Model>& model, const std::shared_ptr<Shader>& shader, const Transform& t = Transform::Default());
        static void Submit(const std::shared_ptr<DirectionalLight>& directionalLight);
        static void DrawToScreen(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Shader>& shader);

        static void Clear();

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
        static RendererAPIInfo GetAPIInfo() { return RenderCommand::GetAPIInfo(); }

        private:
        static std::vector<std::shared_ptr<Shader>> loadedShaders;
        static std::queue<RenderObject> renderQueue;
        static std::shared_ptr<DirectionalLight> m_DirectionalLight;

        static glm::mat4 m_ViewMatrix;
        static glm::mat4 m_ProjectionMatrix;
        static glm::mat4 m_VP;
        static glm::vec3 m_CameraPosition;
    };
}  // namespace Svarn
