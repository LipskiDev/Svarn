#pragma once

#include "Svarn/Core.h"
#include "Svarn/Events/Event.h"
#include "Svarn/Log.h"

namespace Svarn {
    class SVARN_API Layer {
        public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {};
        virtual void OnDetach() {};
        virtual void OnUpdate() {};
        virtual void OnEvent(Event& e) {
            SV_CORE_TRACE("Called from Layer Event.");
        };

        inline const std::string& GetName() const { return m_DebugName; }

        protected:
        std::string m_DebugName;
    };
}  // namespace Svarn
