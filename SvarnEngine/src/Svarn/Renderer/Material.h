#pragma once

#include "Svarn/Renderer/Shader.h"
#include "Svarn/Renderer/Texture.h"
namespace Svarn {
    class Material {
        public:
        std::string m_MaterialName;

        static Material FromTextures(std::string materialName, std::shared_ptr<Texture> albedoTexture, std::shared_ptr<Texture> normalTexture,
                                     std::shared_ptr<Texture> roughnessTexture, std::shared_ptr<Texture> metallicTexture);

        static Material FromValues(std::string materialName, glm::vec3 color, float roughness, float metallic);

        static Material New();

        void BindToShader(std::shared_ptr<Shader> shader) const;

        void SetAlbedoValue(glm::vec3 color);
        void SetRoughnessValue(float roughness);
        void SetMetallicValue(float metallic);

        void SetAlbedoTexture(std::shared_ptr<Texture> albedoTexture);
        void SetNormalTexture(std::shared_ptr<Texture> normalTexture);
        void SetRoughnessTexture(std::shared_ptr<Texture> roughnessTexture);
        void SetMetallicTexture(std::shared_ptr<Texture> metallicTexture);

        private:
        Material(std::string materialName, std::shared_ptr<Texture> albedoTexture, std::shared_ptr<Texture> normalTexture,
                 std::shared_ptr<Texture> roughnessTexture, std::shared_ptr<Texture> metallicTexture);

        Material(std::string materialName, glm::vec3 color, float roughness, float metallic);

        bool m_UseAlbedoTexture = false;
        bool m_UseNormalTexture = false;
        bool m_UseRoughnessTexture = false;
        bool m_UseMetallicTexture = false;

        // Textures
        std::shared_ptr<Texture> m_AlbedoTexture;

        std::shared_ptr<Texture> m_NormalTexture;

        std::shared_ptr<Texture> m_RoughnessTexture;

        std::shared_ptr<Texture> m_MetallicTexture;

        // Non-Texture Values
        glm::vec3 m_AlbedoValue = glm::vec3(1.0, 0.0, 0.0);
        float m_RoughnessValue = 1.0f;
        float m_MetallicValue = 1.0f;
    };
}  // namespace Svarn
