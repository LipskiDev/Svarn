#include <svpch.h>

#include <Svarn/Renderer/Renderer.h>
#include <filesystem>
#include <memory>
#include "Svarn/Log.h"
#include "Svarn/Renderer/VertexArray.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <queue>

namespace Svarn {

    std::queue<RenderObject> Renderer::renderQueue = std::queue<RenderObject>();

    std::vector<std::shared_ptr<Shader>> Renderer::loadedShaders = std::vector<std::shared_ptr<Shader>>();

    std::shared_ptr<DirectionalLight> Renderer::m_DirectionalLight = nullptr;

    void Renderer::BeginScene(const std::shared_ptr<Camera>& camera) {
        m_ViewMatrix = camera->GetViewMatrix();
        m_ProjectionMatrix = camera->GetProjection();
        m_VP = camera->GetViewProjection();
        m_CameraPosition = camera->GetPosition();
    }

    void Renderer::EndScene() {
        while (!renderQueue.empty()) {
            const RenderObject& obj = renderQueue.front();
            obj.vertexArray->Bind();
            obj.m_Shader->Bind();
            loadedShaders.push_back(obj.m_Shader);
            obj.m_Shader->SetMat4("VP", m_VP);
            obj.m_Shader->SetMat4("modelMatrix", obj.transform.GetModelMatrix());
            obj.m_Shader->SetMat4("viewMatrix", m_ViewMatrix);
            obj.m_Shader->SetMat4("projectionMatrix", m_ProjectionMatrix);
            obj.m_Shader->SetVec3("u_CameraPosition", m_CameraPosition);

            obj.material.BindToShader(obj.m_Shader);

            if (m_DirectionalLight) {
                obj.m_Shader->SetVec3("u_DirLight.direction", m_DirectionalLight->m_LightDirection);
                obj.m_Shader->SetVec3("u_DirLight.radiance", m_DirectionalLight->m_LightRadiance);
            }

            obj.vertexArray->Bind();
            RenderCommand::DrawIndexed(obj.vertexArray);
            renderQueue.pop();
        }

        for (std::shared_ptr<Shader> shader : loadedShaders) {
            shader->m_ActiveTextures = 0;
        }
        loadedShaders.clear();
    }

    void Renderer::Clear() { RenderCommand::Clear(); }

    void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const Material& material,
                          const Transform& t) {
        RenderObject obj(vertexArray, t, shader, material);
        renderQueue.push(obj);
    }

    void Renderer::Submit(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Shader>& shader, const Transform& t) {
        RenderObject obj(mesh->GetVertexArray(), t, shader, mesh->GetMaterial());
        renderQueue.push(obj);
    }

    void Renderer::Submit(const std::shared_ptr<Model>& model, const std::shared_ptr<Shader>& shader, const Transform& t) {
        auto meshes = model->GetAllMeshes();
        for (auto& mesh : meshes) {
            RenderObject obj(mesh->GetVertexArray(), t, shader, mesh->GetMaterial());

            renderQueue.push(obj);
        }
    }

    void Renderer::Submit(const std::shared_ptr<DirectionalLight>& directionalLight) { m_DirectionalLight = directionalLight; };

    // TODO: implement shader library so that i can just call 'DrawToScreen(texture);' and the correct shader + quad is chosen and generated
    void Renderer::DrawToScreen(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Shader>& shader) {
        RenderCommand::DisableDepthTest();
        shader->Bind();
        RenderCommand::DrawIndexed(mesh->GetVertexArray());
        shader->Unbind();
        RenderCommand::EnableDepthTest();
    }
}  // namespace Svarn
