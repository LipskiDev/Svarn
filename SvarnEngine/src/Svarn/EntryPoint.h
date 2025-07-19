#pragma once

#include <stdio.h>

#ifdef SV_PLATFORM_WINDOWS
extern Svarn::Application* Svarn::CreateApplication();

int main(int argc, char** argv) 
{
    Svarn::Log::Init();
    SV_CORE_WARN("Initialized Loggin!\n");
    int a = 555;
    SV_INFO("Hello! Var={0}\n", a);

    

    auto app = Svarn::CreateApplication();

    if(!app)
    {
        SV_CORE_ERROR("Svarn::CreateApplication returned nullptr");
        return -1;
    }
    app->Run();
    delete app;
}   

#endif

#ifdef SV_PLATFORM_LINUX
extern Svarn::Application *Svarn::CreateApplication();

int main(int argc, char **argv)
{
    Svarn::Log::Init();
    SV_CORE_WARN("Initialized Loggin!\n");
    int a = 555;
    SV_INFO("Hello! Var={0}\n", a);

    auto app = Svarn::CreateApplication();
    app->Run();
    delete app;
}

#endif