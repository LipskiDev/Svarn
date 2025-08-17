#pragma once

#include <memory>
#include "Svarn/Renderer/VertexArray.h"
#include <glm/glm.hpp>

namespace Svarn {
    class RendererAPI {
        public:
        enum class API { None = 0, OpenGL = 1, Vulkan = 2 };

        public:
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

        inline static API GetAPI() { return s_API; }

        private:
        static API s_API;
    };
}  // namespace Svarn
