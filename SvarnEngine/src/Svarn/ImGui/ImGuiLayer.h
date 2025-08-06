#pragma once

#include <Svarn/Events/Event.h>
#include <Svarn/Layer.h>
#include "Svarn/Events/ApplicationEvent.h"
#include "Svarn/Events/KeyEvent.h"
#include "Svarn/Events/MouseEvent.h"

namespace Svarn {
    class ImGuiLayer : public Layer {
        public:
        ImGuiLayer();
        ~ImGuiLayer() override = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& e) override;
        void OnUpdate() override;

        private:
        // Mouse Events
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        bool OnMouseMovedEvent(MouseMovedEvent& e);
        bool OnMouseScrolledEvent(MouseScrolledEvent& e);

        // Key Events
        bool OnKeyPressedEvent(KeyPressedEvent& e);
        bool OnKeyReleasedEvent(KeyReleasedEvent& e);

        // Window Events
        bool OnWindowResizeEvent(WindowResizeEvent& e);

        private:
        float m_Time = 0.0f;
    };
}  // namespace Svarn
