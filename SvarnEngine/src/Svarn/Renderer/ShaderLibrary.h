#pragma once

#include <map>
namespace Svarn {
    class ShaderLibrary {
        public:
        void Init();

        std::shared_ptr<Shader> Get(const std::string& shaderName) const;
        void Add(const std::string& name, std::shared_ptr<Shader> shader);

        private:
        std::map<std::string, std::shared_ptr<Shader>> m_ShaderLibrary;
    };

    SVARN_API ShaderLibrary& GetShaderLibrary();
}  // namespace Svarn
