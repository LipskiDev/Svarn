#include <svpch.h>

#include <Platform/OpenGL/OpenGLRendererAPI.h>
#include <glad/gl.h>
#include "Svarn/Log.h"

namespace Svarn {

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) { glClearColor(color.r, color.g, color.b, color.a); };

    void OpenGLRendererAPI::Clear() {
        glClearDepth(1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    };

    void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    };

    void OpenGLRendererAPI::EnableDepthTest() { glEnable(GL_DEPTH_TEST); }

    void OpenGLRendererAPI::DisableDepthTest() { glDisable(GL_DEPTH_TEST); }

    RendererAPIInfo OpenGLRendererAPI::GetRendererAPIInfo() {
        RendererAPIInfo info;

        std::string vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        std::string renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        std::string version = reinterpret_cast<const char*>(glGetString(GL_VERSION));

        info.API = "OpenGL";
        info.Vendor = vendor;
        info.Renderer = renderer;
        info.Version = version;

        return info;
    }
}  // namespace Svarn
