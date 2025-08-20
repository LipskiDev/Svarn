#include <svpch.h>

#include <Platform/OpenGL/OpenGLRendererAPI.h>
#include <glad/gl.h>

namespace Svarn {

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) { glClearColor(color.r, color.g, color.b, color.a); };

    void OpenGLRendererAPI::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); };

    void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    };

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
