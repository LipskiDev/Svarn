#include <svpch.h>

#include <Svarn/Renderer/ShaderLibrary.h>
#include <utility>
#include "Svarn/Log.h"
#include "imgui.h"

namespace Svarn {

    void ShaderLibrary::Init() {
        std::shared_ptr<Shader> pbrShader;
        pbrShader.reset(Shader::Create());
        pbrShader->Attach(ShaderStage::Vertex, "SvarnEngine/src/Svarn/Renderer/Shaders/pbr.vs");
        pbrShader->Attach(ShaderStage::Fragment, "SvarnEngine/src/Svarn/Renderer/Shaders/pbr.fs");
        pbrShader->Link();
        Add("PBR", pbrShader);

        std::shared_ptr<Shader> depthPassShader;
        depthPassShader.reset(Shader::Create());
        depthPassShader->Attach(ShaderStage::Vertex, "SvarnEngine/src/Svarn/Renderer/Shaders/directional_shadow_map.vs");
        depthPassShader->Attach(ShaderStage::Fragment, "SvarnEngine/src/Svarn/Renderer/Shaders/directional_shadow_map.fs");
        depthPassShader->Link();
        Add("DepthPass", depthPassShader);

        std::shared_ptr<Shader> terrainShader;
        terrainShader.reset(Shader::Create());
        terrainShader->Attach(ShaderStage::Vertex, "SvarnEngine/src/Svarn/Renderer/Shaders/terrain.vs");
        terrainShader->Attach(ShaderStage::Fragment, "SvarnEngine/src/Svarn/Renderer/Shaders/terrain.fs");
        terrainShader->Link();
        Add("Terrain", terrainShader);

        SV_CORE_INFO("Successfully initialized ShaderLibrary");
    }

    std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& shaderName) const {
        auto it = m_ShaderLibrary.find(shaderName);
        if (it == m_ShaderLibrary.end()) {
            SV_ASSERT(false, "Could not find Shader {0}", shaderName);
            return nullptr;
        }
        return it->second;
    }

    void ShaderLibrary::Add(const std::string& name, std::shared_ptr<Shader> shader) { m_ShaderLibrary[name] = shader; }

    ShaderLibrary& GetShaderLibrary() {
        static ShaderLibrary instance;
        return instance;
    }

}  // namespace Svarn
