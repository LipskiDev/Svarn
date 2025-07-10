set(SOURCES 
	"Svarn/Application.cpp"
	"Svarn/Application.h"
	"SvarnEnginePCH.cpp" 
	"SvarnEnginePCH.h")



add_library(SvarnEngine SHARED
	${SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
	FILES ${SOURCES}
)