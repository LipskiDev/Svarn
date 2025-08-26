#include <svpch.h>

#include <Svarn/Renderer/Mesh.h>
#include "Svarn/Renderer/Buffer.h"
#include "Svarn/Renderer/VertexArray.h"

namespace Svarn {
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
        m_VAO.reset(VertexArray::Create());

        m_VBO.reset(VertexBuffer::Create(reinterpret_cast<const float*>(vertices.data()), vertices.size() * sizeof(Vertex)));

        m_VBO->SetLayout({{ShaderDataType::Float3, "a_Position"},
                          {ShaderDataType::Float3, "a_Normal"},
                          {ShaderDataType::Float2, "a_TexCoord"},
                          {ShaderDataType::Float4, "a_Tangent"},
                          {ShaderDataType::Float4, "a_Bitangent"}});
        m_VAO->AddVertexBuffer(m_VBO);

        m_IBO.reset(IndexBuffer::Create(indices.data(), indices.size()));
        m_VAO->SetIndexBuffer(m_IBO);

        m_IndexCount = indices.size();
    }

    Mesh* Mesh::Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) { return new Mesh(vertices, indices); }
}  // namespace Svarn
