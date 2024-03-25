cmake_minimum_required(VERSION 3.26)
project(Flow_Field_Pathfinding_v2_0_0)

set(CMAKE_CXX_STANDARD 14)

include_directories("Part1")
include_directories("Part2")
set(P_ASTAR ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/build)
set(SDL2_PATH "D:/SDL2-2.30.0/x86_64-w64-mingw32")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(ASTAR2
        "Part2/Game.cpp"
        "Part2/Game.h"
        "Part2/Level.cpp"
        "Part2/Level.h"
        "Part2/main.cpp"
        "Part2/TextureLoader.cpp"
        "Part2/TextureLoader.h"
        "Part2/Unit.cpp"
        "Part2/Unit.h"
        "Part2/Vector2D.cpp"
        "Part2/Vector2D.h")

add_executable(ASTAR1
        "Part1/Game.cpp"
        "Part1/Game.h"
        "Part1/Level.cpp"
        "Part1/Level.h"
        "Part1/main.cpp"
        "Part1/TextureLoader.cpp"
        "Part1/TextureLoader.h"
        "Part1/Unit.cpp"
        "Part1/Unit.h"
        "Part1/Vector2D.cpp"
        "Part1/Vector2D.h")

set_target_properties(ASTAR1 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${P_ASTAR}
)

target_link_libraries(ASTAR1 ${SDL2_LIBRARY})

set_target_properties(ASTAR2 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${P_ASTAR}
)

target_link_libraries(ASTAR2 ${SDL2_LIBRARY})