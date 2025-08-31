#include <svpch.h>

#include <Svarn/Renderer/Renderer.h>
#include <memory>
#include "Svarn/Log.h"
#include "Svarn/Renderer/VertexArray.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Svarn {

    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_VP;
    glm::vec3 m_CameraPosition;

    std::vector<std::shared_ptr<Shader>> loadedShaders;

    void Renderer::BeginScene(const std::shared_ptr<Camera>& camera) {
        m_ViewMatrix = camera->GetViewMatrix();
        m_ProjectionMatrix = camera->GetProjection();
        m_VP = camera->GetViewProjection();
        m_CameraPosition = camera->GetPosition();
    }

    void Renderer::EndScene() {
        for (std::shared_ptr<Shader> shader : loadedShaders) {
            shader->m_ActiveTextures = 0;
        }
    }

    void Renderer::Clear() { RenderCommand::Clear(); }

    void Renderer::Submit(std::shared_ptr<VertexArray>& vertexArray, std::shared_ptr<Shader>& shader) {
        loadedShaders.push_back(shader);
        shader->Bind();
        vertexArray->Bind();
        shader->SetMat4("modelMatrix", glm::mat4(1.0));
        shader->SetMat4("viewMatrix", m_ViewMatrix);
        shader->SetMat4("projectionMatrix", m_ProjectionMatrix);
        shader->SetMat4("VP", m_VP);
        shader->SetVec3("u_CameraPosition", m_CameraPosition);

        RenderCommand::DrawIndexed(vertexArray);
    }

    void Renderer::Submit(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Shader>& shader) {
        loadedShaders.push_back(shader);
        shader->Bind();
        mesh->GetVertexArray()->Bind();
        shader->SetMat4("modelMatrix", glm::mat4(1.0));
        shader->SetMat4("viewMatrix", m_ViewMatrix);
        shader->SetMat4("projectionMatrix", m_ProjectionMatrix);
        shader->SetMat4("VP", m_VP);
        shader->SetVec3("u_CameraPosition", m_CameraPosition);

        RenderCommand::DrawIndexed(mesh->GetVertexArray());
    }

    void Renderer::Submit(std::shared_ptr<Model>& model, std::shared_ptr<Shader>& shader) {
        loadedShaders.push_back(shader);
        shader->Bind();
        auto meshes = model->GetAllMeshes();
        for (auto& mesh : meshes) {
            mesh->GetVertexArray()->Bind();
        }

        glm::mat4 modelMatrix(1.0f);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        shader->SetMat4("modelMatrix", modelMatrix);
        shader->SetMat4("viewMatrix", m_ViewMatrix);
        shader->SetMat4("projectionMatrix", m_ProjectionMatrix);
        shader->SetMat4("VP", m_VP);
        shader->SetVec3("u_CameraPosition", m_CameraPosition);

        for (auto& mesh : meshes) {
            RenderCommand::DrawIndexed(mesh->GetVertexArray());
        }
    }

    // TODO: implement shader library so that i can just call 'DrawToScreen(texture);' and the correct shader + quad is chosen and generated
    void Renderer::DrawToScreen(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Shader>& shader) {
        RenderCommand::DisableDepthTest();
        Submit(mesh, shader);
        RenderCommand::EnableDepthTest();
    }
}  // namespace Svarn
