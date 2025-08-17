#include <svpch.h>

#include "OpenGLRenderingContext.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <GL/gl.h>
#include "Svarn/Core.h"

namespace Svarn {

    OpenGLRenderingContext::OpenGLRenderingContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle) {
        SV_CORE_ASSERT(windowHandle, "WindowHandle is null!");
    }

    void OpenGLRenderingContext::Init() {
        glfwMakeContextCurrent(m_WindowHandle);
        int success = gladLoadGL(glfwGetProcAddress);
        SV_CORE_ASSERT(success, "Failed to initialize GLAD!");

        const char* vendor =
            reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        const char* renderer =
            reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        const char* version =
            reinterpret_cast<const char*>(glGetString(GL_VERSION));

        SV_CORE_INFO("OpenGL Info:");

        SV_CORE_INFO("  Vendor: {0}", vendor);
        SV_CORE_INFO("  Renderer: {0}", renderer);
        SV_CORE_INFO("  Version: {0}", version);
    }

    void OpenGLRenderingContext::SwapBuffers() {
        glfwSwapBuffers(m_WindowHandle);
    }

}  // namespace Svarn
