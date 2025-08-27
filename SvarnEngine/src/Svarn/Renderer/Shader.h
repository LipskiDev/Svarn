#pragma once

#include <string>
#include <glm/glm.hpp>
#include "glad/gl.h"

namespace Svarn {

    enum class ShaderStage { Vertex, Fragment, Geometry, TessControl, TessEvaluation, Compute };

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

    class Shader {
        public:
        virtual ~Shader() {};

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
        virtual void SetVec3(const std::string& name, const glm::vec3& value) = 0;

        virtual void ReloadShader() = 0;

        static Shader* Create(const std::string& vertexPath, const std::string& fragmentPath);
    };

}  // namespace Svarn
