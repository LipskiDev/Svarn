#pragma once

#include <cstdint>

namespace Svarn {

    class Texture {
        public:
        virtual void Bind(uint32_t unit) const = 0;

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
