#include <svpch.h>
#include <Svarn/Scene/DirectionalLight.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Svarn {
    DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 radiance) : m_LightDirection(direction), m_LightRadiance(radiance) {
        lightProj = glm::ortho(-200.0f, 200.0f,  // left, right
                               -200.0f, 200.0f,  // bottom, top
                               0.1f, 200.0f      // near, far
        );
    };

    glm::mat4 DirectionalLight::CalculateLightTTransform() {
        return lightProj * glm::lookAt(-m_LightDirection * 10.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
}  // namespace Svarn
