cmake_minimum_required(VERSION 3.10)
project(LogicEngine)
set(CMAKE_CXX_STANDARD 17)

# Download json.hpp if missing
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/json.hpp)
    file(DOWNLOAD https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp ${CMAKE_CURRENT_SOURCE_DIR}/json.hpp)
endif()

add_library(logic_engine LogicEngine.cpp RuleRepository.cpp SharedTypes.cpp)
add_executable(logic_test tests/logic_test.cpp)
target_link_libraries(logic_test logic_engine)
