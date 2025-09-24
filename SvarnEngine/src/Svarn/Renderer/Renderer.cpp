#include <svpch.h>

#include <Svarn/Renderer/Renderer.h>
#include <memory>
#include "Svarn/Log.h"
#include "Svarn/Renderer/Framebuffer.h"
#include "Svarn/Renderer/VertexArray.h"
#include <Svarn/Renderer/ShaderLibrary.h>

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

        m_PBRShader->Bind();
        m_PBRShader->BindTexture("shadowMap", m_DepthMapLight->GetDepthAttachment());

        m_PBRShader->SetMat4("u_ProjectionMatrix", m_ProjectionMatrix);
        m_PBRShader->SetMat4("u_ViewMatrix", m_ViewMatrix);
        m_PBRShader->SetVec3("u_CameraPosition", m_Camera->GetPosition());
        if (m_DirectionalLight) {
            m_PBRShader->SetVec3("u_DirLight.direction", m_DirectionalLight->m_LightDirection);
            m_PBRShader->SetVec3("u_DirLight.radiance", m_DirectionalLight->m_LightRadiance);
        }
        m_PBRShader->SetMat4("u_DirectionalLightTransformMatrix", m_DirectionalLight->CalculateLightTTransform());
        for (const RenderObject& obj : renderQueue) {
            obj.vertexArray->Bind();

            m_PBRShader->SetMat4("u_ModelMatrix", obj.transform.GetModelMatrix());

            obj.material.BindToShader(m_PBRShader);
            RenderCommand::DrawIndexed(obj.vertexArray);
        }
        m_PBRShader->Unbind();

        m_DepthMapLight->Unbind();

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
