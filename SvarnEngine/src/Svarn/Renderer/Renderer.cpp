#include <svpch.h>

#include <Svarn/Renderer/Renderer.h>
#include <memory>
#include "Svarn/Log.h"
#include "Svarn/Renderer/VertexArray.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <queue>

namespace Svarn {

    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_VP;
    glm::vec3 m_CameraPosition;

    std::vector<std::shared_ptr<Shader>> loadedShaders;
    std::queue<RenderObject> renderQueue;

    void Renderer::BeginScene(const std::shared_ptr<Camera>& camera) {
        m_ViewMatrix = camera->GetViewMatrix();
        m_ProjectionMatrix = camera->GetProjection();
        m_VP = camera->GetViewProjection();
        m_CameraPosition = camera->GetPosition();
    }

    void Renderer::EndScene() {
        while (!renderQueue.empty()) {
            const RenderObject& obj = renderQueue.front();
            std::shared_ptr<Shader> shader = obj.m_Shader;
            shader->Bind();
            loadedShaders.push_back(shader);
            shader->SetMat4("VP", m_VP);
            // // shader->SetMat4("modelMatrix", obj.transform.GetModelMatrix());
            shader->SetMat4("viewMatrix", m_ViewMatrix);
            shader->SetMat4("projectionMatrix", m_ProjectionMatrix);

            obj.vertexArray->Bind();
            RenderCommand::DrawIndexed(obj.vertexArray);

            renderQueue.pop();
        }

        for (std::shared_ptr<Shader> shader : loadedShaders) {
            shader->m_ActiveTextures = 0;
        }
        loadedShaders.clear();
    }

    void Renderer::Clear() { RenderCommand::Clear(); }

    void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const Transform& t) {
        RenderObject obj(vertexArray, t, shader);
        renderQueue.push(obj);
    }

    void Renderer::Submit(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Shader>& shader, const Transform& t) {
        RenderObject obj(mesh->GetVertexArray(), t, shader);
        renderQueue.push(obj);
    }

    void Renderer::Submit(const std::shared_ptr<Model>& model, const std::shared_ptr<Shader>& shader, const Transform& t) {
        auto meshes = model->GetAllMeshes();

        for (auto& mesh : meshes) {
            RenderObject obj(mesh->GetVertexArray(), t, shader);
            renderQueue.push(obj);
        }
    }

    // TODO: implement shader library so that i can just call 'DrawToScreen(texture);' and the correct shader + quad is chosen and generated
    void Renderer::DrawToScreen(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Shader>& shader) {
        RenderCommand::DisableDepthTest();
        shader->Bind();
        RenderCommand::DrawIndexed(mesh->GetVertexArray());
        shader->Unbind();
        RenderCommand::EnableDepthTest();
    }
}  // namespace Svarn
