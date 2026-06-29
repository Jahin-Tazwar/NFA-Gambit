cmake_minimum_required(VERSION 3.16)

project(chess-game VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------
# Source files
# -------------------------

set(ENGINE_SOURCES
    src/engine/Piece.cpp
    src/engine/Board.cpp
    src/engine/Move.cpp
)

set(GUI_SOURCES
    src/gui/Window.cpp
    src/gui/Assets.cpp
)

set(MAIN_SOURCE
    src/main.cpp
)

# -------------------------
# Executable
# -------------------------

add_executable(chess-game
    ${MAIN_SOURCE}
    ${ENGINE_SOURCES}
    ${GUI_SOURCES}
)

# -------------------------
# Include directories
# -------------------------

target_include_directories(chess-game PRIVATE
    src
    src/engine
    src/gui
    C:/raylib/w64devkit/include
)

# -------------------------
# Raylib (GUI library)
# -------------------------

target_link_directories(chess-game PRIVATE C:/raylib/w64devkit/lib)

# -------------------------
# Optional: Windows-specific system libs 
# -------------------------
if (WIN32)
    target_link_libraries(chess-game PRIVATE raylib winmm gdi32 opengl32)
endif()

# -------------------------
# Compiler warnings 
# -------------------------

if (MSVC)
    target_compile_options(chess-game PRIVATE /W4)
else()
    target_compile_options(chess-game PRIVATE -Wall -Wextra -Wpedantic)
endif()