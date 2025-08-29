#include <glad/gl.h>
#include <Svarn.h>

#include "Svarn/Application.h"
#include "Svarn/Renderer/Primitives.h"
#include "Svarn/Renderer/Texture.h"
#include "Svarn/Layer.h"
#include <Svarn/Scene/PerspectiveCamera.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace Svarn;

class ExampleLayer : public Layer {
    std::shared_ptr<PerspectiveCamera> m_Camera;

    std::shared_ptr<Mesh> m_SphereMesh;

    std::shared_ptr<Model> m_Cerberus;
    std::shared_ptr<Texture> m_CerberusAlbedo;
    std::shared_ptr<Texture> m_CerberusNormals;
    std::shared_ptr<Texture> m_CerberusRoughness;
    std::shared_ptr<Texture> m_CerberusMetallic;

    std::shared_ptr<Texture> m_EquirectEnvironmentTexture;

    std::shared_ptr<Shader> m_CerberusShader;
    std::shared_ptr<Shader> m_EquirectToCubeShader;

    RendererAPIInfo apiInfo;

    GLuint envTextureUnfiltered, envTextureEquirect;

    bool m_RenderModel = false;

    public:
    ExampleLayer() : Layer("Example") {
        apiInfo = Renderer::GetAPIInfo();

        m_Camera.reset(new PerspectiveCamera(90, 16.0 / 9.0, 0.1f, 10000));

        m_Cerberus.reset(Model::Create("Sandbox/assets/models/Cerberus_LP.FBX"));
        m_CerberusAlbedo.reset(Texture::Create("Sandbox/assets/textures/Cerberus_A.tga"));
        m_CerberusNormals.reset(Texture::Create("Sandbox/assets/textures/Cerberus_N.tga"));
        m_CerberusRoughness.reset(Texture::Create("Sandbox/assets/textures/Cerberus_R.tga"));
        m_CerberusMetallic.reset(Texture::Create("Sandbox/assets/textures/Cerberus_M.tga"));

        m_CerberusShader.reset(Shader::Create());
        m_CerberusShader->Attach(ShaderStage::Vertex, "Sandbox/shaders/cerberus.vs");
        m_CerberusShader->Attach(ShaderStage::Fragment, "Sandbox/shaders/pbr.fs");
        m_CerberusShader->Link();

        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &envTextureUnfiltered);
        glTextureStorage2D(envTextureUnfiltered, 1, GL_RGBA16F, 1024, 1024);
        glTextureParameteri(envTextureUnfiltered, GL_TEXTURE_MIN_FILTER, 1 > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
        glTextureParameteri(envTextureUnfiltered, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameterf(envTextureUnfiltered, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);

        m_EquirectToCubeShader.reset(Shader::Create());
        m_EquirectToCubeShader->Attach(ShaderStage::Compute, "Sandbox/shaders/equirectToCube.cs");
        m_EquirectToCubeShader->Link();

        m_EquirectEnvironmentTexture.reset(Texture::Create("Sandbox/assets/textures/meadow.hdr"));

        m_EquirectToCubeShader->Bind();
        m_EquirectToCubeShader->BindTexture("inputTexture", m_EquirectEnvironmentTexture);

        glBindImageTexture(0, envTextureUnfiltered, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16F);
        m_EquirectToCubeShader->Dispatch(1024 / 32, 1024 / 32, 6);

        m_SphereMesh.reset(Primitives::Sphere(10, 32, 32));
    }

    void OnUpdate(Timestep ts) override {
        m_Camera->OnUpdate(ts);

        if (Input::IsKeyPressed(SV_KEY_R)) {
            m_CerberusShader->ReloadShader();
        }

        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();

        Renderer::BeginScene(m_Camera);

        m_CerberusShader->Bind();
        m_CerberusShader->SetVec3("u_DirLight.direction", glm::vec3(-1.0, -1.0, -1.0));
        m_CerberusShader->SetVec3("u_DirLight.radiance", glm::vec3(1.0));

        if (m_RenderModel) {
            m_CerberusShader->BindTexture("material.albedoTexture", m_CerberusAlbedo);
            m_CerberusShader->BindTexture("material.normalTexture", m_CerberusNormals);
            m_CerberusShader->BindTexture("material.roughnessTexture", m_CerberusRoughness);
            m_CerberusShader->BindTexture("material.metallicTexture", m_CerberusMetallic);
            m_CerberusShader->SetBool("material.useAlbedoTexture", true);
            m_CerberusShader->SetBool("material.useNormalTexture", true);
            m_CerberusShader->SetBool("material.useRoughnessTexture", true);
            m_CerberusShader->SetBool("material.useMetallicTexture", true);

            Renderer::Submit(m_Cerberus, m_CerberusShader);
        } else {
            m_CerberusShader->SetVec3("material.albedo", glm::vec3(1.0, 0.0, 0.0));
            m_CerberusShader->SetFloat("material.roughness", 0.0);
            m_CerberusShader->SetFloat("material.metallic", 0.0);

            m_CerberusShader->SetBool("material.useAlbedoTexture", false);
            m_CerberusShader->SetBool("material.useNormalTexture", false);
            m_CerberusShader->SetBool("material.useRoughnessTexture", false);
            m_CerberusShader->SetBool("material.useMetallicTexture", false);

            Renderer::Submit(m_SphereMesh, m_CerberusShader);
        }

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
            if (ImGui::Button("Toggle Render Model")) {
                m_RenderModel = !m_RenderModel;  // switch on press
            }

            ImGui::Text("Render Model: %s", m_RenderModel ? "ON" : "OFF");
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
