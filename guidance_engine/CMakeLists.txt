cmake_minimum_required(VERSION 3.10)
project(GuidanceEngine)
set(CMAKE_CXX_STANDARD 17)

# Download json.hpp if missing
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/json.hpp)
    file(DOWNLOAD https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp ${CMAKE_CURRENT_SOURCE_DIR}/json.hpp)
endif()

add_library(guidance_engine Guidance.cpp GuidanceRepository.cpp)
add_executable(guidance_test tests/guidance_test.cpp)
target_link_libraries(guidance_test guidance_engine)
