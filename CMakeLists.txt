cmake_minimum_required(VERSION 3.9)

project(terminaltrains)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm -lncurses  -ltinfo -pthread")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

include_directories(include)

find_package(spdlog REQUIRED)
find_package(Curses REQUIRED)

file(GLOB sources include/*.h src/*.cpp include/game/*.h src/game/*.cpp include/game/game_objects/*.h src/game/game_objects/*.cpp)
add_executable(terminaltrains ${sources})
target_link_libraries(terminaltrains PRIVATE spdlog::spdlog ${CURSES_LIBRARIES})