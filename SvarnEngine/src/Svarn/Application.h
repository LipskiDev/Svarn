#pragma once

#include <Svarn/Core.h>
#include <Svarn/Events/Event.h>
#include <Svarn/Window.h>
#include "Events/ApplicationEvent.h"


namespace Svarn {
    class SVARN_API Application
    {
    public:
        Application();
        virtual ~Application();

        virtual void Run();

        void OnEvent(Event &e);

    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

    Application* CreateApplication();
}