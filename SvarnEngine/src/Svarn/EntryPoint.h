#pragma once

#ifdef SV_PLATFORM_WINDOWS
#include <stdio.h>
extern Svarn::Application* Svarn::CreateApplication();

int main(int argc, char** argv) 
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

#ifdef SV_PLATFORM_LINUX
#include <stdio.h>
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