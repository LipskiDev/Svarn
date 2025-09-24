#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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

        public:
        std::array<glm::vec3, 8> GetFrustumCorners() {
            const glm::mat4 invVP = glm::inverse(m_ProjectionMatrix * m_ViewMatrix);

            const float ndc[2] = {-1.0f, 1.0f};

            std::array<glm::vec3, 8> c;
            int idx = 0;
            for (int rx = 0; rx < 2; ++rx)
                for (int ty = 0; ty < 2; ++ty)
                    for (int zf = 0; zf < 2; ++zf) {
                        glm::vec4 p = invVP * glm::vec4(ndc[rx], ndc[ty], ndc[zf], 1.0f);
                        p /= p.w;
                        c[idx++] = glm::vec3(p);
                    }
            return c;
        }

        glm::vec3 GetFrustumCenter() {
            const std::array<glm::vec3, 8> corners = GetFrustumCorners();

            glm::vec3 center(0.0f);
            for (const auto& corner : corners) {
                center += corner;
            }

            center /= static_cast<float>(corners.size());
            return center;
        }

        protected:
        glm::vec3 m_Position;
        glm::quat m_Rotation;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        private:
        virtual void UpdateCamera() = 0;
    };
}  // namespace Svarn
