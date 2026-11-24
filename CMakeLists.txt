cmake_minimum_required(VERSION 3.16)
project(NexusAI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(nexus-ai main.cpp)

if(MSVC)
    target_compile_options(nexus-ai PRIVATE /W4)
else()
    target_compile_options(nexus-ai PRIVATE -Wall -Wextra -pedantic)
endif()
