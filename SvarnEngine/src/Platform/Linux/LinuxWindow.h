#pragma once

#include <GLFW/glfw3.h>
#include <Svarn/Window.h>
#include <Svarn/Renderer/RenderingContext.h>
#include <glad/gl.h>

namespace Svarn {

    class LinuxWindow : public Window {
        public:
        LinuxWindow(const WindowProps &props);
        virtual ~LinuxWindow();

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }

        // Window Attributes
        inline void SetEventCallback(const EventCallbackFn &callback) override {
            m_Data.EventCallback = callback;
        }
        void SetVSync(bool enabled);
        bool IsVSync() const;

        inline virtual void *GetNativeWindow() const { return m_Window; }

        private:
        virtual void Init(const WindowProps &props);
        virtual void Shutdown();

        private:
        GLFWwindow *m_Window;
        RenderingContext *m_Context;

        struct WindowData {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };
}  // namespace Svarn
