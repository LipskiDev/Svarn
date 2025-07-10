#include "Application.h"
#include "stdio.h"

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
        while(true) {
            printf("Hello from Application\n");
        }
    }
} // namespace Svarn