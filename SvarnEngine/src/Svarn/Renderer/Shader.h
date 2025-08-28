#pragma once

#include <bitset>
#include <string>
#include <glm/glm.hpp>
#include "glad/gl.h"

namespace Svarn {

    enum class ShaderStage { Vertex, Fragment, Geometry, TessControl, TessEvaluation, Compute };

    class Shader {
        public:
        virtual ~Shader() {};

        virtual void Attach(ShaderStage stage, const std::string& path) = 0;
        virtual void Link() const = 0;
        virtual bool IsComputeShader() const = 0;

        // Graphics Functions
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        // Compute Functions
        virtual void Dispatch(uint32_t groupsX, uint32_t groupsY, uint32_t groupsZ) = 0;

        virtual void SetFloat(const std::string& name, const float& value) = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
        virtual void SetVec3(const std::string& name, const glm::vec3& value) = 0;

        virtual void ReloadShader() = 0;

        static Shader* Create();

        protected:
        bool m_IsCompute = false;
        std::bitset<6> m_Stages;
    };

}  // namespace Svarn
