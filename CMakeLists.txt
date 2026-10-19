
cmake_minimum_required(VERSION 3.10)
project(BuddyCodingIntegrated)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    backend
    logic_engine
    guidence_engine
)

file(GLOB BACKEND_SOURCES "backend/*.cpp")
file(GLOB LOGIC_SOURCES "logic_engine/*.cpp")
file(GLOB GUIDANCE_SOURCES "guidence_engine/*.cpp")

add_executable(buddy_integrated_terminal
    ${BACKEND_SOURCES}
    ${LOGIC_SOURCES}
    ${GUIDANCE_SOURCES}
)

# Exclude any unrelated source files if necessary.
