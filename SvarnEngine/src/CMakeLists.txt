set(SOURCES
    "Svarn.h"
    "svpch.cpp"
    "svpch.h"

    "Svarn/EntryPoint.h"
    "Svarn/Core.h"
    "Svarn/Application.cpp"
    "Svarn/Application.h"
    "Svarn/Input.h"
    "Svarn/Timestep.h"
        
    "Svarn/Layer.cpp"
    "Svarn/Layer.h"
    "Svarn/LayerStack.cpp"
    "Svarn/LayerStack.h"

    "Svarn/ImGui/ImGuiLayer.cpp"
    "Svarn/ImGui/ImGuiLayer.h"

    "Svarn/Scene/Camera.h"
    "Svarn/Scene/DirectionalLight.cpp"
    "Svarn/Scene/DirectionalLight.h"
    "Svarn/Scene/PerspectiveCamera.cpp"
    "Svarn/Scene/PerspectiveCamera.h"

    "Svarn/Log.cpp"
    "Svarn/Log.h"
    "Svarn/Window.h"

    "Svarn/Math/Transform.h"

    "Svarn/KeyCodes.h"

    "Svarn/Events/ApplicationEvent.h"
    "Svarn/Events/Event.h"
    "Svarn/Events/KeyEvent.h"
    "Svarn/Events/MouseEvent.h"


    "Svarn/Renderer/Buffer.cpp"
    "Svarn/Renderer/Buffer.h"
    "Svarn/Renderer/Framebuffer.cpp"
    "Svarn/Renderer/Framebuffer.h"
    "Svarn/Renderer/Mesh.cpp"
    "Svarn/Renderer/Mesh.h"
    "Svarn/Renderer/Model.cpp"
    "Svarn/Renderer/Model.h"
    "Svarn/Renderer/Primitives.cpp"
    "Svarn/Renderer/Primitives.h"
    "Svarn/Renderer/RenderCommand.cpp"
    "Svarn/Renderer/RenderCommand.h"
    "Svarn/Renderer/Renderer.cpp"
    "Svarn/Renderer/Renderer.h"
    "Svarn/Renderer/RendererAPI.cpp"
    "Svarn/Renderer/RendererAPI.h"
    "Svarn/Renderer/RenderingContext.h"
    "Svarn/Renderer/Shader.cpp"
    "Svarn/Renderer/Shader.h"
    "Svarn/Renderer/Texture.cpp"
    "Svarn/Renderer/Texture.h"
    "Svarn/Renderer/VertexArray.cpp"
    "Svarn/Renderer/VertexArray.h"

    "Platform/OpenGL/OpenGLBuffer.cpp"
    "Platform/OpenGL/OpenGLBuffer.h"
    "Platform/OpenGL/OpenGLFramebuffer.cpp"
    "Platform/OpenGL/OpenGLFramebuffer.h"
    "Platform/OpenGL/OpenGLRendererAPI.cpp"
    "Platform/OpenGL/OpenGLRendererAPI.h"
    "Platform/OpenGL/OpenGLRenderingContext.cpp"  
    "Platform/OpenGL/OpenGLRenderingContext.h"  
    "Platform/OpenGL/OpenGLShader.h"
    "Platform/OpenGL/OpenGLShader.cpp"
    "Platform/OpenGL/OpenGLTexture.cpp"
    "Platform/OpenGL/OpenGLTexture.h"
    "Platform/OpenGL/OpenGLVertexArray.cpp"
    "Platform/OpenGL/OpenGLVertexArray.h"

    "Platform/Linux/LinuxInput.cpp"
    "Platform/Linux/LinuxInput.h"
    "Platform/Linux/LinuxWindow.cpp"
    "Platform/Linux/LinuxWindow.h"
)

add_library(SvarnEngine SHARED
		${SOURCES}
)

message("CMAKE_VERSION: ${CMAKE_VERSION}")
message("HALLO JACOB ${PROJECT_SOURCE_DIR}")




add_custom_command(TARGET SvarnEngine POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E echo "Post-build step: Copy into ${PROJECT_SOURCE_DIR}/../Sandbox/src/Debug"

	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SvarnEngine> "${PROJECT_SOURCE_DIR}/../build/Sandbox/src/Debug"
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SvarnEngine> "${PROJECT_SOURCE_DIR}/../Sandbox/src/Debug"
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SvarnEngine> "${PROJECT_SOURCE_DIR}/../build/Sandbox/src/Debug"
)


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
			FILES ${SOURCES}
)
