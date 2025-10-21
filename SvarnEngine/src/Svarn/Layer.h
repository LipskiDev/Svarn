#pragma once

#include "Svarn/Core.h"
#include "Svarn/Events/Event.h"
#include "Svarn/Timestep.h"

namespace Svarn {
    class SVARN_API Layer {
        public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {};
        virtual void OnDetach() {};
        virtual void OnUpdate([[maybe_unused]] Timestep ts) {};
        virtual void OnImGuiRender([[maybe_unused]] Timestep ts) {};
        virtual void OnEvent([[maybe_unused]] Event& e) {};

        inline const std::string& GetName() const { return m_DebugName; }

        protected:
        std::string m_DebugName;
    };
}  // namespace Svarn
