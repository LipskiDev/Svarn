include(FetchContent)
set(FETCHCONTENT_QUIET off)
set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/vendor CACHE PATH "Missing description." FORCE)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "Disable update of FetchContent" FORCE)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)   # static into .so needs PIC
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(PNG_SHARED ON  CACHE BOOL "" FORCE)
set(PNG_STATIC OFF CACHE BOOL "" FORCE)
set(PNG_TESTS  OFF CACHE BOOL "" FORCE)
set(PNG_TOOLS  OFF CACHE BOOL "" FORCE)



message(STATUS "Downloading third party libraries using CMake's FetchContent and CPM")

FetchContent_Declare(
	assimp
	GIT_REPOSITORY	https://github.com/assimp/assimp.git
	GIT_TAG			master
	GIT_SHALLOW		TRUE
)

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
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG master
	GIT_SHALLOW		TRUE
)

FetchContent_Declare(
    libpng
    GIT_REPOSITORY https://github.com/glennrp/libpng.git
    GIT_TAG v1.6.46
	GIT_SHALLOW		TRUE
)

FetchContent_Declare(
	spdlog
	GIT_REPOSITORY	https://github.com/gabime/spdlog.git
	GIT_TAG			v1.15.1
	GIT_SHALLOW		TRUE
)

FetchContent_Declare(
    zlib
    GIT_REPOSITORY https://github.com/madler/zlib.git
    GIT_TAG v1.3.1
	GIT_SHALLOW		TRUE
)

FetchContent_MakeAvailable(zlib)

if(TARGET png_shared)
  # Link the target, not file paths; portable across platforms/configs
  # If you set BUILD_SHARED_LIBS ON for zlib above, use 'zlib' instead.
  target_link_libraries(png_shared PRIVATE zlibstatic)
  # Make sure headers are visible when building libpng
  target_include_directories(png_shared PRIVATE
    ${zlib_SOURCE_DIR} ${zlib_BINARY_DIR}
  )
  add_dependencies(png_shared zlib)  # ensure build order
endif()

FetchContent_MakeAvailable(libpng)
set(PNG_LIBRARY png CACHE INTERNAL "")
set(PNG_PNG_INCLUDE_DIR ${libpng_SOURCE_DIR} CACHE INTERNAL "")
set(PNG_LIBRARY_DIST ${libpng_BINARY_DIR}/Dist/libpng16.lib)
set(PNG_LIBRARY_RELEASE ${libpng_BINARY_DIR}/Release/libpng16.lib)
set(PNG_LIBRARY_DEBUG ${libpng_BINARY_DIR}/Debug/libpng16d.lib)
find_package(PNG REQUIRED)
if(TARGET PNG::PNG)
  set_target_properties(PNG::PNG PROPERTIES
    IMPORTED_LOCATION_DEBUG ${PNG_LIBRARY_DEBUG}
    IMPORTED_LOCATION_RELEASE ${PNG_LIBRARY_RELEASE}
    IMPORTED_LOCATION_DIST ${PNG_LIBRARY_DIST}
  )
endif()

# --- Fix: Force png_shared to use correct zlib for all configs ---

include_directories(${libpng_SOURCE_DIR} ${libpng_BINARY_DIR})
add_dependencies(png_shared zlib)


set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_ZLIB OFF CACHE INTERNAL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES Off CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS Off CACHE BOOL "" FORCE)
set(GLFW_INSTALL Off CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_USE_STD_FORMAT OFF CACHE BOOL "" FORCE)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

FetchContent_MakeAvailable(assimp glad glfw glm spdlog)

glad_add_library(glad STATIC API gl:core=4.6 LOCATION ${PROJECT_SOURCE_DIR}/vendor/glad-build/${TARGET})

FetchContent_Declare(stb_image
	GIT_REPOSITORY https://github.com/nothings/stb.git
	GIT_TAG master
	CONFIGURE_COMMAND ""
	BUILD_COMMAND "")
FetchContent_MakeAvailable(stb_image)
add_library(stb_image INTERFACE)
file(COPY ${stb_image_SOURCE_DIR}/stb_image.h DESTINATION "${PROJECT_SOURCE_DIR}/vendor/stb_image-build/include/stb_image")
set(STBIMAGE_INSTALL_DIR "${PROJECT_SOURCE_DIR}/vendor/stb_image-build")

CPMAddPackage(NAME imgui
GITHUB_REPOSITORY ocornut/imgui
	GIT_TAG docking
	DOWNLOAD_ONLY YES)
if(imgui_ADDED)
	FILE(GLOB imgui_sources ${imgui_SOURCE_DIR}/*.cpp)
	file(GLOB IMGUI_HEADERS ${IMGUI_PATH}/*.h)
	FILE(GLOB imgui_sources_cpp ${imgui_SOURCE_DIR}/misc/cpp/*.cpp)

	set(IMGUI_DIR ${imgui_SOURCE_DIR})
	set(IMGUI_INCLUDE_DIR ${imgui_SOURCE_DIR})
	set(IMGUI_SOURCES ${imgui_sources} ${imgui_sources_cpp})

  add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS} "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp" "${IMGUI_DIR}/backends/imgui_impl_opengl3.h" "${IMGUI_DIR}/backends/imgui_impl_opengl3_loader.h" "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp" "${IMGUI_DIR}/backends/imgui_impl_glfw.h")
	target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD=1)
	target_include_directories(imgui PUBLIC ${IMGUI_DIR} "glfw-src/include" "glad-build/include")
	set_target_properties(imgui PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/vendor/imgui-build"
		LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/vendor/imgui-build"
		RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/vendor/imgui-build")
endif()

set_target_properties(assimp PROPERTIES FOLDER "Utilities/assimp")
set_target_properties(glad PROPERTIES FOLDER "Utilities/glad")
set_target_properties(glfw PROPERTIES FOLDER "Utilities/GLFW3")
set_target_properties(glm PROPERTIES FOLDER "Utilities")
set_target_properties(imgui PROPERTIES FOLDER "Utilities/imgui")
set_target_properties(spdlog PROPERTIES FOLDER "Utilities")
#======================================
message(STATUS "Fetching thirdparty libraries done")
