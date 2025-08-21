#include <svpch.h>
#include <Platform/OpenGL/OpenGLTexture.h>
#include <glad/gl.h>
#include <sstream>
#include <string>
#include "Svarn/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_SIMD
#include <stb_image/stb_image.h>

namespace Svarn {
    OpenGLTexture::OpenGLTexture(std::string texturePath) {
        int width = 0;
        int height = 0;
        int channels = 0;
        ::stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = nullptr;
        {
            data = ::stbi_load(texturePath.c_str(), &width, &height, &channels, 4);
        }

        if (!data) {
            ::stbi_image_free(data);
            SV_CORE_ERROR("Failed to load texture at path {0}", texturePath);
            return;
        }

        InvalidateImpl(texturePath, static_cast<uint32_t>(width), static_cast<uint32_t>(height), data, static_cast<uint32_t>(channels));

        ::stbi_image_free(data);
    }
    void OpenGLTexture::Bind(uint32_t unit) const {
        glBindTextureUnit(unit, m_RendererID);
        std::stringstream ss;
        ss << "texture" << unit;
        glUniform1i(glGetUniformLocation(m_RendererID, ss.str().c_str()), unit);  // sampler -> unit 0};
    }

    void OpenGLTexture::SetFiltering(TextureFiltering minFilter, TextureFiltering magFilter) const {
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        GLenum glMin = (minFilter == TextureFiltering::Linear) ? GL_LINEAR_MIPMAP_LINEAR  // trilinear if you have mips
                                                               : GL_NEAREST_MIPMAP_NEAREST;

        GLenum glMag = (magFilter == TextureFiltering::Linear) ? GL_LINEAR : GL_NEAREST;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glMag);
    };
    void OpenGLTexture::SetWrapping(TextureWrapping wrap) const {
        glBindTexture(GL_TEXTURE_2D, m_RendererID);

        GLenum glWrap = (wrap == TextureWrapping::Repeat) ? GL_REPEAT : GL_CLAMP_TO_EDGE;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrap);
    };

    void OpenGLTexture::InvalidateImpl(std::string_view path, uint32_t width, uint32_t height, const void* data, uint32_t channels) {
        m_Path = path;
        SV_CORE_TRACE("Loading texture from path: {}", m_Path);
        m_IsLoaded = true;
        m_Width = width;
        m_Height = height;

        GLenum internalFormat = 0;
        GLenum dataFormat = 0;
        switch (channels) {
            case 1:
                internalFormat = GL_R8;
                dataFormat = GL_RED;
                SV_CORE_TRACE("Texture channel count is 1. Internal format is: {}. Data Format is: {}.", internalFormat, dataFormat);
                break;
            case 2:
                internalFormat = GL_RG8;
                dataFormat = GL_RG;
                SV_CORE_TRACE("Texture channel count is 2. Internal format is: {}. Data Format is: {}.", internalFormat, dataFormat);
                break;
            case 3:
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
                SV_CORE_TRACE("Texture channel count is 3. Internal format is: {}. Data Format is: {}.", internalFormat, dataFormat);
                break;
            case 4:
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
                SV_CORE_TRACE("Texture channel count is 4. Internal format is: {}. Data Format is: {}.", internalFormat, dataFormat);
                break;
            default:
                SV_CORE_ERROR("Texture channel count is not within (1-4) range. Channel count: {}", channels);
                return;
        }

        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;

        SV_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");
        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, internalFormat, static_cast<int>(m_Width), static_cast<int>(m_Height));

        // Calculate memory usage based on channels and dimensions
        size_t textureMemory = static_cast<size_t>(m_Width) * m_Height * channels;
        // Track GPU memory allocation
        std::string textureName = "OpenGL Texture2D: " + std::string(path);

        SV_CORE_INFO("Loaded texture {0}, Texture Size {1}", textureName, textureMemory);

        // Register with GPU Resource Inspector

        // NOTE: Texture Wrapping
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // NOTE: Texture Filtering
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_RendererID, 0, 0, 0, static_cast<int>(m_Width), static_cast<int>(m_Height), dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(m_RendererID);
    }

}  // namespace Svarn
