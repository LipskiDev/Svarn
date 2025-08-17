
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Svarn/Scene/Camera.h"
namespace Svarn {
    class PerspectiveCamera : public Camera {
        public:
        PerspectiveCamera(float fov, float aspectRatio, float nearClipPlane, float farClipPlane);

        private:
        float m_FOV;
        float m_AspectRatio;
        float m_NearClipPlane;
        float m_FarClipPlane;
        glm::mat4 m_ViewMatrix = glm::mat4(1.0);
        glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0);
        glm::vec3 m_Position = glm::vec3(0.0);
        glm::quat m_Rotation{1.0f, 0.0f, 0.0f, 0.0f};
    };
}  // namespace Svarn
