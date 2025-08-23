#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Svarn/Renderer/Buffer.h"
#include "Svarn/Renderer/VertexArray.h"

namespace Svarn {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    class Mesh {
        public:
        static Mesh* Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

        std::shared_ptr<VertexArray> GetVertexArray() const { return m_VAO; }

        private:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

        std::shared_ptr<IndexBuffer> m_IBO;
        std::shared_ptr<VertexBuffer> m_VBO;
        std::shared_ptr<VertexArray> m_VAO;
        uint32_t m_IndexCount = 0;
    };
}  // namespace Svarn
