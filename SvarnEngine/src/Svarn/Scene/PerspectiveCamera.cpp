#include <svpch.h>
#include <Svarn/Scene/PerspectiveCamera.h>
#include "Svarn/Core.h"
#include "Svarn/Events/Event.h"
#include "Svarn/Events/MouseEvent.h"
#include "Svarn/KeyCodes.h"
#include "glm/ext/matrix_clip_space.hpp"
#include <Svarn/Input.h>
#include <Svarn/Log.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Svarn {
    PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClipPlane, float farClipPlane)
        : m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClipPlane(nearClipPlane), m_FarClipPlane(farClipPlane) {
        m_Position = glm::vec3(0.0, 0.0, 3.0);
        m_Rotation = glm::quat(1.0, 0.0, 0.0, 0.0);
        m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClipPlane, m_FarClipPlane);
        const glm::mat4 I(1.0f);
        const glm::mat4 T = glm::translate(I, m_Position);
        const glm::mat4 R = glm::mat4_cast(glm::normalize(m_Rotation));
        const glm::mat4 worldFromCamera = T * R;

        m_ViewMatrix = glm::inverse(worldFromCamera);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    };

    void PerspectiveCamera::OnUpdate(Timestep ts) {
        glm::vec3 forward = glm::rotate(m_Rotation, glm::vec3(0.0f, 0.0f, -1.0f));
        glm::vec3 up = glm::rotate(m_Rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 right = glm::rotate(m_Rotation, glm::vec3(1.0f, 0.0f, 0.0f));

        float speed = 100;
        if (Input::IsKeyPressed(SV_KEY_W)) {
            m_Position += ts.GetSeconds() * forward * speed;
        }

        if (Input::IsKeyPressed(SV_KEY_A)) {
            m_Position += ts.GetSeconds() * -right * speed;
        }

        if (Input::IsKeyPressed(SV_KEY_S)) {
            m_Position += ts.GetSeconds() * -forward * speed;
        }

        if (Input::IsKeyPressed(SV_KEY_D)) {
            m_Position += ts.GetSeconds() * right * speed;
        }

        if (Input::IsKeyPressed(SV_KEY_SPACE)) {
            m_Position += ts.GetSeconds() * up * speed;
        }

        if (Input::IsKeyPressed(SV_KEY_LEFT_SHIFT)) {
            m_Position += ts.GetSeconds() * -up * speed;
        }

        UpdateCamera();
    };

    bool PerspectiveCamera::RotateCamera(MouseMovedEvent& event) {
        auto [x, y] = Input::GetMousePosition();

        if (Input::IsMouseButtonPressed(0)) {
            if (m_FirstMouse) {
                m_LastX = x;
                m_LastY = y;
                m_FirstMouse = false;
            }

            float xoffset = x - m_LastX;
            float yoffset = y - m_LastY;

            m_LastX = x;
            m_LastY = y;

            m_Yaw -= xoffset;
            m_Pitch -= yoffset;

            if (m_Pitch > 89.0f) m_Pitch = 89.0f;
            if (m_Pitch < -89.0f) m_Pitch = -89.0f;

            glm::quat qPitch = glm::angleAxis(glm::radians(m_Pitch), glm::vec3(1, 0, 0));
            glm::quat qYaw = glm::angleAxis(glm::radians(m_Yaw), glm::vec3(0, 1, 0));

            m_Rotation = glm::normalize(qYaw * qPitch);

            UpdateCamera();
        } else {
            m_FirstMouse = true;
        }

        return false;
    }

    void PerspectiveCamera::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseMovedEvent>(SV_BIND_EVENT_FN(PerspectiveCamera::RotateCamera));
    }

    void PerspectiveCamera::UpdateCamera() {
        glm::mat4 V = glm::mat4_cast(glm::conjugate(m_Rotation))       // R^T
                      * glm::translate(glm::mat4(1.0f), -m_Position);  // T^{-1}
        m_ViewMatrix = V;
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}  // namespace Svarn
