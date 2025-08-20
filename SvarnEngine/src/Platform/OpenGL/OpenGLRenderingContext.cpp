#include <svpch.h>

#include "OpenGLRenderingContext.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <GL/gl.h>
#include "Svarn/Core.h"

namespace Svarn {

    OpenGLRenderingContext::OpenGLRenderingContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
        SV_CORE_ASSERT(windowHandle, "WindowHandle is null!");
    }

    void OpenGLRenderingContext::Init() {
        glfwMakeContextCurrent(m_WindowHandle);
        int success = gladLoadGL(glfwGetProcAddress);
        SV_CORE_ASSERT(success, "Failed to initialize GLAD!");
    }

    void OpenGLRenderingContext::SwapBuffers() { glfwSwapBuffers(m_WindowHandle); }

}  // namespace Svarn
