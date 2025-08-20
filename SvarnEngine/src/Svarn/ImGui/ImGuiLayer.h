#pragma once

#include <Svarn/Events/Event.h>
#include <Svarn/Layer.h>

namespace Svarn {
    class ImGuiLayer : public Layer {
        public:
        ImGuiLayer();
        ~ImGuiLayer() override = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender(Timestep ts) override;

        void Begin();
        void End();

        private:
        float m_Time = 0.0f;
    };

}  // namespace Svarn
