#pragma once

#include <glm/fwd.hpp>
#include "Svarn/Renderer/Texture.h"
namespace Svarn {
    class OpenGLTexture : public Texture {
        public:
        OpenGLTexture(std::string texturePath);
        virtual void Bind(uint32_t unit) const override;

        private:
        void InvalidateImpl(std::string_view path, uint32_t width, uint32_t height, const void* data, uint32_t channels);
    };
}  // namespace Svarn
