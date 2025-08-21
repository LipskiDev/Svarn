#include <Svarn.h>

#include "Svarn/Renderer/Texture.h"
#include "Svarn/Layer.h"
#include "Svarn/Renderer/Buffer.h"
#include <Svarn/Scene/PerspectiveCamera.h>

using namespace Svarn;

class ExampleLayer : public Layer {
    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<VertexArray> m_VertexArray;

    std::shared_ptr<Shader> m_BlueShader;
    std::shared_ptr<VertexArray> m_SquareVA;

    std::shared_ptr<PerspectiveCamera> m_Camera;

    std::shared_ptr<Texture> m_MissingTexture;

    RendererAPIInfo apiInfo;

    public:
    ExampleLayer() : Layer("Example") {
        apiInfo = Renderer::GetAPIInfo();
        m_Camera.reset(new PerspectiveCamera(90, 16.0 / 9.0, 0.1f, 100));

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

        m_SquareVA.reset(VertexArray::Create());

        float squareVertices[5 * 4] = {
            // pos                 // uv
            -0.75f, -0.75f, 0.0f, 0.0f, 0.0f,  // bottom-left
            0.75f,  -0.75f, 0.0f, 1.0f, 0.0f,  // bottom-right
            0.75f,  0.75f,  0.0f, 1.0f, 1.0f,  // top-right
            -0.75f, 0.75f,  0.0f, 0.0f, 1.0f   // top-left
        };

        std::shared_ptr<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({{ShaderDataType::Float3, "a_Position"}, {ShaderDataType::Float2, "a_TexCoord"}});

        m_SquareVA->AddVertexBuffer(squareVB);
        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};

        std::shared_ptr<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);

        std::string vertexPath = "Sandbox/shaders/triangle.vs";
        std::string fragmentPath = "Sandbox/shaders/triangle.fs";

        m_Shader.reset(Shader::Create(vertexPath, fragmentPath));

        std::string blueVertexPath = "Sandbox/shaders/blue.vs";
        std::string blueFragmentPath = "Sandbox/shaders/blue.fs";

        m_BlueShader.reset(Shader::Create(blueVertexPath, blueFragmentPath));

        m_MissingTexture.reset(Texture::Create("Sandbox/assets/textures/missing.png"));
    }

    void OnUpdate(Timestep ts) override {
        m_Camera->OnUpdate(ts);

        if (Input::IsKeyPressed(SV_KEY_R)) {
            m_Shader->ReloadShader();
            m_BlueShader->ReloadShader();
        }

        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();

        m_MissingTexture->Bind(0);

        Renderer::BeginScene(m_Camera);

        Renderer::Submit(m_SquareVA, m_BlueShader);

        Renderer::Submit(m_VertexArray, m_Shader);

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
