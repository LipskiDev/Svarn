#include <svpch.h>
#include <glad/gl.h>

#include "Application.h"
#include <Svarn/Log.h>
#include <Svarn/Input.h>
#include "Svarn/Renderer/Buffer.h"
#include "Svarn/Renderer/VertexArray.h"
#include <Svarn/Renderer/RenderCommand.h>
#include <Svarn/Renderer/Renderer.h>

namespace Svarn {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
    Application* Application::s_Instance = nullptr;

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type) {
            case Svarn::ShaderDataType::Float:
                return GL_FLOAT;
            case Svarn::ShaderDataType::Float2:
                return GL_FLOAT;
            case Svarn::ShaderDataType::Float3:
                return GL_FLOAT;
            case Svarn::ShaderDataType::Float4:
                return GL_FLOAT;
            case Svarn::ShaderDataType::Mat3:
                return GL_FLOAT;
            case Svarn::ShaderDataType::Mat4:
                return GL_FLOAT;
            case Svarn::ShaderDataType::Int:
                return GL_INT;
            case Svarn::ShaderDataType::Int2:
                return GL_INT;
            case Svarn::ShaderDataType::Int3:
                return GL_INT;
            case Svarn::ShaderDataType::Int4:
                return GL_INT;
            case Svarn::ShaderDataType::Bool:
                return GL_BOOL;
        }

        SV_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    Application::Application() {
        SV_CORE_ASSERT(!s_Instance, "Application already exists.");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application() {}

    void Application::Run() {
        while (m_Running) {
            m_Window->OnUpdate();

            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate();
            }

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack) {
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    void Application::PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer) {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
            (*--it)->OnEvent(e);
            if (e.Handled) break;
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        m_Running = false;
        return true;
    }
}  // namespace Svarn
