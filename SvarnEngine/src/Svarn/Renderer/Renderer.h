#pragma once

#include <Svarn/Renderer/RenderCommand.h>
#include "Svarn/Renderer/Framebuffer.h"
#include "Svarn/Renderer/Mesh.h"
#include "Svarn/Renderer/Model.h"
#include "Svarn/Renderer/Shader.h"
#include "Svarn/Scene/Camera.h"
#include "Svarn/Scene/DirectionalLight.h"
#include <Svarn/Math/Transform.h>

namespace Svarn {
    struct RenderObject {
        std::shared_ptr<VertexArray> vertexArray;
        Transform transform;
        Material material;

        RenderObject(std::shared_ptr<VertexArray> va, Transform t, Material mat = Material::New()) : transform(t), material(mat) {
            vertexArray = va;
            this->material = mat;
        }
    };

    class Renderer {
        public:
        void Init();
        void BeginScene(const std::shared_ptr<Camera>& camera);
        void EndScene();

        void Submit(const std::shared_ptr<VertexArray>& vertexArray, const Material& m, const Transform& t = Transform::Default());

        void Submit(const std::shared_ptr<Mesh>& mesh, const Transform& t = Transform::Default());
        void Submit(const std::shared_ptr<Model>& model, const Transform& t = Transform::Default());
        void Submit(const std::shared_ptr<DirectionalLight>& directionalLight);
        void DrawToScreen(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Shader>& shader);

        void RenderTerrain(bool shouldRenderTerrain);

        void Clear();

        inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
        RendererAPIInfo GetAPIInfo() { return RenderCommand::GetAPIInfo(); }

        private:
        std::vector<std::shared_ptr<Shader>> loadedShaders;
        std::vector<RenderObject> renderQueue;
        std::shared_ptr<DirectionalLight> m_DirectionalLight;

        std::shared_ptr<Shader> m_DepthShader;
        std::shared_ptr<Shader> m_PBRShader;
        std::shared_ptr<Shader> m_TerrainShader;

        // Terrain Renderer Variables
        bool m_ShouldRenderTerrain = false;

        std::shared_ptr<Mesh> m_PatchMesh;
        Material m_TerrainMaterial = Material::FromValues("Terrain Material", glm::vec3(1.0), 1.0, 0.0);

        // General Camera
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_VP;
        std::shared_ptr<Camera> m_Camera;

        glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(0.0f, 200.0f, 0.0f),  // light position, above the scene
                                                glm::vec3(0.0f, 0.0f, 0.0f),    // looking at the world origin
                                                glm::vec3(0.0f, 0.0f, 1.0f)     // up direction (so Y is "down light")
        );
        glm::mat4 depthProjectionMatrix = glm::ortho(-100.0f, 100.0f,  // left, right
                                                     -100.0f, 100.0f,  // bottom, top
                                                     0.1f, 200.0f      // near, far
        );
        // Shadow Mapping
        std::shared_ptr<Framebuffer> m_DepthMapLight;
    };

    SVARN_API Renderer& GetRenderer();
    SVARN_API const void* RendererAddress();

}  // namespace Svarn
