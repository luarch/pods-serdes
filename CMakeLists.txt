cmake_minimum_required(VERSION 3.14)
project(PodsSerdes)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

add_executable(PodsSerdes main.cpp)
target_link_libraries(PodsSerdes PRIVATE argparse)
