cmake_minimum_required(VERSION 3.14)
project(PodsSerdesExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
    PodsSerdes
    URL http://localhost:8000/PodsSerdes
    DOWNLOAD_NO_EXTRACT ON
)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(PodsSerdes json)
set(PODS_SERDES_BINARY ${podsserdes_SOURCE_DIR}/PodsSerdes)
set(PODS_SERDES_GENERATED_SOURCE_DIR ${PROJECT_BINARY_DIR}/podsSerdesGeneratedSource/include/)
set(PODS_SERDES_SOURCE_FILE ${PROJECT_SOURCE_DIR}/api-femas/ustptraderapi/USTPFtdcUserApiStruct.h)
set(PODS_SERDES_DEST_FILE ${PODS_SERDES_GENERATED_SOURCE_DIR}/pods_serdes_gen/messages.hpp)

add_custom_command(
    OUTPUT ${PODS_SERDES_GENERATED_SOURCE_DIR}/pods_serdes_gen/messages.hpp
    COMMAND chmod +x ${PODS_SERDES_BINARY}
    COMMAND mkdir -p ${PODS_SERDES_GENERATED_SOURCE_DIR}/pods_serdes_gen/
    COMMAND ${PODS_SERDES_BINARY} -f ${PODS_SERDES_SOURCE_FILE} -c ${PODS_SERDES_DEST_FILE}
    DEPENDS ${PODS_SERDES_SOURCE_FILE}
    COMMENT "Generating PodsSerdes files")

add_executable(femasApiVisitor femasApiVisitor.cpp)
add_custom_target(podsSerdesGeneration
    DEPENDS ${PODS_SERDES_DEST_FILE}
    COMMENT "Generated PodsSerdes files")

add_dependencies(femasApiVisitor
    podsSerdesGeneration
    nlohmann_json::nlohmann_json
)
target_include_directories(femasApiVisitor PRIVATE
    ${PODS_SERDES_GENERATED_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/api-femas/ustptraderapi
)

