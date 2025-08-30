#include <svpch.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include <sys/types.h>
#include "Svarn/Core.h"
#include "Svarn/Renderer/Shader.h"
#include "glad/gl.h"
#include "glm/gtc/type_ptr.hpp"

namespace Svarn {

    OpenGLShader::OpenGLShader() { m_ShaderID = glCreateProgram(); };

    OpenGLShader::~OpenGLShader() { glDeleteShader(m_ShaderID); };

    void OpenGLShader::Attach(ShaderStage stage, const std::string& path) {
        if (stage == ShaderStage::Compute) {
            if (m_Stages.test(0) || m_Stages.test(1) || m_Stages.test(2) || m_Stages.test(3) || m_Stages.test(4)) {
                SV_ASSERT(false, "Can't attach Compute Shader to Graphical Shader Pipeline.");
                return;
            }
            m_IsCompute = true;
        } else {
            if (IsComputeShader()) {
                SV_ASSERT(false, "Can't attach Graphical Shader to Computer Shader Pipeline");
                return;
            }
        }

        m_Stages.set(int(stage), 1);
        m_ShaderPaths[stage] = path;
        m_ShaderIDs[stage] = CompileShader(stage, path);
    }

    void OpenGLShader::Link() const {
        for (auto& shader : m_ShaderIDs) {
            glAttachShader(m_ShaderID, shader.second);
        }

        glLinkProgram(m_ShaderID);

        GLint isLinked = 0;
        glGetProgramiv(m_ShaderID, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(m_ShaderID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_ShaderID, maxLength, &maxLength, &infoLog[0]);
            glDeleteProgram(m_ShaderID);
            for (auto& shader : m_ShaderIDs) {
                glDeleteShader(shader.second);
            }

            SV_CORE_ERROR("{0}", infoLog.data());
            SV_CORE_ASSERT(false, "Shader link failure!");

            return;
        }

        for (auto& shader : m_ShaderIDs) {
            glDetachShader(m_ShaderID, shader.second);
        }
    }

    bool OpenGLShader::IsComputeShader() const { return m_IsCompute; }

    void OpenGLShader::Bind() { glUseProgram(m_ShaderID); };
    void OpenGLShader::Unbind() { glDeleteProgram(m_ShaderID); };

    void OpenGLShader::Dispatch(uint32_t groupsX, uint32_t groupsY, uint32_t groupsZ) { glDispatchCompute(groupsX, groupsY, groupsZ); }

    void OpenGLShader::SetFloat(const std::string& uniformName, const float& value) {
        GLint uniformLocation = glGetUniformLocation(m_ShaderID, uniformName.c_str());
        glUniform1f(uniformLocation, value);
    };

    void OpenGLShader::SetBool(const std::string& uniformName, const bool& value) {
        GLint uniformLocation = glGetUniformLocation(m_ShaderID, uniformName.c_str());
        glUniform1i(uniformLocation, value);
    };

    void OpenGLShader::SetMat4(const std::string& uniformName, const glm::mat4& value) {
        GLint uniformLocation = glGetUniformLocation(m_ShaderID, uniformName.c_str());
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
    };

    void OpenGLShader::SetVec3(const std::string& uniformName, const glm::vec3& value) {
        GLint uniformLocation = glGetUniformLocation(m_ShaderID, uniformName.c_str());
        glUniform3f(uniformLocation, value.x, value.y, value.z);
    };

    void OpenGLShader::BindTexture(const std::string& uniformName, std::shared_ptr<Texture> texture) {
        GLint uniformLocation = glGetUniformLocation(m_ShaderID, uniformName.c_str());
        glActiveTexture(GL_TEXTURE0 + m_ActiveTextures);
        glBindTexture(GL_TEXTURE_2D, texture->m_RendererID);
        glUniform1i(uniformLocation, m_ActiveTextures);
        ++m_ActiveTextures;
    }

    GLuint OpenGLShader::CompileShader(ShaderStage stage, std::string path) {
        std::string src = ReadFile(path);

        GLuint shader = glCreateShader(toGLenum(stage));

        const GLchar* source = src.c_str();
        glShaderSource(shader, 1, &source, 0);

        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(shader);
            SV_CORE_ERROR("{0}", infoLog.data());
            SV_CORE_ASSERT(false, "Vertex Shader compilation failed.");
        }
        return shader;
    };

    void OpenGLShader::LinkShaders(GLuint vertexShader, GLuint fragmentShader) {
        glAttachShader(m_ShaderID, vertexShader);
        glAttachShader(m_ShaderID, fragmentShader);

        glLinkProgram(m_ShaderID);

        GLint isLinked = 0;
        glGetProgramiv(m_ShaderID, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(m_ShaderID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_ShaderID, maxLength, &maxLength, &infoLog[0]);
            glDeleteProgram(m_ShaderID);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            SV_CORE_ERROR("{0}", infoLog.data());
            SV_CORE_ASSERT(false, "Shader link failure!");

            return;
        }

        glDetachShader(m_ShaderID, vertexShader);
        glDetachShader(m_ShaderID, fragmentShader);
    };

    void OpenGLShader::ReloadShader() {
        for (std::pair<ShaderStage, std::string> pair : m_ShaderPaths) {
            ShaderStage stage = pair.first;
            std::string path = pair.second;

            Attach(stage, path);
        }

        Link();
        SV_INFO("Shaders reloaded successfully.");
    };

    std::string OpenGLShader::ReadFile(const std::string& filepath) {
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (!in) {
            SV_CORE_ERROR("Could not open file '{0}'", filepath);
            return {};
        }

        in.seekg(0, std::ios::end);
        auto end = in.tellg();
        if (end == -1) {
            SV_CORE_ERROR("Could not read from file '{0}'", filepath);
            return {};
        }

        std::string result(static_cast<size_t>(end), '\0');
        in.seekg(0, std::ios::beg);
        in.read(&result[0], result.size());

        return result;
    }
}  // namespace Svarn
