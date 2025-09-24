#pragma once

namespace Svarn {
    class DirectionalLight {
        public:
        DirectionalLight(glm::vec3 direction, glm::vec3 radiance);

        glm::vec3 m_LightDirection;
        glm::vec3 m_LightRadiance;
        glm::mat4 CalculateLightTTransform();

        private:
        glm::mat4 lightProj;
    };
}  // namespace Svarn
