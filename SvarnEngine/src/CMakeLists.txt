set(SOURCES
		"Svarn.h"
		"svpch.cpp"
		"svpch.h"

		"Svarn/EntryPoint.h"
		"Svarn/Core.h"
		"Svarn/Application.cpp"
		"Svarn/Application.h"
		
		
		"Svarn/Log.cpp"
		"Svarn/Log.h"
		"Svarn/Window.h"

		"Svarn/Events/ApplicationEvent.h"
		"Svarn/Events/Event.h"
		"Svarn/Events/KeyEvent.h"
		"Svarn/Events/MouseEvent.h"

		"Platform/Windows/WindowsWindow.cpp"
		"Platform/Windows/WindowsWindow.h"
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
