
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Svarn/Events/Event.h"
#include "Svarn/Events/MouseEvent.h"
#include "Svarn/Scene/Camera.h"
#include "Svarn/Timestep.h"

namespace Svarn {
    class PerspectiveCamera : public Camera {
        public:
        PerspectiveCamera(float fov, float aspectRatio, float nearClipPlane, float farClipPlane);

        const glm::mat4& GetViewProjectionMatrix() { return m_ViewProjectionMatrix; }
        const glm::mat4& GetViewMatrix() { return m_ViewMatrix; }
        const glm::vec3& GetPosition() { return m_Position; }

        void OnEvent(Event& e);
        void OnUpdate(Timestep ts);

        private:
        float m_FOV;
        float m_AspectRatio;
        float m_NearClipPlane;
        float m_FarClipPlane;
        glm::vec3 m_Position = glm::vec3(0.0);
        glm::quat m_Rotation{1.0f, 0.0f, 0.0f, 0.0f};

        virtual void UpdateCamera() override;
        bool RotateCamera(MouseMovedEvent& event);

        bool m_FirstMouse = true;
        float m_LastX, m_LastY;
        float m_Yaw = 0.0, m_Pitch = 0.0;
    };
}  // namespace Svarn
