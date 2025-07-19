include(FetchContent)
set(FETCHCONTENT_QUIET off)
set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/vendor CACHE PATH "Missing description." FORCE)

message(STATUS "Downloading third party libraries using CMake's FetchContent and CPM")


FetchContent_Declare(
	glad
	GIT_REPOSITORY	https://github.com/Dav1dde/glad.git
	GIT_TAG			v2.0.8
	GIT_SHALLOW		TRUE
	SOURCE_SUBDIR	cmake
)

FetchContent_Declare(
	glfw
	GIT_REPOSITORY	https://github.com/glfw/glfw.git
	GIT_TAG			3.4
	GIT_SHALLOW		TRUE
)

FetchContent_Declare(
	spdlog
	GIT_REPOSITORY	https://github.com/gabime/spdlog.git
	GIT_TAG			v1.15.1
	GIT_SHALLOW		TRUE
)





set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES Off CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS Off CACHE BOOL "" FORCE)
set(GLFW_INSTALL Off CACHE BOOL "" FORCE)
set(SPDLOG_USE_STD_FORMAT OFF CACHE BOOL "" FORCE)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

FetchContent_MakeAvailable(glad glfw spdlog)

glad_add_library(glad STATIC API gl:core=4.6 LOCATION ${PROJECT_SOURCE_DIR}/vendor/glad-build/${TARGET})




set_target_properties(glad PROPERTIES FOLDER "Utilities/glad")
set_target_properties(glfw PROPERTIES FOLDER "Utilities/GLFW3")
set_target_properties(spdlog PROPERTIES FOLDER "Utilities")
#======================================
message(STATUS "Fetching thirdparty libraries done")