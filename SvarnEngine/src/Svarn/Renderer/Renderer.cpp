#include <svpch.h>

#include <Svarn/Renderer/Renderer.h>
#include <memory>
#include "Svarn/Log.h"
#include "Svarn/Renderer/Framebuffer.h"
#include "Svarn/Renderer/VertexArray.h"
#include <Svarn/Renderer/ShaderLibrary.h>
#include <Svarn/Renderer/Primitives.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Svarn {

    void Renderer::Init() {
        GetShaderLibrary().Init();
        FramebufferSpecification depthMapSpec;
        depthMapSpec.attachments = {{TextureFormat::Depth24, AttachmentType::Depth}};
        depthMapSpec.height = 1024;
        depthMapSpec.width = 1024;

        m_DepthMapLight.reset(Framebuffer::Create(depthMapSpec));

        m_DepthShader = GetShaderLibrary().Get("DepthPass");
        if (!m_DepthShader) SV_CORE_INFO("DepthPass Shader not found");

        m_PBRShader = GetShaderLibrary().Get("PBR");
        if (!m_PBRShader) SV_CORE_INFO("PBR Shader not found");

        m_TerrainShader = GetShaderLibrary().Get("Terrain");
        if (!m_TerrainShader) SV_CORE_INFO("Terrain Shader not found");

        m_PatchMesh = Primitives::Grid(512, 512, 1);

        SV_CORE_TRACE("Successfully initialized Renderer");
    }

    void Renderer::BeginScene(const std::shared_ptr<Camera>& camera) {
        m_ViewMatrix = camera->GetViewMatrix();
        m_ProjectionMatrix = camera->GetProjection();
        m_VP = camera->GetViewProjection();
        m_Camera = camera;
    }

    void Renderer::EndScene() {
        Clear();

        // Depth Pass from Global directional Light source
        m_DepthShader->Bind();
        m_DepthMapLight->Bind();

        m_DepthShader->SetMat4("u_DirectionalLightTransformMatrix", m_DirectionalLight->CalculateLightTTransform());

        for (const RenderObject& obj : renderQueue) {
            obj.vertexArray->Bind();

            m_DepthShader->SetMat4("u_ModelMatrix", obj.transform.GetModelMatrix());

            RenderCommand::DrawIndexed(obj.vertexArray);
        }
        m_DepthMapLight->Unbind();
        m_DepthShader->Unbind();

        // Physically based Rendering pass using shadow mapping from direction light

        m_PBRShader->Bind();
        m_PBRShader->BindTexture("shadowMap", m_DepthMapLight->GetDepthAttachment());

        if (m_DirectionalLight) {
            m_PBRShader->SetVec3("u_DirLight.direction", m_DirectionalLight->m_LightDirection);
            m_PBRShader->SetVec3("u_DirLight.radiance", m_DirectionalLight->m_LightRadiance);
        }
        m_PBRShader->SetMat4("u_DirectionalLightTransformMatrix", m_DirectionalLight->CalculateLightTTransform());
        for (const RenderObject& obj : renderQueue) {
            obj.vertexArray->Bind();
            m_PBRShader->SetMat4("u_ProjectionMatrix", m_ProjectionMatrix);
            m_PBRShader->SetMat4("u_ViewMatrix", m_ViewMatrix);
            m_PBRShader->SetVec3("u_CameraPosition", m_Camera->GetPosition());

            m_PBRShader->SetMat4("u_ModelMatrix", obj.transform.GetModelMatrix());

            obj.material.BindToShader(m_PBRShader);
            RenderCommand::DrawIndexed(obj.vertexArray);
        }
        m_PBRShader->Unbind();

        m_DepthMapLight->Unbind();

        // Terrain Rendering
        if (m_ShouldRenderTerrain) {
            m_TerrainShader->Bind();
            m_TerrainShader->SetMat4("u_ProjectionMatrix", m_ProjectionMatrix);
            m_TerrainShader->SetMat4("u_ViewMatrix", m_ViewMatrix);
            m_TerrainShader->SetMat4("u_ModelMatrix", glm::mat4(1.0));
            RenderCommand::DrawIndexed(m_PatchMesh->GetVertexArray());
        }

        m_DepthMapLight->Clear();
        renderQueue.clear();
    }

    void Renderer::Clear() { RenderCommand::Clear(); }

    void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const Material& material, const Transform& t) {
        RenderObject obj(vertexArray, t, material);
        renderQueue.push_back(obj);
    }

    void Renderer::Submit(const std::shared_ptr<Mesh>& mesh, const Transform& t) {
        RenderObject obj(mesh->GetVertexArray(), t, mesh->GetMaterial());
        renderQueue.push_back(obj);
    }

    void Renderer::Submit(const std::shared_ptr<Model>& model, const Transform& t) {
        auto meshes = model->GetAllMeshes();
        for (auto& mesh : meshes) {
            RenderObject obj(mesh->GetVertexArray(), t, mesh->GetMaterial());

            renderQueue.push_back(obj);
        }
    }

    void Renderer::Submit(const std::shared_ptr<DirectionalLight>& directionalLight) { m_DirectionalLight = directionalLight; };

    void Renderer::RenderTerrain(bool shouldRenderTerrain) { m_ShouldRenderTerrain = shouldRenderTerrain; }

    // TODO: implement shader library so that i can just call 'DrawToScreen(texture);' and the correct shader + quad is chosen and generated
    void Renderer::DrawToScreen(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Shader>& shader) {
        RenderCommand::DisableDepthTest();
        shader->Bind();
        RenderCommand::DrawIndexed(mesh->GetVertexArray());
        shader->Unbind();
        RenderCommand::EnableDepthTest();
    }

    Renderer& GetRenderer() {
        static Renderer instance;
        return instance;
    }

    const void* RendererAddress() { return static_cast<const void*>(&GetRenderer()); }

}  // namespace Svarn
