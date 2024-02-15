cmake_minimum_required(VERSION 3.14)
project(PodsSerdes
    VERSION 0.1
    DESCRIPTION "The tool to help serialize and deserialize POD types with the magic of libclang"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake/
)
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
    GIT_TAG v3.0
)

FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)

FetchContent_Declare(
    loguru
    GIT_REPOSITORY https://github.com/emilk/loguru.git
    GIT_TAG master
)

FetchContent_MakeAvailable(argparse json loguru)
find_package(libclang REQUIRED)

configure_file(PodsSerdesConfig.h.in PodsSerdesConfig.h)
add_executable(PodsSerdes src/main.cpp)

target_link_libraries(PodsSerdes PRIVATE
    argparse
    nlohmann_json::nlohmann_json
    loguru::loguru
    ${LIBCLANG_LIBRARY}
    ${CMAKE_DL_LIBS}
    stdc++fs
)

target_include_directories(PodsSerdes PRIVATE
    ${PROJECT_BINARY_DIR}
    ${LIBCLANG_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
)
