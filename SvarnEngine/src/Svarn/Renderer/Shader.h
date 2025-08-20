#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Svarn {

    class Shader {
        public:
        virtual ~Shader() {};

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

        virtual void ReloadShader() = 0;

        static Shader* Create(const std::string& vertexPath, const std::string& fragmentPath);
    };

}  // namespace Svarn
