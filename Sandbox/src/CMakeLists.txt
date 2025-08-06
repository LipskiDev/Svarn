set(Sandbox_SOURCES
		"SandboxApp.cpp"
)

add_executable(Sandbox
        ${Sandbox_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
			FILES ${Sandbox_SOURCES}
)