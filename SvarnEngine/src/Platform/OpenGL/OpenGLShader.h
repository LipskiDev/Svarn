#pragma once

#include <unordered_map>
#include "Svarn/Renderer/Shader.h"
#include "glad/gl.h"
namespace Svarn {

    static GLenum toGLenum(ShaderStage s) {
        switch (s) {
            case ShaderStage::Vertex:
                return GL_VERTEX_SHADER;
            case ShaderStage::Fragment:
                return GL_FRAGMENT_SHADER;
            case ShaderStage::Geometry:
                return GL_GEOMETRY_SHADER;
            case ShaderStage::TessControl:
                return GL_TESS_CONTROL_SHADER;
            case ShaderStage::TessEvaluation:
                return GL_TESS_EVALUATION_SHADER;
            case ShaderStage::Compute:
                return GL_COMPUTE_SHADER;
        }
        return 0;
    }

    class OpenGLShader : public Shader {
        public:
        OpenGLShader();
        virtual ~OpenGLShader() override;

        virtual void Attach(ShaderStage stage, const std::string& path) override;
        virtual void Link() const override;
        virtual bool IsComputeShader() const override;

        // Graphics Functions
        virtual void Bind() override;
        virtual void Unbind() override;

        // Compute Functions
        virtual void Dispatch(uint32_t groupsX, uint32_t groupsY, uint32_t groupsZ) override;

        virtual void SetMat4(const std::string& uniformName, const glm::mat4& value) override;
        virtual void SetVec3(const std::string& uniformName, const glm::vec3& value) override;

        virtual void ReloadShader() override;

        private:
        GLuint CompileShader(ShaderStage stage, std::string path);
        void LinkShaders(GLuint vertexShader, GLuint fragmentShader);
        std::string ReadFile(const std::string& filepath);

        std::unordered_map<ShaderStage, uint32_t> m_ShaderIDs;
        std::unordered_map<ShaderStage, std::string> m_ShaderPaths;

        private:
        GLint m_ShaderID;

        std::string m_VertexPath, m_FragmentPath;
    };

}  // namespace Svarn
