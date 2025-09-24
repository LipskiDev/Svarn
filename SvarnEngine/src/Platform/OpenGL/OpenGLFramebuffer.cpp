#include <svpch.h>

#include <Platform/OpenGL/OpenGLFramebuffer.h>
#include "Svarn/Core.h"
#include "Svarn/Renderer/Texture.h"
#include "glad/gl.h"
#include <Svarn/Application.h>

namespace Svarn {

    static bool IsDepthFormat(TextureFormat f) {
        switch (f) {
            case TextureFormat::Depth24Stencil8:
            case TextureFormat::Depth32F:
            case TextureFormat::Depth24:
                return true;
            default:
                return false;
        }
    }

    // Fill a TextureSpecification suitable for an attachment texture
    static TextureSpecification MakeAttachmentTexSpec(uint32_t w, uint32_t h, TextureFormat fmt) {
        TextureSpecification ts;
        ts.width = w;
        ts.height = h;
        ts.format = fmt;
        ts.filtering = TextureFiltering::Nearest;
        ts.wrapping = TextureWrapping::ClampToEdge;
        return ts;
    }

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec) {
        m_Spec = spec;
        Invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer() {
        if (m_FBO) glDeleteFramebuffers(1, &m_FBO);
    }

    void OpenGLFramebuffer::Invalidate() {
        if (m_FBO) {
            glDeleteFramebuffers(1, &m_FBO);
            m_FBO = 0;
        }

        m_ColorAttachments.clear();
        m_DepthAttachment.reset();

        glGenFramebuffers(1, &m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        std::vector<GLenum> drawBuffers;
        uint32_t colorIndex = 0;

        for (const auto& ad : m_Spec.attachments) {
            TextureSpecification ts = MakeAttachmentTexSpec(m_Spec.width, m_Spec.height, ad.format);

            if (IsDepthFormat(ad.format)) {
                m_DepthAttachment.reset(Texture::Create(ts));

                GLenum attach = (ad.format == TextureFormat::Depth24Stencil8) ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;

                glFramebufferTexture2D(GL_FRAMEBUFFER, attach, GL_TEXTURE_2D, m_DepthAttachment->m_RendererID, 0);
                SV_CORE_INFO("Depth Attachment ID: {0}", m_DepthAttachment->m_RendererID);
            } else {
                std::shared_ptr<Texture> colorTex;
                colorTex.reset(Texture::Create(ts));

                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorIndex, GL_TEXTURE_2D, colorTex->m_RendererID, 0);
                SV_CORE_INFO("Color Attachment ID: {0}", colorTex->m_RendererID);

                drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + colorIndex);
                m_ColorAttachments.push_back(colorTex);
                colorIndex++;
            }
        }

        if (m_ColorAttachments.empty()) {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        } else {
            glDrawBuffers(drawBuffers.size(), drawBuffers.data());
        }

        auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        SV_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Framebuffer incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        glViewport(0, 0, m_Spec.width, m_Spec.height);
    }

    void OpenGLFramebuffer::Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        int windowHeight = Application::Get().GetWindow().GetHeight();
        int windowWidth = Application::Get().GetWindow().GetWidth();
        glViewport(0, 0, windowWidth, windowHeight);
    }

    void OpenGLFramebuffer::Clear() {
        Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Unbind();
    }

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) {
        if (width == 0 || height == 0) return;
        if (m_Spec.width == width && m_Spec.height == height) return;
        m_Spec.width = width;
        m_Spec.height = height;
        Invalidate();
    }

    std::shared_ptr<Texture> OpenGLFramebuffer::GetColorAttachment(size_t index) {
        if (index >= m_ColorAttachments.size()) return nullptr;
        return m_ColorAttachments[index];
    }
}  // namespace Svarn
