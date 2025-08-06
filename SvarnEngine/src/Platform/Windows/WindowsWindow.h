#pragma once

#include <GLFW/glfw3.h>
#include <Svarn/Window.h>
#include <glad/gl.h>

namespace Svarn {

    class WindowsWindow : public Window {
        public:
        WindowsWindow(const WindowProps &props);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }

        // Window Attributes
        inline void SetEventCallback(const EventCallbackFn &callback) override {
            m_Data.EventCallback = callback;
        }
        void SetVSync(bool enabled);
        bool IsVSync() const;

        private:
        virtual void Init(const WindowProps &props);
        virtual void Shutdown();

        private:
        GLFWwindow *m_Window;

        struct WindowData {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };
}  // namespace Svarn
