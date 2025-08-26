
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include "Svarn/Events/Event.h"
#include "Svarn/Events/MouseEvent.h"
#include "Svarn/Scene/Camera.h"
#include "Svarn/Timestep.h"

namespace Svarn {
    class PerspectiveCamera : public Camera {
        public:
        PerspectiveCamera(float fov, float aspectRatio, float nearClipPlane, float farClipPlane);

        void OnEvent(Event& e);
        void OnUpdate(Timestep ts);

        private:
        float m_FOV;
        float m_AspectRatio;
        float m_NearClipPlane;
        float m_FarClipPlane;

        virtual void UpdateCamera() override;
        bool RotateCamera(MouseMovedEvent& event);

        bool m_FirstMouse = true;
        float m_LastX, m_LastY;
        float m_Yaw = 0.0, m_Pitch = 0.0;
    };
}  // namespace Svarn
