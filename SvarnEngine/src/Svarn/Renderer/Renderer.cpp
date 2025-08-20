#include <svpch.h>

#include <Svarn/Renderer/Renderer.h>
#include <memory>
#include "Svarn/Renderer/VertexArray.h"

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
}  // namespace Svarn
