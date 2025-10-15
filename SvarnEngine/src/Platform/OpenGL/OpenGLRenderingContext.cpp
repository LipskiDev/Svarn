#include <svpch.h>

#include "OpenGLRenderingContext.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <GL/gl.h>
#include "Svarn/Core.h"
#include "Svarn/Log.h"
#include "Svarn/Renderer/ShaderLibrary.h"
#include <Svarn/Renderer/Renderer.h>

namespace Svarn {

    OpenGLRenderingContext::OpenGLRenderingContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
        SV_CORE_ASSERT(windowHandle, "WindowHandle is null!");
    }

    void OpenGLRenderingContext::Init() {
        glfwMakeContextCurrent(m_WindowHandle);
        SV_CORE_INFO("OpenGL Rendering Context setup.");
        int success = gladLoadGL(glfwGetProcAddress);
        SV_CORE_ASSERT(success, "Failed to initialize GLAD!");
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
        glClearDepth(1.0);
        GetRenderer().Init();
    }

    void OpenGLRenderingContext::SwapBuffers() { glfwSwapBuffers(m_WindowHandle); }

}  // namespace Svarn
