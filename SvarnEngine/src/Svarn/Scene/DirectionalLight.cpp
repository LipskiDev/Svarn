#include <svpch.h>
#include <Svarn/Scene/DirectionalLight.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Svarn {
    DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 radiance) : m_LightDirection(direction), m_LightRadiance(radiance) {};

    glm::mat4 DirectionalLight::ComputeShadowFrustum(std::array<glm::vec3, 8> frustumCorners) {
        glm::vec3 L = glm::normalize(m_LightDirection);
        glm::vec3 up = fabs(glm::dot(L, glm::vec3(0, 1, 0))) > 0.99f ? glm::vec3(1, 0, 0) : glm::vec3(0, 1, 0);
        glm::vec3 z_l = -L;
        glm::vec3 x_l = glm::normalize(glm::cross(up, z_l));
        glm::vec3 y_l = glm::cross(z_l, x_l);

        glm::vec3 center(0.0f);
        for (auto& p : frustumCorners) center += p;

        center *= 1.0f / 8.0f;

        glm::mat4 M_view_light = glm::lookAt(center, center + L, y_l);

        glm::vec3 minLS(1e9f), maxLS(-1e9f);
        for (auto& p : frustumCorners) {
            glm::vec4 q = M_view_light * glm::vec4(p, 1.0f);
            minLS = glm::min(minLS, glm::vec3(q));
            maxLS = glm::max(maxLS, glm::vec3(q));
        }

        float casterMargin = 20.f;
        minLS.z -= casterMargin;

        float mapRes = 2048.f;
        float w = maxLS.x - minLS.x;
        float h = maxLS.y - minLS.y;
        float texelX = w / mapRes;
        float texelY = h / mapRes;
        float cx = 0.5f * (minLS.x + maxLS.x);
        float cy = 0.5f * (minLS.y + maxLS.y);
        cx = std::floor(cx / texelX) * texelX;
        cy = std::floor(cy / texelY) * texelY;
        minLS.x = cx - 0.5 * w;
        maxLS.x = cx + 0.5f * w;
        minLS.y = cy - 0.5f * h;
        maxLS.y = cy + 0.5f * h;

        float left = minLS.x, right = maxLS.x;
        float bottom = minLS.y, top = maxLS.y;

        float nearPlane = -maxLS.z;
        float farPlane = -minLS.z;

        glm::mat4 M_proj_light = glm::ortho(left, right, bottom, top, nearPlane, farPlane);

        return M_proj_light * M_view_light;
    };
}  // namespace Svarn
