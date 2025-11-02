#include <glad/gl.h>
#include <Svarn.h>

#include "Svarn/Application.h"
#include "Svarn/Core/ThreadHandler.h"
#include "Svarn/Renderer/Framebuffer.h"
#include "Svarn/Renderer/Primitives.h"
#include "Svarn/Renderer/ShaderLibrary.h"
#include "Svarn/Renderer/Texture.h"
#include "Svarn/Layer.h"
#include "Svarn/Scene/DirectionalLight.h"
#include "Svarn/Terrain/TerrainStreamer.h"
#include "imgui.h"
#include <Svarn/Scene/PerspectiveCamera.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <thread>

using namespace Svarn;

class ExampleLayer : public Layer {
    std::shared_ptr<PerspectiveCamera> m_Camera;
    std::shared_ptr<DirectionalLight> m_Light;

    RendererAPIInfo apiInfo;

    bool m_RenderTerrain = true;

    public:
    ExampleLayer() : Layer("Example") {
        apiInfo = GetRenderer().GetAPIInfo();

        m_Camera.reset(new PerspectiveCamera(90, 16.0 / 9.0, 1.f, 10000));
        m_Light.reset(new DirectionalLight(glm::vec3(0.0, -1.0, 0.0), glm::vec3(1.0, 1.0, 1.0)));
    }

    void OnUpdate(Timestep ts) override {
        m_Camera->OnUpdate(ts);

        GetRenderer().Submit(m_Light);

        if (Input::IsKeyPressed(SV_KEY_L)) {
            GetThreadHandler().enqueue([] { std::cout << "L key press handled in thread " << std::this_thread::get_id() << std::endl; });
        }

        GetRenderer().BeginScene(m_Camera);

        GetRenderer().RenderTerrain(m_RenderTerrain);

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

            auto camPos = m_Camera->GetPosition();
            auto camChunk = GetTerrainStreamer().GetCurrentChunk(*m_Camera);
            ImGui::Text("Camera Coordinates: (%f, %f)", camPos.x, camPos.z);
            ImGui::Text("Camera Chunk: (%d, %d)", (int)camChunk.x, (int)camChunk.z);

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            if (ImGui::Button(m_RenderTerrain ? "Render Terrain ON" : "Render Terrain OFF")) {
                m_RenderTerrain = !m_RenderTerrain;
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
