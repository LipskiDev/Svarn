#pragma once

#include <Svarn/Renderer/Texture.h>
#include <memory>
#include "Svarn/Log.h"
#include "Svarn/Renderer/Material.h"
#include "Svarn/Renderer/Mesh.h"
#include "assimp/scene.h"
namespace Svarn {
    class Model {
        public:
        static Model* Create(const std::string& path);

        std::vector<std::shared_ptr<Mesh>> GetAllMeshes() const;

        void SetMaterial(Material material) {
            m_Material = material;
            for (auto& m : m_Meshes) {
                m->SetMaterial(m_Material);
            }
        }

        Material GetMaterial() { return m_Material; }

        private:
        Model(const std::string& path);
        void LoadModel(const std::string& path);
        void processNode(::aiNode* node, const ::aiScene* scene);
        std::shared_ptr<Mesh> processMesh(::aiMesh* mesh);

        private:
        std::vector<std::shared_ptr<Mesh>> m_Meshes;
        std::string m_Directory;
        Material m_Material = Material::New();
    };
}  // namespace Svarn
