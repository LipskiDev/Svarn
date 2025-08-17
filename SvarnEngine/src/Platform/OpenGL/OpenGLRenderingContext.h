#pragma once

#include <Svarn/Renderer/RenderingContext.h>

struct GLFWwindow;

namespace Svarn {

    class OpenGLRenderingContext : public RenderingContext {
        public:
        OpenGLRenderingContext(GLFWwindow* windowHandle);

        virtual void Init() override;
        virtual void SwapBuffers() override;

        private:
        GLFWwindow* m_WindowHandle;
    };

}  // namespace Svarn
