#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Svarn/Log.h"

namespace Svarn {

    class Camera {
        public:
        Camera() = default;

        virtual ~Camera() = default;

        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewProjection() const { return m_ViewProjectionMatrix; }
        const glm::vec3& GetPosition() const { return m_Position; }
        const glm::quat& GetRotation() const { return m_Rotation; }

        protected:
        glm::vec3 m_Position = glm::vec3(0.0);
        glm::quat m_Rotation{1.0f, 0.0f, 0.0f, 0.0f};
        glm::mat4 m_ViewMatrix = glm::mat4(1.0);
        glm::mat4 m_ProjectionMatrix = glm::mat4(1.0);
        glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0);

        private:
        virtual void UpdateCamera() = 0;
    };

}  // namespace Svarn
