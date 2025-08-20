#include <Svarn.h>

#include "Svarn/Layer.h"
#include <Svarn/Scene/PerspectiveCamera.h>

using namespace Svarn;

class ExampleLayer : public Layer {
    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<VertexArray> m_VertexArray;

    std::shared_ptr<Shader> m_BlueShader;
    std::shared_ptr<VertexArray> m_SquareVA;

    std::shared_ptr<PerspectiveCamera> m_Camera;

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

        float squareVertices[3 * 4] = {-0.75f, -0.75f, 0.0f, 0.75f, -0.75f, 0.0f, 0.75f, 0.75f, 0.0f, -0.75f, 0.75f, 0.0f};
        std::shared_ptr<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({{ShaderDataType::Float3, "a_Position"}});

        m_SquareVA->AddVertexBuffer(squareVB);
        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};

        std::shared_ptr<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);

        std::string vertexSrc = R"(
        #version 330 core
        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;   

        out vec3 v_Position;
        out vec4 v_Color;   

        uniform mat4 VP;

        void main()
        {
          v_Position = a_Position;
          v_Color = a_Color;
          gl_Position = VP * vec4(a_Position, 1.0);
        }
        )";

        std::string fragSrc = R"(
          #version 330 core

          layout(location = 0) out vec4 color;

          in vec3 v_Position;
          in vec4 v_Color;   

          void main()
          {
            color = vec4(v_Position * 0.5 + 0.5, 1.0);
            color = v_Color;
          }
        )";

        m_Shader.reset(Shader::Create(vertexSrc, fragSrc));

        std::string blueShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

            uniform mat4 VP;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = VP * vec4(a_Position, 1.0);	
			}
		)";

        std::string blueShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

        m_BlueShader.reset(Shader::Create(blueShaderVertexSrc, blueShaderFragmentSrc));
    }

    void OnUpdate(Timestep ts) override {
        m_Camera->OnUpdate(ts);
        glm::vec3 cameraPosition = m_Camera->GetPosition();

        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();

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
