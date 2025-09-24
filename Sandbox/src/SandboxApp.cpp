#include <glad/gl.h>
#include <Svarn.h>

#include "Svarn/Application.h"
#include "Svarn/Renderer/Framebuffer.h"
#include "Svarn/Renderer/Primitives.h"
#include "Svarn/Renderer/ShaderLibrary.h"
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

    std::shared_ptr<Mesh> m_Ground;
    std::shared_ptr<Texture> m_GroundTexture;
    Material m_GroundMaterial = Material::New();
    Transform m_GroundTransform = Transform(glm::vec3(0.0f, -10.f, 0.0f), glm::vec3(100.f), glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f));

    std::shared_ptr<Model> m_Cerberus;
    std::shared_ptr<Texture> m_CerberusAlbedo;
    std::shared_ptr<Texture> m_CerberusNormals;
    std::shared_ptr<Texture> m_CerberusRoughness;
    std::shared_ptr<Texture> m_CerberusMetallic;

    Material m_CerberusMaterial = Material::New();

    Transform m_CerberusTransform = Transform(glm::vec3(-65.0, 30.0, 0.0), glm::vec3(1.0), glm::vec3(0.0f, glm::radians(90.0f), glm::radians(90.0f)));

    std::shared_ptr<Shader> m_PBRShader;

    Material m_sphereMaterial = Material::New();

    // PBR Variables
    glm::vec3 m_SphereAlbedo = glm::vec3(1.0, 1.0, 1.0);
    float m_SphereRoughness = 1.0;
    float m_SphereMetallic = 1.0;

    RendererAPIInfo apiInfo;

    bool m_RenderModel = false;

    public:
    ExampleLayer() : Layer("Example") {
        apiInfo = GetRenderer().GetAPIInfo();

        m_Camera.reset(new PerspectiveCamera(90, 16.0 / 9.0, 1.f, 10000));
        m_Light.reset(new DirectionalLight(glm::vec3(0.0, -1.0, 0.0), glm::vec3(1.0, 1.0, 1.0)));

        m_CerberusAlbedo.reset(Texture::Create("Sandbox/assets/textures/Cerberus_A.tga"));
        m_CerberusNormals.reset(Texture::Create("Sandbox/assets/textures/Cerberus_N.tga"));
        m_CerberusRoughness.reset(Texture::Create("Sandbox/assets/textures/Cerberus_R.tga"));
        m_CerberusMetallic.reset(Texture::Create("Sandbox/assets/textures/Cerberus_M.tga"));

        m_CerberusMaterial =
            Material::FromTextures("Cerberus Material", m_CerberusAlbedo, m_CerberusNormals, m_CerberusRoughness, m_CerberusMetallic);

        m_Cerberus.reset(Model::Create("Sandbox/assets/models/Cerberus_LP.FBX"));

        m_Cerberus->SetMaterial(m_CerberusMaterial);

        m_PBRShader = GetShaderLibrary().Get("PBR");

        m_SphereMesh = Primitives::Sphere(10, 64, 64);
        m_SphereMesh->SetMaterial(m_sphereMaterial);
        m_Ground = Primitives::Quad();

        m_GroundTexture.reset(Texture::Create("Sandbox/assets/textures/PNG/Green/texture_01.png"));
        m_GroundTexture->SetFiltering(TextureFiltering::Nearest, TextureFiltering::Nearest);
        m_GroundMaterial.SetMetallicValue(0.0);
        m_GroundMaterial.SetRoughnessValue(1.0);
        m_GroundMaterial.SetAlbedoTexture(m_GroundTexture);
        m_Ground->SetMaterial(m_GroundMaterial);
    }

    void OnUpdate(Timestep ts) override {
        m_Camera->OnUpdate(ts);

        GetRenderer().Submit(m_Light);
        if (Input::IsKeyPressed(SV_KEY_R)) {
            m_PBRShader->ReloadShader();
        }

        GetRenderer().BeginScene(m_Camera);

        if (m_RenderModel) {
            GetRenderer().Submit(m_Cerberus, m_CerberusTransform);
        } else {
            GetRenderer().Submit(m_SphereMesh);
        }

        GetRenderer().Submit(m_Ground, m_GroundTransform);

        GetRenderer().EndScene();
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
                if (ImGui::ColorEdit3("Sphere Color", &m_SphereAlbedo.x)) {
                    m_sphereMaterial.SetAlbedoValue(m_SphereAlbedo);
                    m_SphereMesh->SetMaterial(m_sphereMaterial);
                };

                if (ImGui::SliderFloat("Sphere Roughness", &m_SphereRoughness, 0.0f, 1.0f)) {
                    m_sphereMaterial.SetRoughnessValue(m_SphereRoughness);
                    m_SphereMesh->SetMaterial(m_sphereMaterial);
                }

                if (ImGui::SliderFloat("Sphere Metallic", &m_SphereMetallic, 0.0f, 1.0f)) {
                    m_sphereMaterial.SetMetallicValue(m_SphereMetallic);
                    m_SphereMesh->SetMaterial(m_sphereMaterial);
                }
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
