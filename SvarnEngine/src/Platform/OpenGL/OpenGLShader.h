#pragma once

#include "Svarn/Renderer/Shader.h"
#include "glad/gl.h"
namespace Svarn {
    class OpenGLShader : public Shader {
        public:
        OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath);
        virtual ~OpenGLShader() override;

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void SetMat4(const std::string& uniformName, const glm::mat4& value) override;
        virtual void SetVec3(const std::string& uniformName, const glm::vec3& value) override;

        virtual void ReloadShader() override;

        private:
        GLuint CompileVertexShader(std::string path);
        GLuint CompileFragmentShader(std::string path);
        void LinkShaders(GLuint vertexShader, GLuint fragmentShader);
        std::string ReadFile(const std::string& filepath);

        private:
        GLint m_ShaderID;

        std::string m_VertexPath, m_FragmentPath;
    };

}  // namespace Svarn
