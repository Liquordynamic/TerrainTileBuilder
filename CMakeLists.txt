cmake_minimum_required(VERSION 3.27)
project(TerrainTileBuilder)

set(CMAKE_CXX_STANDARD 20)

# Variable
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third_party")
set(LOCAL_THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third_party/local_lib")
set(EXTERNAL_THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third_party/external_lib")

# Configure assets header file
configure_file(${INCLUDE_DIR}/helpers/RootDir.h.in
        ${CMAKE_BINARY_DIR}/RootDir.h)

# QuikGrid
set(QUIKGRID_DIR "${LOCAL_THIRD_PARTY_DIR}/QuikGrid")
set(QUIKGRID_INCLUDE_DIR "${QUIKGRID_DIR}/include")
file(GLOB QUIKGRID_SRC_FILES "${QUIKGRID_DIR}/*.cpp")
add_library(quikgrid ${QUIKGRID_SRC_FILES})
target_include_directories(quikgrid
    PUBLIC
        ${QUIKGRID_INCLUDE_DIR}
)
list(APPEND LIBS quikgrid)

# Nlohmann_json
set(NLOHMANN_JSON_DIR "${EXTERNAL_THIRD_PARTY_DIR}/json")
set(NLOHMANN_JSON_INCLUDE_DIR "${EXTERNAL_THIRD_PARTY_DIR}/json/include")
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(${NLOHMANN_JSON_DIR})
list(APPEND LIBS nlohmann_json::nlohmann_json)
#target_include_directories(nlohmann_json::nlohmann_json
#        "${NLOHMANN_JSON_DIR}/include"
#)

# Executable
file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_BINARY_DIR}
        ${QUIKGRID_INCLUDE_DIR}
        ${NLOHMANN_JSON_INCLUDE_DIR}
        "${CMAKE_SOURCE_DIR}/include"
)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${LIBS}
)
