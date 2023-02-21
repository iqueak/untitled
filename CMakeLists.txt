#https://github.com/raysan5/raylib/blob/master/projects/CMake/CMakeLists.txt
cmake_minimum_required(VERSION 3.16) # FetchContent is available in 3.11+
project(untitled)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
# Raylib
set(RAYLIB_VERSION 4.2.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

#anl
include_directories(vendor/anl vendor/anl/include)

# Our Project

add_executable(${PROJECT_NAME} src/main.cpp src/World/ChunkManager.h src/World/ChunkManager.cpp src/World/Block.cpp src/World/Block.h src/World/TextureManager.cpp src/World/TextureManager.h src/Utils/Logger.h src/PlayerController.cpp src/PlayerController.h src/KeyboardController.h src/CameraController.cpp src/World/CameraController.h src/Utils/Logger.cpp src/Window.cpp src/Window.h src/World/WorldGenerator.h src/World/WorldGenerator.cpp src/World/WorldManager.cpp src/World/WorldManager.h)

target_link_libraries(${PROJECT_NAME} raylib)