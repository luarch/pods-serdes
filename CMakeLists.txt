cmake_minimum_required(VERSION 3.14)
project(PodsSerdes
    VERSION 0.1
    DESCRIPTION "The tool to help serialize and deserialize Pod types with the magic of libclang")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
    GIT_TAG v3.0
)
FetchContent_MakeAvailable(argparse)

find_package(nlohmann_json)

configure_file(PodsSerdesConfig.h.in PodsSerdesConfig.h)
add_executable(PodsSerdes main.cpp)

target_link_libraries(PodsSerdes PRIVATE
    argparse
    nlohmann_json::nlohmann_json)

target_include_directories(PodsSerdes PRIVATE ${PROJECT_BINARY_DIR})
