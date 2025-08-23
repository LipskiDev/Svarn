#include <svpch.h>

#include <Svarn/Renderer/Renderer.h>
#include <memory>
#include "Svarn/Log.h"
#include "Svarn/Renderer/VertexArray.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Svarn {

    glm::mat4 m_VP;

    void Renderer::BeginScene(const std::shared_ptr<Camera>& camera) { m_VP = camera->GetViewProjection(); }

    void Renderer::EndScene() {}

    void Renderer::Submit(std::shared_ptr<VertexArray>& vertexArray, std::shared_ptr<Shader>& shader) {
        shader->Bind();
        vertexArray->Bind();
        shader->SetMat4("VP", m_VP);
        RenderCommand::DrawIndexed(vertexArray);
    }

    void Renderer::Submit(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Shader>& shader) {
        shader->Bind();
        mesh->GetVertexArray()->Bind();
        shader->SetMat4("VP", m_VP);
        RenderCommand::DrawIndexed(mesh->GetVertexArray());
    }

    void Renderer::Submit(std::shared_ptr<Model>& model, std::shared_ptr<Shader>& shader) {
        shader->Bind();
        auto meshes = model->GetAllMeshes();
        shader->SetMat4("VP", m_VP);
        glm::mat4 modelMatrix(1.0f);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        shader->SetMat4("M", modelMatrix);

        for (auto& mesh : meshes) {
            Submit(mesh, shader);
        }
    }
}  // namespace Svarn
