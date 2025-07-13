#include <svpch.h>
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Log.h"


namespace Svarn
{
    Application::Application()
    {
        
    }
    
    Application::~Application()
    {
        
    }

    void Application::Run()
    {

        WindowResizeEvent e(1280, 720);
        SV_TRACE(e);
        WindowCloseEvent e1;
        SV_TRACE(e1);
        WindowCloseEvent e2;
        SV_TRACE(e2);
        AppTickEvent e3;
        SV_TRACE(e3);
        AppUpdateEvent e4;
        SV_TRACE(e4);
        AppRenderEvent e5;
        SV_TRACE(e5);

        while(true) {
        }
    }
} // namespace Svarn