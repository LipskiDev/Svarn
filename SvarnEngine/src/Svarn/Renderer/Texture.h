#pragma once

#include <cstdint>

namespace Svarn {

    enum class TextureFiltering { Nearest, Linear };

    enum class TextureWrapping { Repeat, ClampToEdge };

    class Texture {
        public:
        virtual void Bind(uint32_t unit) const = 0;
        virtual void SetFiltering(TextureFiltering minFilter, TextureFiltering magFilter) const = 0;
        virtual void SetWrapping(TextureWrapping wrap) const = 0;

        static Texture* Create(std::string texturePath);

        protected:
        std::string m_Path;
        uint32_t m_Width, m_Height;
        bool m_IsLoaded;
        unsigned int m_InternalFormat, m_DataFormat;

        protected:
        uint32_t m_RendererID;
    };

}  // namespace Svarn
