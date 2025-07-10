project("Sandbox"
		LANGUAGES CXX)



set(Sandbox_VERSION_MAJOR 0)
set(Sandbox_VERSION_MINOR 0)
set(Sandbox_VERSION_PATCH 1)
set(Sandbox_VERSION
${Sandbox_VERSION_MAJOR}.${Sandbox_VERSION_MINOR}.${Sandbox_VERSION_PATCH}
)

# Main targets built by this project
add_subdirectory(src)

SVARN_CONFIGURE_APP(Sandbox NO_PCH)

add_dependencies(Sandbox
				SvarnEngine
)


SVARN_SET_COMMON_INCLUDE_DIRECTORIES(Sandbox)

target_compile_definitions(Sandbox PRIVATE SV_PLATFORM_WINDOWS)

target_link_libraries(Sandbox
					SvarnEngine
)

