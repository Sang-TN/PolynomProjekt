cmake_minimum_required(VERSION 3.16)
project(PolynomProjekt LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- raylib: erst lokal suchen, sonst automatisch von GitHub holen ---
find_package(raylib 5.5 QUIET)
if(NOT raylib_FOUND)
    include(FetchContent)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG        5.5
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(raylib)
endif()

# --- Alle .cpp-Dateien im Projektordner (neue Dateien werden automatisch erkannt) ---
file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/*.cpp")

add_executable(polynom ${SOURCES})
target_link_libraries(polynom PRIVATE raylib)

if(APPLE)
    target_link_libraries(polynom PRIVATE
        "-framework Cocoa" "-framework IOKit" "-framework OpenGL" "-framework CoreVideo")
endif()
