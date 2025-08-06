#include <Svarn.h>

#include "Svarn/Layer.h"

class ExampleLayer : public Svarn::Layer {
    public:
    ExampleLayer() : Layer("Example") {}

    void OnUpdate() override { SV_INFO("ExampleLayer::Update"); }

    void OnEvent(Svarn::Event& e) override { SV_TRACE("{0}", e); }
};

class Sandbox : public Svarn::Application {
    public:
    Sandbox() {
        PushLayer(new ExampleLayer());
        PushOverlay(new Svarn::ImGuiLayer());
    }

    ~Sandbox() {}
};

Svarn::Application* Svarn::CreateApplication() {
    SV_CORE_TRACE("Svarn::CreateApplication was called");
    return new Sandbox();
}
