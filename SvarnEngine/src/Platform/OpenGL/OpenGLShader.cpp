#include <svpch.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include "glad/gl.h"
#include "glm/gtc/type_ptr.hpp"

namespace Svarn {

    OpenGLShader::OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath) {
        m_VertexPath = vertexPath;
        m_FragmentPath = fragmentPath;

        m_ShaderID = glCreateProgram();

        GLuint vertexShader = CompileVertexShader(vertexPath);
        GLuint fragmentShader = CompileFragmentShader(fragmentPath);

        LinkShaders(vertexShader, fragmentShader);
    };

    OpenGLShader::~OpenGLShader() { glDeleteShader(m_ShaderID); };

    void OpenGLShader::Bind() { glUseProgram(m_ShaderID); };
    void OpenGLShader::Unbind() { glDeleteProgram(m_ShaderID); };

    void OpenGLShader::SetMat4(const std::string& uniformName, const glm::mat4& value) {
        GLint uniformLocation = glGetUniformLocation(m_ShaderID, uniformName.c_str());
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
    };

    GLuint OpenGLShader::CompileVertexShader(std::string vertexPath) {
        std::string vertexSrc = ReadFile(vertexPath);

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
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(vertexShader);
            SV_CORE_ERROR("{0}", infoLog.data());
            SV_CORE_ASSERT(false, "Vertex Shader compilation failed.");
        }
        return vertexShader;
    };

    GLuint OpenGLShader::CompileFragmentShader(std::string fragmentPath) {
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        std::string fragmentSrc = ReadFile(fragmentPath);

        const GLchar* source = fragmentSrc.c_str();
        source = fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);
        glCompileShader(fragmentShader);

        GLint isCompiled = 0;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
            glDeleteShader(fragmentShader);

            SV_CORE_ERROR("{0}", infoLog.data());

            SV_CORE_ASSERT(false, "Fragment shader compilation failure!");
        }

        return fragmentShader;
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
        // 1) Compile stages
        GLuint vs = CompileVertexShader(m_VertexPath);
        GLuint fs = CompileFragmentShader(m_FragmentPath);
        if (!vs || !fs) {
            glDeleteShader(vs);
            glDeleteShader(fs);
            SV_CORE_ERROR("Reload aborted: compilation failed.");
        }

        // 2) Link into a TEMP program
        GLuint newProg = glCreateProgram();
        glAttachShader(newProg, vs);
        glAttachShader(newProg, fs);
        glLinkProgram(newProg);

        // shader objects are no longer needed after link
        glDeleteShader(vs);
        glDeleteShader(fs);

        GLint linked = GL_FALSE;
        glGetProgramiv(newProg, GL_LINK_STATUS, &linked);
        if (!linked) {
            GLint len = 0;
            glGetProgramiv(newProg, GL_INFO_LOG_LENGTH, &len);
            std::string log(len, '\0');
            glGetProgramInfoLog(newProg, len, nullptr, log.data());
            SV_CORE_ERROR("Link failed:\n{}", log);
            glDeleteProgram(newProg);
        }

        // 3) Swap programs atomically
        GLuint old = m_ShaderID;
        m_ShaderID = newProg;
        glUseProgram(m_ShaderID);  // make new one current
        glDeleteProgram(old);      // safe: deletion deferred if still in use

        // 4) (optional) re-apply cached uniforms/samplers if you have a cache
        // reapplyUniforms();

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
