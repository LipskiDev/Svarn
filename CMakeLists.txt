cmake_minimum_required(VERSION 3.25)

message(STATUS "CMake version: ${CMAKE_VERSION}")

# Performance optimizations for CMake itself
set(CMAKE_FIND_PACKAGE_NO_PACKAGE_REGISTRY ON)
set(CMAKE_FIND_PACKAGE_NO_SYSTEM_PACKAGE_REGISTRY ON)

project(SvarnEngineBase
        VERSION 0.0.1
        LANGUAGES CXX C)

# Build optimization options
option(SVARN_ENABLE_LTO "Enable Link Time Optimization" ON)
option(SVARN_ENABLE_PCH "Enable Precompiled Headers" ON)

# Includes
include(cmake/SetupConfigurations.cmake)
include(cmake/CommonProperties.cmake)
include(cmake/CPM.cmake)

# Project wide setup - Configure module paths
list(APPEND CMAKE_MODULE_PATH 
    ${CMAKE_CURRENT_LIST_DIR}/cmake
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS YES)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Utilities/cmake")

# Build options
option(BUILD_TESTS "Build tests" OFF)

# Main targets built by this project
add_subdirectory(SvarnEngine)
add_subdirectory(Sandbox)

set_target_properties(Sandbox PROPERTIES FOLDER "Applications")


if(${CMAKE_GENERATOR} MATCHES "Visual Studio")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "Sandbox")
endif()