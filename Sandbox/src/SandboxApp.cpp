#include <Svarn.h>

#include "Svarn/Application.h"
#include "Svarn/Renderer/Texture.h"
#include "Svarn/Layer.h"
#include <Svarn/Scene/PerspectiveCamera.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace Svarn;

class ExampleLayer : public Layer {
    std::shared_ptr<PerspectiveCamera> m_Camera;

    std::shared_ptr<Model> m_Cerberus;
    std::shared_ptr<Texture> m_CerberusAlbedo;
    std::shared_ptr<Texture> m_CerberusNormals;
    std::shared_ptr<Texture> m_CerberusRoughness;
    std::shared_ptr<Texture> m_CerberusMetallic;

    std::shared_ptr<Shader> m_CerberusShader;

    RendererAPIInfo apiInfo;

    public:
    ExampleLayer() : Layer("Example") {
        apiInfo = Renderer::GetAPIInfo();

        m_Camera.reset(new PerspectiveCamera(90, 16.0 / 9.0, 0.1f, 10000));

        m_Cerberus.reset(Model::Create("Sandbox/assets/models/Cerberus_LP.FBX"));
        m_CerberusAlbedo.reset(Texture::Create("Sandbox/assets/textures/Cerberus_A.tga"));
        m_CerberusNormals.reset(Texture::Create("Sandbox/assets/textures/Cerberus_N.tga"));
        m_CerberusRoughness.reset(Texture::Create("Sandbox/assets/textures/Cerberus_R.tga"));
        m_CerberusMetallic.reset(Texture::Create("Sandbox/assets/textures/Cerberus_M.tga"));

        m_CerberusShader.reset(Shader::Create("Sandbox/shaders/cerberus.vs", "Sandbox/shaders/cerberus.fs"));
        //         u_DirLight.direction = vec3(-1.0, -1.0, -1.0);
        // u_DirLight.radiance = vec3(0.5);
    }

    void OnUpdate(Timestep ts) override {
        m_Camera->OnUpdate(ts);

        if (Input::IsKeyPressed(SV_KEY_R)) {
            m_CerberusShader->ReloadShader();
        }

        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();

        Renderer::BeginScene(m_Camera);

        m_CerberusShader->SetVec3("u_DirLight.direction", glm::vec3(-1.0, -1.0, -1.0));
        m_CerberusShader->SetVec3("u_DirLight.radiance", glm::vec3(1.0));
        m_CerberusAlbedo->Bind(0);
        m_CerberusNormals->Bind(1);
        m_CerberusRoughness->Bind(2);
        m_CerberusMetallic->Bind(3);
        Renderer::Submit(m_Cerberus, m_CerberusShader);

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
