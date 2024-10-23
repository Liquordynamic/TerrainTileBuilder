cmake_minimum_required(VERSION 3.25)
project(TerrainTileBuilder)

set(CMAKE_CXX_STANDARD 20)

# Variables
set(CPU_PARALLEL_COMPUTATION FALSE)

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(INCLUDE_OUTPUT_DIR "${CMAKE_BINARY_DIR}/include")
set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third_party")
set(LOCAL_THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third_party/local_lib")
set(EXTERNAL_THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third_party/external_lib")
set(LOCAL_THIRD_PARTY_OUTPUT_DIR "${CMAKE_BINARY_DIR}/third_party/local_lib")
set(EXTERNAL_THIRD_PARTY_OUTPUT_DIR "${CMAKE_BINARY_DIR}/third_party/external_lib")

# QuikGrid
set(QUIKGRID_DIR "${LOCAL_THIRD_PARTY_DIR}/QuikGrid")
set(QUIKGRID_INCLUDE_DIR "${QUIKGRID_DIR}/include")
set(QUIKGRID_OUTPUT_DIR "${LOCAL_THIRD_PARTY_OUTPUT_DIR}/QuikGrid")
file(GLOB QUIKGRID_SRC_FILES "${QUIKGRID_DIR}/*.cpp")
add_library(quikgrid ${QUIKGRID_SRC_FILES})
target_include_directories(quikgrid
    PUBLIC
        ${QUIKGRID_INCLUDE_DIR}
)
set_target_properties(quikgrid PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${QUIKGRID_OUTPUT_DIR}
)
list(APPEND LIBS quikgrid)

# STB
set(STB_DIR "${EXTERNAL_THIRD_PARTY_DIR}/stb")

# OpenMP
find_package(OpenMP REQUIRED)
if (OpenMP_FOUND)
    message(STATUS "Found OpenMP")
    set(CPU_PARALLEL_COMPUTATION TRUE)
    list(APPEND LIBS OpenMP::OpenMP_CXX)
endif ()

# Nlohmann_json
set(NLOHMANN_JSON_DIR "${EXTERNAL_THIRD_PARTY_DIR}/json")
add_subdirectory(${NLOHMANN_JSON_DIR})
list(APPEND LIBS nlohmann_json::nlohmann_json)

# Nanoflann
set(NANOFLANN_DIR "${EXTERNAL_THIRD_PARTY_DIR}/nanoflann")
add_subdirectory(${NANOFLANN_DIR})
list(APPEND LIBS nanoflann::nanoflann)

# Configure assets header file
configure_file("${INCLUDE_DIR}/Config.h.in"
        "${INCLUDE_OUTPUT_DIR}/Config.h"
)

# Get files
file(GLOB_RECURSE CPP_FILES "${SOURCE_DIR}/*.*")
add_executable(${PROJECT_NAME}
        ${CPP_FILES}
)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${STB_DIR}
        ${INCLUDE_DIR}
        ${INCLUDE_OUTPUT_DIR}
)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${LIBS}
)
