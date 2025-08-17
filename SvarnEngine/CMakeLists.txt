include(CMakePrintHelpers)
include(CheckIPOSupported)

project(SvarnEngine
        VERSION 0.0.1
        LANGUAGES CXX)



if (WIN32)
	message("Build for Windows")
    add_compile_definitions(SV_PLATFORM_WINDOWS)
elseif (APPLE)
	message("Build for Mac")
    add_compile_definitions(SV_PLATFORM_MAC)
elseif (UNIX)
	message("Build for Linux")
    add_compile_definitions(SV_PLATFORM_LINUX)
endif()



# Main targets built by this project
add_subdirectory(src)

# Externally provided content
add_subdirectory(vendor)

# Configure the SvarnEngine target using our common properties
svarn_set_output_directories(SvarnEngine)
svarn_enable_lto(SvarnEngine)
svarn_enable_pch(SvarnEngine src/svpch.h)
svarn_set_compiler_options(SvarnEngine)

# Additional engine-specific compile definitions
target_compile_definitions(SvarnEngine PRIVATE
						_CRT_SECURE_NO_WARNINGS
						GLFW_INCLUDE_NONE
						YAML_CPP_STATIC_DEFINE
)



target_compile_definitions(SvarnEngine PRIVATE SV_BUILD_DLL)


target_include_directories(SvarnEngine PRIVATE
	src
	vendor/glfw-src/include
    vendor/imgui-src
    vendor/imgui-src/backends
    vendor/imgui-src/examples
    vendor/glm-src
	vendor/spdlog-src/include
)

if(WIN32)
    target_link_libraries(
        SvarnEngine
        glad
        glfw
        imgui
        spdlog::spdlog
        opengl32          
    )
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(
        SvarnEngine
        glad
        glfw
        imgui
        spdlog::spdlog
        OpenGL::GL
    )
endif()
