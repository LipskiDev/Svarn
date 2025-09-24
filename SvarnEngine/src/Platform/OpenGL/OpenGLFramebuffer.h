#pragma once

#include <memory>
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Svarn/Renderer/Framebuffer.h"
namespace Svarn {
    class OpenGLFramebuffer : public Framebuffer {
        public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        ~OpenGLFramebuffer();

        void Bind() override;
        void Unbind() override;
        void Clear() override;

        void Resize(uint32_t width, uint32_t height) override;

        uint32_t GetRendererID() const override { return m_FBO; }

        std::shared_ptr<Texture> GetColorAttachment(size_t index = 0) override;
        std::shared_ptr<Texture> GetDepthAttachment() override { return m_DepthAttachment; }

        private:
        void Invalidate();

        private:
        uint32_t m_FBO = 0;
        std::vector<std::shared_ptr<Texture>> m_ColorAttachments;
        std::shared_ptr<Texture> m_DepthAttachment;
    };
}  // namespace Svarn
