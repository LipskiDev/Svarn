#pragma once

#include <glm/fwd.hpp>
#include "Svarn/Renderer/Texture.h"
namespace Svarn {
    class OpenGLTexture : public Texture {
        public:
        friend class OpenGLFramebuffer;

        OpenGLTexture(std::string texturePath);
        OpenGLTexture(TextureSpecification spec);
        virtual void SetFiltering(TextureFiltering minFilter, TextureFiltering magFilter) const override;
        virtual void SetWrapping(TextureWrapping wrap) const override;
        virtual void SetData(const void* pixels, size_t /*sizeBytes*/, int level) override;

        private:
        void InvalidateImpl(std::string_view path, uint32_t width, uint32_t height, const void* data, uint32_t channels);
        virtual void MapFormat(TextureFormat f, unsigned& internal, unsigned& format, unsigned& type) override;
    };
}  // namespace Svarn
