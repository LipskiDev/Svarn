#pragma once

#include <Svarn/Renderer/RendererAPI.h>
#include <memory>
#include "Svarn/Renderer/VertexArray.h"

namespace Svarn {
    class OpenGLRendererAPI : public RendererAPI {
        public:
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;

        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
    };
}  // namespace Svarn
