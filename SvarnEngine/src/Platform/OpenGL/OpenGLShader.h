#pragma once

#include "Svarn/Renderer/Shader.h"
#include "glad/gl.h"
namespace Svarn {
    class OpenGLShader : public Shader {
        public:
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~OpenGLShader() override;

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void SetMat4(const std::string& uniformName, const glm::mat4& value) override;

        private:
        GLint m_ShaderID;
    };

}  // namespace Svarn
