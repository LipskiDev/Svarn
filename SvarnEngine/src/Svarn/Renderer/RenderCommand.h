#pragma once

#include <Svarn/Renderer/RendererAPI.h>
#include "Svarn/Renderer/VertexArray.h"

namespace Svarn {
    class RenderCommand {
        public:
        inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }

        inline static void Clear() { s_RendererAPI->Clear(); }

        inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) { s_RendererAPI->DrawIndexed(vertexArray); }

        inline static void EnableDepthTest() { s_RendererAPI->EnableDepthTest(); }
        inline static void DisableDepthTest() { s_RendererAPI->DisableDepthTest(); }

        inline static RendererAPIInfo GetAPIInfo() { return s_RendererAPI->GetRendererAPIInfo(); }

        private:
        static RendererAPI* s_RendererAPI;
    };
}  // namespace Svarn
