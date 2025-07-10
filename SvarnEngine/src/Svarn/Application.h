#pragma once

#include "Core.h"

namespace Svarn {
    class SVARN_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
    };

    Application* CreateApplication();
}