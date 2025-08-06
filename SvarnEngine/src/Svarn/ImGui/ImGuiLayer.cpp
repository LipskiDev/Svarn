#include <svpch.h>
#include <Svarn/ImGui/ImGuiLayer.h>
#include <Svarn/Application.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include "Svarn/Events/ApplicationEvent.h"
#include "Svarn/Events/Event.h"
#include "Svarn/Events/KeyEvent.h"
#include "Svarn/Events/MouseEvent.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

namespace Svarn {
    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

    void ImGuiLayer::OnAttach() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();

        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;

        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        ImGui_ImplOpenGL3_Init("#version 410");
    };

    void ImGuiLayer::OnDetach() {};

    void ImGuiLayer::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseButtonPressedEvent>(
            SV_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));

        dispatcher.Dispatch<MouseButtonReleasedEvent>(
            SV_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));

        dispatcher.Dispatch<MouseMovedEvent>(
            SV_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
    };

    void ImGuiLayer::OnUpdate() {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize =
            ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    };

    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = true;

        return false;
    }

    bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = false;

        return false;
    }

    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(e.GetX(), e.GetY());

        return false;
    }

    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += e.GetXOffset();
        io.MouseWheel += e.GetYOffset();

        return false;
    }

    bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();

        io.AddKeyEvent(e.GetKeyCode(), true);

        /*
              io.KeysDown[e.GetKeyCode()] = true;

              io.KeyCtrl = io.KeysData[GLFW_KEY_LEFT_CONTROL] ||
                           io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
              io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] ||
                            io.KeysDown[GLFW_KEY_RIGHT_SHIFT];

              io.KeyAlt =
                  io.KeysDown[GLFW_KEY_LEFT_ALT] ||
           io.KeysDown[GLFW_KEY_RIGHT_ALT];

              io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] ||
                            io.KeysDown[GLFW_KEY_RIGHT_SUPER];
      */
        return false;
    }

    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e) {}

    bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e) {}

}  // namespace Svarn
