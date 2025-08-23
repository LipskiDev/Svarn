#include "GLFW/glfw3.h"
#include "svpch.h"

#include "LinuxWindow.h"
#include <Svarn/Events/ApplicationEvent.h>
#include <Svarn/Events/KeyEvent.h>
#include <Svarn/Events/MouseEvent.h>

#include <Platform/OpenGL/OpenGLRenderingContext.h>

namespace Svarn {

    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char *description) { SV_CORE_ERROR("GLFW Error ({0}): {1}", error, description); }

    Window *Window::Create(const WindowProps &props) { return new LinuxWindow(props); }

    LinuxWindow::LinuxWindow(const WindowProps &props) { Init(props); }

    LinuxWindow::~LinuxWindow() { Shutdown(); }

    void LinuxWindow::OnUpdate() {
        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    void LinuxWindow::SetVSync(bool enabled) {
        if (enabled) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }

        m_Data.VSync = enabled;
    }

    bool LinuxWindow::IsVSync() const { return false; }

    void LinuxWindow::Init(const WindowProps &props) {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        SV_CORE_INFO("Creating window {0} ({1}x{2})", props.Title, props.Width, props.Height);

        if (!s_GLFWInitialized) {
            glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
            int success = glfwInit();
            SV_CORE_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // or 4,5
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
        glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, GL_FALSE);
        m_Context = new OpenGLRenderingContext(m_Window);
        m_Context->Init();

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);
        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height) {
            WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

            WindowResizeEvent event(width, height);
            data.Width = width;
            data.Height = height;
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window) {
            WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods) {
            WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                } break;
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xOffset, double yOffset) {
            WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPosition, double yPosition) {
            WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
            MouseMovedEvent event((float)xPosition, (float)yPosition);
            data.EventCallback(event);
        });
    }

    void LinuxWindow::Shutdown() { glfwDestroyWindow(m_Window); }

}  // namespace Svarn
