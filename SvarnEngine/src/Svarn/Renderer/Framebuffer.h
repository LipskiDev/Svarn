#pragma once

#include "Svarn/Renderer/Texture.h"
namespace Svarn {
    enum class AttachmentType { Color, Depth };

    struct AttachmentDesc {
        TextureFormat format;
        AttachmentType type;
        bool sampled = true;
    };

    struct FramebufferSpecification {
        uint32_t width = 1, height = 1;
        uint32_t samples = 1;
        std::vector<AttachmentDesc> attachments;
        bool allowReadback = false;
    };

    class Framebuffer {
        public:
        virtual ~Framebuffer() {};

        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        virtual void Clear() = 0;

        virtual void Resize(uint32_t width, uint32_t height) = 0;

        virtual uint32_t GetRendererID() const = 0;

        virtual std::shared_ptr<Texture> GetColorAttachment(size_t index = 0) = 0;
        virtual std::shared_ptr<Texture> GetDepthAttachment() = 0;

        static Framebuffer* Create(FramebufferSpecification spec);

        protected:
        FramebufferSpecification m_Spec;
    };
}  // namespace Svarn
