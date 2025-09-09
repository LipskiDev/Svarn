#pragma once

namespace Svarn {
    class DirectionalLight {
        public:
        DirectionalLight(glm::vec3 direction, glm::vec3 radiance);

        glm::mat4 ComputeShadowFrustum(std::array<glm::vec3, 8> frustumCorners);
        glm::vec3 m_LightDirection;
        glm::vec3 m_LightRadiance;
    };
}  // namespace Svarn
