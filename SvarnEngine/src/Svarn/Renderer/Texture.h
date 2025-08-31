#pragma once

#include <cstdint>

namespace Svarn {

    enum class TextureFormat { RGBA8, RGBA16F, R11F_G11F_B10F, R32F, Depth24, Depth32F, Depth24Stencil8 };

    enum class TextureFiltering { Nearest, Linear };

    enum class TextureWrapping { Repeat, ClampToEdge };

    struct TextureSpecification {
        uint32_t width, height;
        TextureFormat format = TextureFormat::RGBA8;
        TextureFiltering filtering = TextureFiltering::Linear;
        TextureWrapping wrapping = TextureWrapping::Repeat;
        bool generateMips = false;
    };

    class Texture {
        friend class OpenGLShader;
        friend class OpenGLFramebuffer;

        public:
        virtual void SetFiltering(TextureFiltering minFilter, TextureFiltering magFilter) const = 0;
        virtual void SetWrapping(TextureWrapping wrap) const = 0;
        virtual void SetData(const void* pixels, size_t /*sizeBytes*/, int level) = 0;

        static Texture* Create(std::string texturePath);
        static Texture* Create(TextureSpecification spec);

        private:
        virtual void MapFormat(TextureFormat f, unsigned& internal, unsigned& format, unsigned& type) = 0;

        protected:
        std::string m_Path;
        uint32_t m_Width, m_Height;
        bool m_IsLoaded;
        unsigned int m_InternalFormat, m_DataFormat;

        protected:
        static bool IsDepth(TextureFormat f);

        protected:
        uint32_t m_RendererID;
        TextureSpecification m_Spec;
    };

}  // namespace Svarn
