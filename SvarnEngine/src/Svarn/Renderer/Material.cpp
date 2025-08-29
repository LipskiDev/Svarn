#include <svpch.h>

#include <Svarn/Renderer/Material.h>
#include "Svarn/Renderer/Shader.h"
#include "imgui.h"

namespace Svarn {
    Material* Material::FromTextures(std::shared_ptr<Texture> albedoTexture, std::shared_ptr<Texture> normalTexture,
                                     std::shared_ptr<Texture> roughnessTexture, std::shared_ptr<Texture> metallicTexture) {
        return new Material(albedoTexture, normalTexture, roughnessTexture, metallicTexture);
    }

    Material* Material::FromValues(glm::vec3 color, float roughness, float metallic) { return new Material(color, roughness, metallic); }

    Material* Material::New() { return new Material(); }

    Material::Material(std::shared_ptr<Texture> albedoTexture, std::shared_ptr<Texture> normalTexture, std::shared_ptr<Texture> roughnessTexture,
                       std::shared_ptr<Texture> metallicTexture) {
        m_AlbedoTexture = albedoTexture;
        m_NormalTexture = normalTexture;
        m_RoughnessTexture = roughnessTexture;
        m_MetallicTexture = metallicTexture;

        m_UseAlbedoTexture = true;
        m_UseNormalTexture = true;
        m_UseRoughnessTexture = true;
        m_UseMetallicTexture = true;
    }

    Material::Material(glm::vec3 color, float roughness, float metallic) {
        m_AlbedoValue = color;
        m_RoughnessValue = roughness;
        m_MetallicValue = metallic;

        m_UseAlbedoTexture = false;
        m_UseNormalTexture = false;
        m_UseRoughnessTexture = false;
        m_UseMetallicTexture = false;
    }

    void Material::BindToShader(std::shared_ptr<Shader> shader) {
        shader->Bind();

        if (m_UseAlbedoTexture) {
            shader->SetBool("material.useAlbedoTexture", true);
            m_AlbedoTexture->Bind(0);
        } else {
            shader->SetBool("material.useAlbedoTexture", false);
            shader->SetVec3("material.albedo", m_AlbedoValue);
        }

        if (m_UseNormalTexture) {
            shader->SetBool("material.useNormalTexture", true);
            m_NormalTexture->Bind(1);
        } else {
            shader->SetBool("material.useNormalTexture", false);
        }

        if (m_UseRoughnessTexture) {
            shader->SetBool("material.useRoughnessTexture", true);
            m_RoughnessTexture->Bind(2);
        } else {
            shader->SetBool("material.useRoughnessTexture", false);
            shader->SetFloat("material.roughness", m_RoughnessValue);
        }

        if (m_UseMetallicTexture) {
            shader->SetBool("material.useMetallicTexture", true);
            m_MetallicTexture->Bind(3);
        } else {
            shader->SetBool("material.useMetallicTexture", false);
            shader->SetFloat("material.metallic", m_MetallicValue);
        }
    };

    void Material::SetAlbedoValue(glm::vec3 color) {
        m_AlbedoValue = color;
        m_UseAlbedoTexture = false;
    };

    void Material::SetRoughnessValue(float roughness) {
        m_RoughnessValue = roughness;
        m_UseRoughnessTexture = false;
    };

    void Material::SetMetallicValue(float metallic) {
        m_MetallicValue = metallic;
        m_UseMetallicTexture = false;
    };

    void Material::SetAlbedoTexture(std::shared_ptr<Texture> albedoTexture) {
        m_AlbedoTexture = albedoTexture;
        m_UseAlbedoTexture = true;
    };

    void Material::SetNormalTexture(std::shared_ptr<Texture> normalTexture) {
        m_NormalTexture = normalTexture;
        m_UseNormalTexture = true;
    };

    void Material::SetRoughnessTexture(std::shared_ptr<Texture> roughnessTexture) {
        m_RoughnessTexture = roughnessTexture;
        m_UseRoughnessTexture = true;
    };

    void Material::SetMetallicTexture(std::shared_ptr<Texture> metallicTexture) {
        m_MetallicTexture = metallicTexture;
        m_UseMetallicTexture = true;
    };

}  // namespace Svarn
