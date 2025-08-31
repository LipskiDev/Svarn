#include <svpch.h>
#include <Platform/OpenGL/OpenGLTexture.h>
#include <glad/gl.h>
#include <sstream>
#include <string>
#include "Svarn/Log.h"
#include "Svarn/Renderer/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_SIMD
#include <stb_image/stb_image.h>

namespace Svarn {

    void OpenGLTexture::MapFormat(TextureFormat f, unsigned& internal, unsigned& format, unsigned& type) {
        switch (f) {
            case TextureFormat::RGBA8:
                internal = GL_RGBA8;
                format = GL_RGBA;
                type = GL_UNSIGNED_BYTE;
                break;
            case TextureFormat::RGBA16F:
                internal = GL_RGBA16F;
                format = GL_RGBA;
                type = GL_HALF_FLOAT;
                break;
            case TextureFormat::R11F_G11F_B10F:
                internal = GL_R11F_G11F_B10F;
                format = GL_RGB;
                type = GL_UNSIGNED_INT_10F_11F_11F_REV;
                break;
            case TextureFormat::R32F:
                internal = GL_R32F;
                format = GL_RED;
                type = GL_FLOAT;
                break;
            case TextureFormat::Depth24:
                internal = GL_DEPTH_COMPONENT24;
                format = GL_DEPTH_COMPONENT;
                type = GL_FLOAT;
                break;
            case TextureFormat::Depth32F:
                internal = GL_DEPTH_COMPONENT32F;
                format = GL_DEPTH_COMPONENT;
                type = GL_FLOAT;
                break;
            case TextureFormat::Depth24Stencil8:
                internal = GL_DEPTH24_STENCIL8;
                format = GL_DEPTH_STENCIL;
                type = GL_UNSIGNED_INT_24_8;
                break;
            default:
                internal = GL_RGBA8;
                format = GL_RGBA;
                type = GL_UNSIGNED_BYTE;
                break;
        }
    }

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

    OpenGLTexture::OpenGLTexture(TextureSpecification spec) {
        m_Spec = spec;
        m_Width = spec.width;
        m_Height = spec.height;
        m_IsLoaded = false;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

        unsigned internal, format, type;
        MapFormat(spec.format, internal, format, type);

        // Allocate immutable storage
        const int levels = spec.generateMips ? (1 + (int)std::floor(std::log2((float)std::max(spec.width, spec.height)))) : 1;

        glTextureStorage2D(m_RendererID, levels, internal, (GLsizei)spec.width, (GLsizei)spec.height);

        SetFiltering(spec.filtering, spec.filtering);
        SetWrapping(spec.wrapping);
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

    void OpenGLTexture::SetData(const void* pixels, size_t /*sizeBytes*/, int level) {
        unsigned internal, format, type;
        MapFormat(m_Spec.format, internal, format, type);
        glTextureSubImage2D(m_RendererID, level, 0, 0, (GLsizei)m_Spec.width, (GLsizei)m_Spec.height, format, type, pixels);
        if (m_Spec.generateMips) glGenerateTextureMipmap(m_RendererID);
    }

    void OpenGLTexture::InvalidateImpl(std::string_view path, uint32_t width, uint32_t height, const void* data, uint32_t channels) {
        m_Path = path;
        m_IsLoaded = true;
        m_Width = width;
        m_Height = height;
        SV_CORE_TRACE("Loading texture from path: {0} with width {1} and height {2}", m_Path, m_Width, m_Height);

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
