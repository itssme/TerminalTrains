cmake_minimum_required(VERSION 3.9)

project(terminaltrains)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm -lncurses  -ltinfo -pthread")

include_directories(include)

# SPDLOG TODO set over env var
include_directories("/home/joel/Documents/school/nvs_5te_klasse/inclue_libs/spdlog/include")

# TODO: set env variables
#include_directories($ENV{SPDLOG_INCLUDE_PATH})

file(GLOB sources include/*.h src/*.cpp)
add_executable(terminaltrains ${sources})