#include <svpch.h>

#include <Svarn/Renderer/Renderer.h>
#include <memory>
#include "Svarn/Renderer/VertexArray.h"

namespace Svarn {

    void Renderer::BeginScene() {}

    void Renderer::EndScene() {}

    void Renderer::Submit(std::shared_ptr<VertexArray>& vertexArray) {
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}  // namespace Svarn
