#pragma once

#include <Svarn/Renderer/RendererAPI.h>
#include "Svarn/Renderer/VertexArray.h"

namespace Svarn {
    class RenderCommand {
        public:
        inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }

        inline static void Clear() { s_RendererAPI->Clear(); }

        inline static void DrawIndexed(std::shared_ptr<VertexArray>& vertexArray) { s_RendererAPI->DrawIndexed(vertexArray); }

        private:
        static RendererAPI* s_RendererAPI;
    };
}  // namespace Svarn
