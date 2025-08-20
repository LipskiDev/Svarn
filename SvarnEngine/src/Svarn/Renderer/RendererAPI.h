#pragma once

#include <memory>
#include "Svarn/Renderer/VertexArray.h"
#include <glm/glm.hpp>
#include <string>

namespace Svarn {

    struct RendererAPIInfo {
        std::string API;
        std::string Vendor;
        std::string Renderer;
        std::string Version;
    };

    class RendererAPI {
        public:
        enum class API { None = 0, OpenGL = 1, Vulkan = 2 };

        public:
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

        inline static API GetAPI() { return s_API; }
        virtual RendererAPIInfo GetRendererAPIInfo() = 0;

        private:
        static API s_API;
    };
}  // namespace Svarn
