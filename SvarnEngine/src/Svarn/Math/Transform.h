#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/fwd.hpp>
#include "glm/ext/matrix_transform.hpp"
namespace Svarn {
    struct Transform {
        glm::vec3 m_Translation;
        glm::vec3 m_Scale;
        glm::quat m_Rotation;

        Transform(glm::vec3 translation, glm::vec3 scale, glm::vec3 rotation) {
            m_Translation = translation;
            m_Scale = scale;
            m_Rotation = glm::quat(rotation);
        }

        glm::mat4 GetModelMatrix() {
            glm::mat4 base = glm::mat4(1.0);
            // base = glm::translate(base, m_Translation);
            // base = base * glm::toMat4(m_Rotation);
            // base = glm::scale(base, m_Scale);
            return base;
        }

        void Rotate(glm::vec3 rotation) {
            glm::quat q = glm::quat(rotation);
            m_Rotation = q * m_Rotation;
        }

        void Translate(glm::vec3 translation) { m_Translation += translation; }

        static Transform Default() { return Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f)); }
    };
}  // namespace Svarn
