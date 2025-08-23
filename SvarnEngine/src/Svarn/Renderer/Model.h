#pragma once

#include <memory>
#include "Svarn/Renderer/Mesh.h"
#include "assimp/scene.h"
namespace Svarn {
    class Model {
        public:
        static Model* Create(const std::string& path);

        std::vector<std::shared_ptr<Mesh>> GetAllMeshes() const;

        private:
        Model(const std::string& path);
        void LoadModel(const std::string& path);
        void processNode(::aiNode* node, const ::aiScene* scene);
        std::shared_ptr<Mesh> processMesh(::aiMesh* mesh);

        private:
        std::vector<std::shared_ptr<Mesh>> m_Meshes;
        std::string m_Directory;
    };
}  // namespace Svarn
