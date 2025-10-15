#include <svpch.h>

#include <Svarn/Renderer/Model.h>

#include <assimp/Importer.hpp>
#include <memory>
#include "Svarn/Log.h"
#include "glm/gtc/type_ptr.hpp"
#include <assimp/postprocess.h>

namespace Svarn {
    Model* Model::Create(const std::string& path) { return new Model(path); }

    Model::Model(const std::string& path) { LoadModel(path); }

    std::vector<std::shared_ptr<Mesh>> Model::GetAllMeshes() const { return m_Meshes; }

    void Model::LoadModel(const std::string& path) {
        Assimp::Importer importer;

        const ::aiScene* scene =
            importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_ImproveCacheLocality |
                                        aiProcess_JoinIdenticalVertices | aiProcess_LimitBoneWeights | aiProcess_SortByPType);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            throw std::runtime_error("Assimp error: " + std::string(importer.GetErrorString()));
        }

        m_Directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    void Model::processNode(aiNode* node, const aiScene* scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_Meshes.push_back(processMesh(mesh));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    std::shared_ptr<Mesh> Model::processMesh(aiMesh* mesh) {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex v;
            v.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

            if (mesh->HasNormals()) {
                v.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
            } else {
                v.normal = {0.0f, 0.0f, 0.0f};
            }

            if (mesh->mTextureCoords[0]) {
                v.uv = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            } else {
                v.uv = {0.0f, 0.0f};
            }

            if (mesh->HasTangentsAndBitangents()) {
                v.tangent = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z, 1};
                v.bitangent = {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z, 1};
            } else {
                v.tangent = {0.0f, 0.0f, 0.0f, 0.0f};
                v.bitangent = {0.0f, 0.0f, 0.0f, 0.0f};
            }

            vertices.push_back(v);
        }
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
        }
        std::shared_ptr<Mesh> processedMesh;
        processedMesh.reset(Mesh::Create(vertices, indices));
        processedMesh->SetMaterial(m_Material);
        SV_CORE_INFO("Setting Mesh material to: {0}", processedMesh->GetMaterial().m_MaterialName);
        return processedMesh;
    }
}  // namespace Svarn
