# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Define the project name
project(LanderProject)

find_package(Freetype REQUIRED)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Library Setup ---

# --- GLFW (Manual Import) ---
add_library(glfw_lib STATIC IMPORTED)
set_target_properties(glfw_lib PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include"
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/lib-vc2022/glfw3.lib"
)

# --- Assimp (Manual Import) ---
add_library(assimp_lib STATIC IMPORTED)
set_target_properties(assimp_lib PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/include"
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/lib/x64/assimp-vc143-mt.lib"
)

# --- GLAD ---
add_library(glad_lib ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/src/glad.c)
target_include_directories(glad_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include)

# --- Executable ---
add_executable(Lander
    src/main.cpp
    src/engine/graphics/Shader.cpp
    src/engine/graphics/Camera.cpp
    src/engine/core/InputManager.cpp
    src/engine/graphics/TextureLoader.cpp
    src/engine/graphics/Mesh.cpp
    src/engine/graphics/Model.cpp
    src/engine/graphics/Sphere.cpp
    src/engine/ui/TextRenderer.cpp
    src/engine/ui/UIManager.cpp
    src/game/CommandModule.cpp
)

# --- Linking and Includes ---
target_include_directories(Lander PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
    ${FREETYPE_INCLUDE_DIRS}
)

# Link all the necessary libraries
target_link_libraries(Lander PRIVATE glad_lib glfw_lib opengl32 assimp_lib Freetype::Freetype)

# --- Linker options for hiding console ---
if(MSVC)
    target_link_options(Lander PRIVATE "/SUBSYSTEM:WINDOWS" "/ENTRY:mainCRTStartup")
endif()

# --- Post-Build Asset Copying ---
add_custom_command(TARGET Lander POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:Lander>/assets"
    COMMENT "Copying assets to build directory"
)

add_custom_command(TARGET Lander POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/bin/x64/assimp-vc143-mt.dll"
    "$<TARGET_FILE_DIR:Lander>"
    COMMENT "Copying Assimp DLL to build directory"
)
