include(CMakePrintHelpers)
include(CheckIPOSupported)

project(SvarnEngine
        VERSION 0.0.1
        LANGUAGES CXX)


# Main targets built by this project
add_subdirectory(src)



# Configure the SvarnEngine target using our common properties
svarn_set_output_directories(SvarnEngine)
svarn_enable_lto(SvarnEngine)
svarn_enable_pch(SvarnEngine src/SvarnEnginePCH.h)
svarn_set_compiler_options(SvarnEngine)

# Additional engine-specific compile definitions
target_compile_definitions(SvarnEngine PRIVATE
						_CRT_SECURE_NO_WARNINGS
						GLFW_INCLUDE_NONE
						YAML_CPP_STATIC_DEFINE
)


target_compile_definitions(SvarnEngine PRIVATE SV_PLATFORM_WINDOWS)
target_compile_definitions(SvarnEngine PRIVATE SV_BUILD_DLL)

svarn_set_common_definitions(SvarnEngine)

target_include_directories(SvarnEngine PRIVATE
	src
	# vendor/assimp-src/include
	# vendor/box2d-src/include/box2d
	# vendor/box2d-src/src
	# vendor/entt-src/single_include/entt
	# vendor/filewatch-src
	# vendor/glad-build/include
	# vendor/glfw-src/include
	# vendor/glm-src
	# vendor/miniaudio-src
	# vendor/lua-src
	# vendor/imgui-src
	# vendor/imgui-src/backends
	# vendor/imgui-src/examples
	# vendor/imguizmo-src
	# vendor/spdlog-src/include
	# vendor/tracy-src/public
	# vendor/sol2-src/include
	# vendor/stb_image-build/include
	# mono/include
	# ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(SvarnEngine
	# assimp
	# box2d
	# glad
	# glfw
	# imgui
	# imguizmo
	# msdf-atlas-gen
	# opengl32.lib
	# sol2
	# stb_image
	# TracyClient
	# yaml-cpp
	# Vulkan::Vulkan
	# Vulkan::glslang
	# Vulkan::SPIRV-Tools
	# debug $ENV{VULKAN_SDK}/Lib/shaderc_sharedd.lib
	# debug $ENV{VULKAN_SDK}/Lib/spirv-cross-cored.lib
	# debug $ENV{VULKAN_SDK}/Lib/spirv-cross-glsld.lib
	# optimized $ENV{VULKAN_SDK}/Lib/shaderc_shared.lib
	# optimized $ENV{VULKAN_SDK}/Lib/spirv-cross-core.lib
	# optimized $ENV{VULKAN_SDK}/Lib/spirv-cross-glsl.lib
	# debug ${CMAKE_CURRENT_SOURCE_DIR}/mono/lib/Debug/libmono-static-sgen.lib
	# optimized ${CMAKE_CURRENT_SOURCE_DIR}/mono/lib/Release/libmono-static-sgen.lib
	# BCrypt
	# Version
	# ws2_32
	# WinMM
	# png_shared
    # $<$<CONFIG:Debug>:${ZLIB_LIBRARY_DEBUG}>
    # $<$<CONFIG:Release>:${ZLIB_LIBRARY_RELEASE}>
    # $<$<CONFIG:Dist>:${ZLIB_LIBRARY_DIST}>
)