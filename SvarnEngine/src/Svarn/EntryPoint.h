#pragma once

#ifdef SV_PLATFORM_WINDOWS

extern Svarn::Application* Svarn::CreateApplication();

int main(int argc, char** argv) 
{
    auto app = Svarn::CreateApplication();
    app->Run();
    delete app;
}   

#endif