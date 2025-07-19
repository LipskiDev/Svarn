include(CMakePrintHelpers)
include(CheckIPOSupported)

project(SvarnEngine
        VERSION 0.0.1
        LANGUAGES CXX)






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


if (WIN32)
    add_compile_definitions(SV_PLATFORM_WINDOWS)
elseif (APPLE)
    add_compile_definitions(SV_PLATFORM_MAC)
elseif (UNIX)
    add_compile_definitions(SV_PLATFORM_LINUX)
endif()

target_compile_definitions(SvarnEngine PRIVATE SV_BUILD_DLL)


target_include_directories(SvarnEngine PRIVATE
	src
	vendor/glfw-src/include
	vendor/spdlog-src/include
)

if(WIN32)
    target_link_libraries(
        SvarnEngine
        glad
        glfw
        spdlog::spdlog
        opengl32           # ohne .lib – das ist MSVC-Notation, nicht nötig
    )
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(
        SvarnEngine
        glad
        glfw
        spdlog::spdlog
        OpenGL::GL
    )
endif()