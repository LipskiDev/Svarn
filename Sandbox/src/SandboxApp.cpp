#include <Svarn.h>

#include "Svarn/Application.h"
#include "Svarn/Renderer/Mesh.h"
#include "Svarn/Renderer/Texture.h"
#include "Svarn/Layer.h"
#include "Svarn/Renderer/Buffer.h"
#include <Svarn/Scene/PerspectiveCamera.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace Svarn;

class ExampleLayer : public Layer {
    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<VertexArray> m_VertexArray;

    std::shared_ptr<Shader> m_BlueShader;
    std::shared_ptr<VertexArray> m_SquareVA;

    std::shared_ptr<PerspectiveCamera> m_Camera;

    std::shared_ptr<Texture> m_MissingTexture;

    std::shared_ptr<Mesh> m_QuadMesh;

    std::shared_ptr<Model> m_Cerberus;
    std::shared_ptr<Texture> m_CerberusAlbedo;

    std::shared_ptr<Texture> m_RedTexture;

    RendererAPIInfo apiInfo;

    public:
    ExampleLayer() : Layer("Example") {
        apiInfo = Renderer::GetAPIInfo();

        m_Camera.reset(new PerspectiveCamera(90, 16.0 / 9.0, 0.1f, 10000));

        m_VertexArray.reset(VertexArray::Create());

        float vertices[3 * 7] = {-0.5f, -0.5f, 0.0f, 0.8f, 1.2f, 0.8f, 1.0f, 0.5f, -0.5f, 0.0f, 0.2f,
                                 0.3f,  0.8f,  1.0f, 0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f,  1.0f};

        std::shared_ptr<VertexBuffer> vertexBuffer;
        vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        BufferLayout layout = {{ShaderDataType::Float3, "a_Position"}, {ShaderDataType::Float4, "a_Color"}};

        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = {0, 1, 2};
        std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        const std::vector<Vertex> squareVertices = {
            {glm::vec3(-0.75f, -0.75f, 0.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f)},
            {glm::vec3(0.75f, -0.75f, 0.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)},
            {glm::vec3(0.75f, 0.75f, 0.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f)},
            {glm::vec3(-0.75f, 0.75f, 0.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)},

        };

        std::vector<uint32_t> squareIndices = {0, 1, 2, 2, 3, 0};

        m_QuadMesh.reset(Mesh::Create(squareVertices, squareIndices));

        std::string vertexPath = "Sandbox/shaders/triangle.vs";
        std::string fragmentPath = "Sandbox/shaders/triangle.fs";

        m_Shader.reset(Shader::Create(vertexPath, fragmentPath));

        std::string blueVertexPath = "Sandbox/shaders/blue.vs";
        std::string blueFragmentPath = "Sandbox/shaders/blue.fs";

        m_BlueShader.reset(Shader::Create(blueVertexPath, blueFragmentPath));

        m_MissingTexture.reset(Texture::Create("Sandbox/assets/textures/missing.png"));
        m_MissingTexture->SetWrapping(TextureWrapping::ClampToEdge);

        m_Cerberus.reset(Model::Create("Sandbox/assets/models/Cerberus_LP.FBX"));
        m_CerberusAlbedo.reset(Texture::Create("Sandbox/assets/textures/Cerberus_A.tga"));

        TextureSpecification spec;
        spec.height = Application::Get().GetWindow().GetHeight();
        spec.width = Application::Get().GetWindow().GetWidth();
        m_RedTexture.reset(Texture::Create(spec));

        std::vector<uint8_t> pixels(4096 * 4096 * 4, 255);  // solid white
        for (uint32_t y = 0; y < spec.height; y++) {
            for (uint32_t x = 0; x < spec.width; x++) {
                bool checker = ((x / 32) % 2) ^ ((y / 32) % 2);  // 32px squares
                int idx = (y * spec.width + x) * 4;
                pixels[idx + 0] = checker ? 255 : 0;  // R
                pixels[idx + 1] = checker ? 255 : 0;  // G
                pixels[idx + 2] = checker ? 255 : 0;  // B
                pixels[idx + 3] = 255;                // A
            }
        }

        m_RedTexture->SetData(pixels.data(), 0, 0);
    }

    void OnUpdate(Timestep ts) override {
        m_Camera->OnUpdate(ts);

        if (Input::IsKeyPressed(SV_KEY_R)) {
            m_Shader->ReloadShader();
            m_BlueShader->ReloadShader();
        }

        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();

        Renderer::BeginScene(m_Camera);
        // m_CerberusAlbedo->Bind(0);

        m_RedTexture->Bind(0);
        Renderer::Submit(m_Cerberus, m_BlueShader);

        // Renderer::Submit(m_QuadMesh, m_BlueShader);
        // Renderer::Submit(m_VertexArray, m_Shader);

        Renderer::EndScene();
    }

    virtual void OnImGuiRender(Timestep ts) override {
        ImGui::Begin("Renderer");
        {
            ImGui::Text("API: %s", apiInfo.API.c_str());
            ImGui::Text("FPS: %d", static_cast<int>(1 / ts.GetSeconds()));
            ImGui::Text("Vendor: %s", apiInfo.Vendor.c_str());
            ImGui::Text("Renderer: %s", apiInfo.Renderer.c_str());
            ImGui::Text("Version: %s", apiInfo.Version.c_str());
        }
        ImGui::End();
    }

    void OnEvent(Svarn::Event& e) override { m_Camera->OnEvent(e); }
};

class Sandbox : public Svarn::Application {
    public:
    Sandbox() { PushLayer(new ExampleLayer()); }

    ~Sandbox() {}
};

Svarn::Application* Svarn::CreateApplication() {
    SV_CORE_TRACE("Svarn::CreateApplication was called");
    return new Sandbox();
}
