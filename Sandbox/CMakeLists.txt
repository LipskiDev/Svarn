project(Sandbox
		LANGUAGES CXX)

set(Sandbox_VERSION_MAJOR 0)
set(Sandbox_VERSION_MINOR 0)
set(Sandbox_VERSION_PATCH 1)
set(Sandbox_VERSION
${Sandbox_VERSION_MAJOR}.${Sandbox_VERSION_MINOR}.${Sandbox_VERSION_PATCH}
)

# Main targets built by this project
add_subdirectory(src)


add_dependencies(Sandbox
				SvarnEngine
)



# Set up common include directories
svarn_set_common_include_directories(Sandbox)

if (WIN32)
    target_compile_definitions(Sandbox PRIVATE SV_PLATFORM_WINDOWS)

elseif (APPLE)
    target_compile_definitions(Sandbox PRIVATE SV_PLATFORM_MAC)

elseif (UNIX)
    target_compile_definitions(Sandbox PRIVATE SV_PLATFORM_LINUX)

endif()




target_link_libraries(Sandbox
					SvarnEngine
)