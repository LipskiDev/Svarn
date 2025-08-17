#include <svpch.h>
#include <Svarn/Renderer/Shader.h>

#include <glad/gl.h>
#include "Svarn/Core.h"

namespace Svarn {
    Shader::Shader(const std::string& vertexSrc,
                   const std::string& fragmentSrc) {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        const GLchar* source = vertexSrc.c_str();
        glShaderSource(vertexShader, 1, &source, 0);

        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength,
                               &infoLog[0]);

            glDeleteShader(vertexShader);
            SV_CORE_ERROR("{0}", infoLog.data());
            SV_CORE_ASSERT(false, "Vertex Shader compilation failed.");
            return;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        source = fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength,
                               &infoLog[0]);
            glDeleteShader(fragmentShader);

            glDeleteShader(vertexShader);

            SV_CORE_ERROR("{0}", infoLog.data());

            SV_CORE_ASSERT(false, "Fragment shader compilation failure!");

            return;
        }

        m_ShaderID = glCreateProgram();

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

    Shader::~Shader() { glDeleteShader(m_ShaderID); };

    void Shader::Bind() { glUseProgram(m_ShaderID); };

    void Shader::Unbind() { glUseProgram(0); };
}  // namespace Svarn
