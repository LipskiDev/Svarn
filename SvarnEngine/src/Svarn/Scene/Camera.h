#pragma once

#include <glm/glm.hpp>

namespace Svarn {

    class Camera {
        public:
        Camera() = default;

        virtual ~Camera() = default;

        const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewProjection() const { return m_ViewProjectionMatrix; }

        protected:
        glm::mat4 m_ViewMatrix = glm::mat4(1.0);
        glm::mat4 m_ProjectionMatrix = glm::mat4(1.0);
        glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0);

        private:
        virtual void UpdateCamera() = 0;
    };

}  // namespace Svarn
