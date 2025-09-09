#include <glad/gl.h>
#include <Svarn.h>

#include "Svarn/Application.h"
#include "Svarn/Renderer/Framebuffer.h"
#include "Svarn/Renderer/Primitives.h"
#include "Svarn/Renderer/Texture.h"
#include "Svarn/Layer.h"
#include "Svarn/Scene/DirectionalLight.h"
#include "imgui.h"
#include <Svarn/Scene/PerspectiveCamera.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace Svarn;

class ExampleLayer : public Layer {
    std::shared_ptr<PerspectiveCamera> m_Camera;
    std::shared_ptr<DirectionalLight> m_Light;

    std::shared_ptr<Mesh> m_SphereMesh;
    std::shared_ptr<Mesh> m_ScreenQuad;

    std::shared_ptr<Mesh> m_Ground;
    glm::mat4 m_GroundTransformation = glm::mat4(1.0);
    std::shared_ptr<Texture> m_GroundTexture;

    std::shared_ptr<Model> m_Cerberus;
    std::shared_ptr<Texture> m_CerberusAlbedo;
    std::shared_ptr<Texture> m_CerberusNormals;
    std::shared_ptr<Texture> m_CerberusRoughness;
    std::shared_ptr<Texture> m_CerberusMetallic;

    std::shared_ptr<Texture> m_EquirectEnvironmentTexture;

    std::shared_ptr<Shader> m_CerberusShader;
    std::shared_ptr<Shader> m_FullScreenQuadShader;
    std::shared_ptr<Shader> m_PureMesh;

    std::shared_ptr<Framebuffer> m_TestFramebuffer;

    // PBR Variables
    glm::vec3 m_SphereAlbedo = glm::vec3(1.0, 0.0, 0.0);
    float m_SphereRoughness = 1.0;
    float m_SphereMetallic = 1.0;

    glm::vec3 m_LightDirection = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 m_LightRadiance = glm::vec3(1.0, 1.0, 1.0);

    RendererAPIInfo apiInfo;

    GLuint envTextureUnfiltered, envTextureEquirect;

    bool m_RenderModel = false;
    bool m_ShowDepthFromLight = false;

    public:
    ExampleLayer() : Layer("Example") {
        apiInfo = Renderer::GetAPIInfo();

        m_Camera.reset(new PerspectiveCamera(90, 16.0 / 9.0, 1.f, 10000));
        m_Light.reset(new DirectionalLight(glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0, 1.0, 1.0)));

        m_Cerberus.reset(Model::Create("Sandbox/assets/models/Cerberus_LP.FBX"));
        m_CerberusAlbedo.reset(Texture::Create("Sandbox/assets/textures/Cerberus_A.tga"));
        m_CerberusNormals.reset(Texture::Create("Sandbox/assets/textures/Cerberus_N.tga"));
        m_CerberusRoughness.reset(Texture::Create("Sandbox/assets/textures/Cerberus_R.tga"));
        m_CerberusMetallic.reset(Texture::Create("Sandbox/assets/textures/Cerberus_M.tga"));

        m_CerberusShader.reset(Shader::Create());
        m_CerberusShader->Attach(ShaderStage::Vertex, "Sandbox/shaders/pbr.vs");
        m_CerberusShader->Attach(ShaderStage::Fragment, "Sandbox/shaders/pbr.fs");
        m_CerberusShader->Link();

        m_FullScreenQuadShader.reset(Shader::Create());
        m_FullScreenQuadShader->Attach(ShaderStage::Vertex, "Sandbox/shaders/quad.vs");
        m_FullScreenQuadShader->Attach(ShaderStage::Fragment, "Sandbox/shaders/quad.fs");
        m_FullScreenQuadShader->Link();

        m_PureMesh.reset(Shader::Create());
        m_PureMesh->Attach(ShaderStage::Vertex, "Sandbox/shaders/mesh.vs");
        m_PureMesh->Attach(ShaderStage::Fragment, "Sandbox/shaders/mesh.fs");
        m_PureMesh->Link();

        m_SphereMesh = Primitives::Sphere(10, 32, 32);
        m_ScreenQuad = Primitives::FullscreenQuad();
        m_Ground = Primitives::FullscreenQuad();
        m_GroundTransformation = glm::translate(m_GroundTransformation, glm::vec3(0.0, -10.0, 0.0));
        m_GroundTransformation = glm::rotate(m_GroundTransformation, glm::radians(-90.f), glm::vec3(1.0, 0.0, 0.0));
        m_GroundTransformation = glm::scale(m_GroundTransformation, glm::vec3(100.0));

        // Textures by Kenny!
        m_GroundTexture.reset(Texture::Create("Sandbox/assets/textures/PNG/Light/texture_01.png"));

        FramebufferSpecification spec;
        spec.width = Application::Get().GetWindow().GetWidth();
        spec.height = Application::Get().GetWindow().GetHeight();
        spec.attachments = {{TextureFormat::RGBA8, AttachmentType::Color}, {TextureFormat::Depth32F, AttachmentType::Depth}};

        m_TestFramebuffer.reset(Framebuffer::Create(spec));
    }

    void OnUpdate(Timestep ts) override {
        m_Camera->OnUpdate(ts);

        if (Input::IsKeyPressed(SV_KEY_R)) {
            m_CerberusShader->ReloadShader();
        }

        if (m_ShowDepthFromLight) {
            m_TestFramebuffer->Bind();
            m_CerberusShader->SetMat4("VP", m_Light->ComputeShadowFrustum(m_Camera->GetFrustumCorners()));
        }

        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();

        m_CerberusShader->Bind();
        Renderer::BeginScene(m_Camera);

        m_CerberusShader->SetMat4("modelMatrix", glm::mat4(1.0));

        m_CerberusShader->SetVec3("u_DirLight.direction", m_Light->m_LightDirection);
        m_CerberusShader->SetVec3("u_DirLight.radiance", m_Light->m_LightRadiance);

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
            m_CerberusShader->SetVec3("material.albedo", m_SphereAlbedo);
            m_CerberusShader->SetFloat("material.roughness", m_SphereRoughness);
            m_CerberusShader->SetFloat("material.metallic", m_SphereMetallic);

            m_CerberusShader->SetBool("material.useAlbedoTexture", false);
            m_CerberusShader->SetBool("material.useNormalTexture", false);
            m_CerberusShader->SetBool("material.useRoughnessTexture", false);
            m_CerberusShader->SetBool("material.useMetallicTexture", false);

            Renderer::Submit(m_SphereMesh, m_CerberusShader);
        }

        m_PureMesh->Bind();
        m_PureMesh->BindTexture("u_Tex", m_GroundTexture);

        m_PureMesh->SetMat4("modelMatrix", m_GroundTransformation);
        Renderer::Submit(m_Ground, m_PureMesh);

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

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            if (ImGui::Button(m_RenderModel ? "Render Model ON" : "Render Model OFF")) {
                m_RenderModel = !m_RenderModel;
            }

            ImGui::Spacing();

            ImGui::SliderFloat3("Light Direction", &m_Light->m_LightDirection.x, -1.0f, 1.0f);
            ImGui::ColorEdit3("Light Color", &m_Light->m_LightRadiance.x);

            if (!m_RenderModel) {
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                ImGui::ColorEdit3("Sphere Color", &m_SphereAlbedo.x);
                ImGui::SliderFloat("Sphere Roughness", &m_SphereRoughness, 0.0f, 1.0f);
                ImGui::SliderFloat("Sphere Metallic", &m_SphereMetallic, 0.0f, 1.0f);
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
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
